#define EASY_GC_DEBUG_MODE
#include "../easygc.h"

typedef struct node_t {
	struct node_t *next;
	int data;
} node;

void print(node *xs, int count) {
	easygc_count_ref(xs);

	node *m = xs;
	for (int x = 0; x < count; x++) {
		printf("%d\n", m->data);
		m = m->next;
	}

	easygc_collect(xs);
}

int main() {
	node *x = easygc_alloc(sizeof(node)); // x->count = 1
	node *y = easygc_alloc(sizeof(node)); // y->count = 1

	x->data = 12;
	x->next = y;
	easygc_count_ref(y); // y->count = 2

	y->data = 13;
	y->next = x;
	easygc_count_ref(x); // x->count = 2

	// variable y is not used anymore
	easygc_collect(y);  // y->count = 1

	print(x, 10);
	// variable x isn't used anymore
	easygc_collect(x); // x->count = 1

	// in the end, both have a reference count of 1
	// because of their recursion
	// so they dont get cleared
	// this probably shouldn't happen
	// i just need to figure out a solution

	easygc_clean();
	return 0;
}
