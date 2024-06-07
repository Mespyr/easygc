# easygc
EasyGC is a simple, header-only garbage collector which is most suited be used in generated C code.

### Pros
- **Small:** `easygc` is only around 130 lines long, making it easy to embed into generated C code.
- **Simple:** There are only four functions in the library that you need to use.

### Con
- **Slowish:** Since `easygc` is a reference-counter, it takes slightly longer than other algorithms to actually do stuff.
- **Uses Malloc:** I'm too lazy to implement my own memory allocator 😔
