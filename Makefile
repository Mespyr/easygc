BIN=test

all: $(BIN)

$(BIN): easygc.h main.c
	$(CC) main.c -o $(BIN) 

clean:
	$(RM) $(BIN)

format: easygc.h main.c
	clang-format -i easygc.h main.c -style=file
