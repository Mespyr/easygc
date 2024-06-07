#include "easygc.h"

typedef struct LinkNum {
    int32_t         num;
    struct LinkNum *next;
} LinkNum;

int main() {
    // create new LinkNum struct, put it inside the first struct,
    // and collect the `inner` ptr.
    // since we incremented the count with `easygc_count_ref`,
    // it wont get deallocated until the first LinkNum is.

    LinkNum *n = easygc_alloc(sizeof(LinkNum));
    n->num = 12;

    LinkNum *inner = easygc_alloc(sizeof(LinkNum));
    inner->num = 14;

    // since we're putting the ptr into the struct,
    // we increment the ref count for the ptr
    n->next = inner;
    easygc_count_ref(inner);

    // we don't use `inner` variable anymore, so we collect it
    easygc_collect(inner);

    printf("%d, %d\n", n->num, n->next->num);

    easygc_collect(n);  // collect `n`, which should deallocate both

    easygc_clean();
    return 0;
}
