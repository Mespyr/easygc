BIN=test

all: $(BIN)

$(BIN): easygc.h main.c
	$(CC) main.c -o $(BIN) 

clean:
	$(RM) $(BIN)
