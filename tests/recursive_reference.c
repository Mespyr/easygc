#include <stdint.h>

//#define EASY_GC_DEBUG_MODE
#include "../easygc.h"

typedef struct Node {
    int32_t      val;
    struct Node *next;
} Node;

typedef struct LinkedList {
	struct Node *head;
	int32_t size;
} LinkedList;

LinkedList *create_list() {
	LinkedList *xs = easygc_alloc(sizeof(LinkedList));
	xs->size = 0;
	xs->head = NULL;
	return xs;
}

void append_item(LinkedList *xs, int32_t num) {
	easygc_count_ref(xs);
	Node *inner = easygc_alloc(sizeof(Node));
	inner->val = num;

	// set inner->next so we count the ref
	inner->next = xs->head;
	easygc_count_ref(xs->head);

	// before we set xs->head, we collect the previous value
	easygc_collect(xs->head);
	xs->head = inner;
	easygc_count_ref(inner);

	easygc_collect(inner);
	easygc_collect(xs);
}

void print_list(LinkedList *xs) {
	easygc_count_ref(xs);
	printf("[ ");
	Node *x = xs->head;
	while (x != NULL) {
		// we count the reference we're reading from list
		easygc_count_ref(x);
		printf("%d ", x->val);
		// we then collect it after using it
		easygc_collect(x);
		x = x->next;
	}
	printf("]\n");
	easygc_collect(xs);
}

int main() {
    LinkedList *n = create_list();
	append_item(n, 12);
	append_item(n, 14);
	append_item(n, 15);
	append_item(n, 16);
	append_item(n, 17);
	print_list(n);
	easygc_collect(n);

    LinkedList *x = create_list();
	append_item(x, 10);
	append_item(x, 11);
	append_item(x, 12);
	append_item(x, 13);
	print_list(x);
	easygc_collect(x);

	easygc_clean();
    return 0;
}
