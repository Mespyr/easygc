CFLAGS=-std=c99 -O3 -flto -s -w -finline-functions -funroll-loops -march=native

ALL_TESTS=tests/recursive_reference tests/array tests/fancy_strings tests/fails

all: $(ALL_TESTS)

define compile_test
$(1): easygc.h $(1).c
	$(CC) $(CFLAGS) $(1).c -o $$@
endef
$(foreach test, $(ALL_TESTS), $(eval $(call compile_test, $(test))))

clean:
	$(RM) $(ALL_TESTS)

format: easygc.h
	clang-format -i easygc.h -style=file
