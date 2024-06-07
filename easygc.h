#ifndef EASY_GC
#define EASY_GC

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct header {
	size_t size;
	struct header *next;
} header_t;

static header_t  base = {.size = 0, .next = NULL}; // base; zero-sized block
static header_t *freed_mem = &base; // first free block of memory
static header_t *used_mem = NULL;   // first used block of memory

static header_t *find_free_header(size_t size);
void *gc_alloc(size_t size);
void gc_collect(void *root);

#ifndef EASY_GC_IMPLEMENTATION
#define EASY_GC_IMPLEMENTATION

#define PTR_ADD(PTR, DIFF) (void*)((uintptr_t)PTR + DIFF)
#define PTR_SUB(PTR, DIFF) (void*)((uintptr_t)PTR - DIFF)

static header_t *find_free_header(size_t size) {
	if (freed_mem == &base) return NULL;

	header_t* prev_hdr = NULL;
	header_t* cur_hdr = freed_mem;

	while (cur_hdr != &base) {
		if (cur_hdr->size < size) {
			prev_hdr = cur_hdr;
			cur_hdr = cur_hdr->next;
			continue;
		}
		// if the first header fit, make the next one the head
		if (prev_hdr == NULL) freed_mem = cur_hdr->next;
		// else, remove the inner header from the list
		else prev_hdr->next = cur_hdr->next;
		return cur_hdr;
	}
	return NULL;
}

void *gc_alloc(size_t size) {
	header_t* h = find_free_header(size);
	if (h == NULL) {
		// create new header, first get ptr-size aligned size and then malloc
		size_t align_size = ((size + (sizeof(uintptr_t) - 1)) & ~(sizeof(uintptr_t) - 1));
		h = (header_t*)malloc(sizeof(header_t) + align_size);
		if (h == NULL) {
			fprintf(stderr, "memory allocation failed!\n");
			exit(1);
		}
		h->size = align_size;
	}
	// put header into used memory
	h->next = used_mem;
	used_mem = h;
	return PTR_ADD(h, sizeof(header_t));
}

void gc_collect(void *root) {
	header_t *header = (header_t*)PTR_SUB(root, sizeof(header_t));
	//printf("%zu\n", header->size);
}

#endif
#endif
