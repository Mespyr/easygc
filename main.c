#include "easygc.h"

// User Code
typedef struct {
	int32_t x;
	int32_t y;
} Point;

int main() {
	Point *p = gc_alloc(sizeof(Point));

	p->x = 12;
	p->y = 13;
	printf("%d, %d\n", p->x, p->y);

	gc_collect(p);
	
	Point *p2 = gc_alloc(sizeof(Point));
	p2->x = 4;
	p2->y = 17;
	printf("%d, %d\n", p2->x, p2->y);

	gc_collect(p2);

	return 0;
}
