#include "../easygc.h"

void too_large_alloc() {
	int *int_arr = easygc_alloc(100000000000000000);
	int_arr[100] = 20;
	printf("%d\n", int_arr[100]);
	easygc_collect(int_arr);
}

void realloc_too_large() {
	int *int_arr = easygc_alloc(100 * sizeof(int));
	int_arr = easygc_realloc(int_arr, 10000000000000000);
	easygc_collect(int_arr);
}

void too_many_refs() {
	int *int_arr = easygc_alloc(100 * sizeof(int));
	// artificially boost the amount of references there are to the pointer
	easygc_count_ref(int_arr);
	easygc_count_ref(int_arr);
	easygc_count_ref(int_arr);
	int_arr = easygc_realloc(int_arr, 200 * sizeof(int));
	// collect it 4 times
	easygc_collect(int_arr);
	easygc_collect(int_arr);
	easygc_collect(int_arr);
	easygc_collect(int_arr);
}

int main() {
	too_many_refs();
	//realloc_too_large();
	//too_large_alloc();
	//easygc_clean();
	return 0;
}
