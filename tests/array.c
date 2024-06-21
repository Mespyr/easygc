#include "../easygc.h"

int *create_array(size_t size) {
	int *ptr = easygc_alloc(sizeof(int) * size);
	// returned items should not be collected
	// since theyre passed on to the outside scope
	return ptr;
}

void print_array(int *arr, size_t size) {
	easygc_count_ref(arr);

	printf("[ ");
	for (unsigned int i = 0; i < size; i++)
		printf("%d ", arr[i]);
	printf("]\n");
	
	easygc_collect(arr);
}

int main() {
	int *arr = create_array(10);
	arr[0] = 11;
	arr[1] = 12;
	arr[2] = 13;
	arr[3] = 14;
	arr[4] = 15;
	arr[5] = 16;
	arr[6] = 17;
	arr[7] = 18;
	arr[8] = 19;
	arr[9] = 20;
	print_array(arr, 10);
	easygc_collect(arr);

	easygc_clean();
	return 0;
}
