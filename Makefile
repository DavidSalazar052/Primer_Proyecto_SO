CC=gcc
CFLAGS=-Wall -Wextra -pthread -Iinclude
SRC=$(wildcard src/*.c)
OUT=bin/terminal_carga

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p bin logs
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -f logs/*.log

.PHONY: all run clean
