#ifndef EASY_GC
#define EASY_GC

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct header {
    size_t         size;
    size_t         count;
    struct header *next;
} header_t;

static header_t *freed_mem = NULL;  // first free block of memory
static header_t *used_mem = NULL;   // first used block of memory

void *easygc_alloc(size_t size);
void  easygc_count_ref(void *ptr);
void  easygc_collect(void *ptr);
void  easygc_clean();

#define RAW_PTR(HDR_PTR) ((void *)((uintptr_t)HDR_PTR + sizeof(header_t)))
#define HEADER(PTR) ((header_t *)((uintptr_t)PTR - sizeof(header_t)))
#define ALIGN(SIZE) \
    ((SIZE + (sizeof(uintptr_t) - 1)) & ~(sizeof(uintptr_t) - 1))
#define FATAL_ERROR(msg)      \
    {                         \
        fprintf(stderr, msg); \
        easygc_clean();       \
        exit(1);              \
    }
#ifdef EASY_GC_DEBUG_MODE
#define DEBUG(...) fprintf(stderr, __VA_ARGS__);
#else
#define DEBUG(...) \
    {}
#endif

static header_t *find_free_header(size_t size) {
    if (freed_mem == NULL) return NULL;
    header_t *prev_hdr = NULL;
    header_t *cur_hdr = freed_mem;
    while (cur_hdr != NULL) {
        if (cur_hdr->size < size) {
            prev_hdr = cur_hdr;
            cur_hdr = cur_hdr->next;
            continue;
        }
        // if the first header fit, make the next one the head
        if (prev_hdr == NULL) freed_mem = cur_hdr->next;
        // else, remove the inner header from the list
        else
            prev_hdr->next = cur_hdr->next;
        return cur_hdr;
    }
    return NULL;
}

static bool in_heap(void *ptr) {
    header_t *cur_hdr = used_mem;
    while (cur_hdr != NULL) {
        if (RAW_PTR(cur_hdr) == ptr) return true;
        cur_hdr = cur_hdr->next;
    }
    return false;
}

void *easygc_alloc(size_t size) {
    header_t *h = find_free_header(size);
    if (h != NULL) DEBUG("chunk reused: { size: %d, addr: %p }\n", h->size, h);

    if (h == NULL) {
        // create new header, first get ptr-size aligned size and then malloc
        size_t align_size = ALIGN(size);
        h = (header_t *)malloc(sizeof(header_t) + align_size);
        if (h == NULL) FATAL_ERROR("memory allocation failed!\n");
        h->size = align_size;
        DEBUG("new chunk allocated: { size: %d, addr: %p }\n", h->size, h);
    }
    // put header into used memory
    h->next = used_mem;
    used_mem = h;
    // set initial ref count
    h->count = 1;
    return RAW_PTR(h);
}

void easygc_count_ref(void *ptr) {
    if (in_heap(ptr)) HEADER(ptr)->count++;
}

void easygc_collect(void *ptr) {
    if (!in_heap(ptr)) return;
    header_t *header = HEADER(ptr);
    if (--header->count != 0) return;

    size_t num_chunks = header->size / sizeof(uintptr_t);
    void **p = (void **)ptr;
    for (unsigned int i = 0; i < num_chunks; i++) {
        easygc_collect(*p);
        p = (void **)((uintptr_t)p + sizeof(uintptr_t));
    }
    // remove header from used mem
    header_t *cur_hdr = used_mem;
    header_t *prev_hdr = NULL;
    // cur_hdr
    while (cur_hdr != NULL) {
        if (cur_hdr != header) {
            prev_hdr = cur_hdr;
            cur_hdr = cur_hdr->next;
            continue;
        }
        if (prev_hdr == NULL)
            used_mem = cur_hdr->next;
        else
            prev_hdr->next = cur_hdr->next;
        DEBUG("chunk freed: { size: %d, addr: %p }\n", cur_hdr->size, cur_hdr);
        break;
    }
    // put chunk into free mem
    header->next = freed_mem;
    freed_mem = header;
}

void easygc_clean() {
    // clean up all free blocks
    while (freed_mem != NULL) {
        header_t *t = freed_mem;
        freed_mem = t->next;
        free(t);
    }
    // clean up all used blocks
    while (used_mem != NULL) {
        header_t *t = used_mem;
        used_mem = t->next;
        free(t);
    }
}

#endif
