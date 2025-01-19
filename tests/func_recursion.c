#define EASY_GC_DEBUG_MODE
#include "../easygc.h"

typedef struct Node {
    int32_t      val;
    struct Node *next;
} Node;

typedef struct {
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

void recursive_print_node(Node *n) {
	easygc_count_ref(n);
	printf("%d ", n->val);

	Node *next = n->next;
	easygc_count_ref(next);
	if (next != NULL) recursive_print_node(next);
	easygc_collect(next);
	
	easygc_collect(n);
}
void print_list(LinkedList *xs) {
	easygc_count_ref(xs);
	printf("[ ");
	Node *head = xs->head;
	easygc_count_ref(head);
	if (head != NULL) recursive_print_node(head);
	easygc_collect(head);
	easygc_collect(xs);
	printf("]\n");
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

	easygc_clean();
	return 0;
}
