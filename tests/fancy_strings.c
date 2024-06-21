#include <string.h>
//#define EASY_GC_DEBUG_MODE
#include "../easygc.h"

typedef struct {
	size_t size;
	char *data;
} string;

string *new_string(const char *data, size_t size) {
	char *alloc = easygc_alloc(size);
	memcpy(alloc, data, size);

	string *ret = easygc_alloc(sizeof(string));
	ret->size = size;
	ret->data = alloc;
	easygc_count_ref(alloc);

	// return checks
	easygc_collect(alloc);
	return ret;
}

void pushc(string *str, char c) {
	easygc_count_ref(str);
	void *new_data = easygc_realloc(str->data, str->size + 1);
	str->data = new_data;
	str->data[str->size++] = c;
	easygc_collect(str);
}

void print(string *str) {
	easygc_count_ref(str);
	printf("%s", str->data);
	easygc_collect(str);
}

int main() {
	string *hello = new_string("Hello ", 6);
	pushc(hello, 'w');
	pushc(hello, 'o');
	pushc(hello, 'r');
	pushc(hello, 'l');
	pushc(hello, 'd');
	pushc(hello, '!');
	pushc(hello, '\n');
	pushc(hello, '\0');

	print(hello);

	easygc_clean();
	return 0;
}
