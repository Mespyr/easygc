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
	node *x = easygc_alloc(sizeof(node));
	node *y = easygc_alloc(sizeof(node));

	x->data = 12;
	x->next = y;
	easygc_count_ref(y);

	y->data = 13;
	y->next = x;
	easygc_count_ref(x);

	easygc_collect(y); // y is not used anymore

	print(x, 10);
	easygc_collect(x);

	easygc_clean();
	return 0;
}
