# easygc
EasyGC is a simple, header-only garbage collector which is most suited for generated C code.

### Pros
- **Small:** EasyGC is less than 200 lines long, making it easy to embed into generated C code.
- **Simple:** There are only four functions in the library that you need to use.

### Con
- **Slowish:** Since EasyGC is a reference-counter, it takes slightly longer than other algorithms to actually do stuff.
- **Uses Malloc:** I'm too lazy to implement my own memory allocator that is cross-platform 😔

## Usage

* EasyGC doesn't require any initialization.

<br>

* To allocate memory on the heap, call the function `easygc_alloc`. This function takes the size of an object you want to allocate, and returns a `void*` to a new chunk of memory. This data is always pointer-aligned (ie: allocating 6 bytes would give you 8 bytes, 21 would give 24, etc...). If this function fails to allocate memory, it prints an error and exits.
```c
void *easygc_alloc(size_t size);
```

<br>

* You can resize a chunk of memory on the heap by calling `easygc_realloc` which takes the pointer to the memory you want to resize, and its new size. If a pointer that is not on the heap is passed into the function, it prints an error and exits. The same happens if the program fails to allocate more memory.
```c
void easygc_realloc(void *ptr, size_t new_size);
```

<br>

* Every time you pass the pointer around (new variable, function call), you have to call the function `easygc_count_ref`. This function literally just increments the reference counter for the pointer passed in.
```c
void easygc_count_ref(void *ptr);
```

<br>

* When a process is done with the pointer, you must call the function `easygc_collect`. This function takes the pointer you finished using and decrements its reference counter. If the reference counter hits 0, it then scans all of the chunks looking for pointers which are tracked by EasyGC, and recursively calls `easygc_count_ref` on them to clear out all unused data.
```c
void easygc_collect(void *root);
```

<br>

* Once the whole program is done, you can call `easygc_clean` to free up all the memory used. It takes no arguments.
