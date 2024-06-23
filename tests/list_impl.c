#include <stdint.h>
#define EASY_GC_DEBUG_MODE
#include "../easygc.h"

typedef struct {
	size_t size;
	uint64_t *data;
} list_u64;

list_u64 *list_u64_new(size_t initial_size) {
	list_u64 *xs = easygc_alloc(sizeof(list_u64));
	xs->data = easygc_alloc(initial_size * sizeof(uint64_t));
	xs->size = 0;
	return xs;
}

void append(list_u64 *xs, uint64_t n) {
	easygc_count_ref(xs);
	size_t max_list_size = HEADER(xs->data)->size / sizeof(uint64_t);
	if (xs->size >= max_list_size)
		xs->data = easygc_realloc(xs->data, ALIGN(max_list_size + 1) * sizeof(uint64_t));
	xs->data[xs->size] = n;
	xs->size++;
	easygc_collect(xs);
}

void print(list_u64 *xs) {
	easygc_count_ref(xs);

	printf("[ ");
	for (unsigned int i = 0; i < xs->size; i++)
		printf("%lu ", xs->data[i]);
	printf("]\n");
	
	easygc_collect(xs);
}

int main() {
	list_u64 *new_list = list_u64_new(10);
	for (int i = 0; i <= 30; i++)
		append(new_list, i);
	
	print(new_list);

	easygc_collect(new_list);
	easygc_clean();
	return 0;
}
