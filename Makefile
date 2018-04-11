CC=gcc
CFLAGS=-std=c99 -I . -fpic -Wall -pedantic -lpthread

BIN=focs.so
SRCS=list/linked_list.c sync/rwlock.c
OBJS=$(SRCS:.c=.o)

.PHONY: clean debug

all: $(BIN)

$(BIN): $(OBJS)
	gcc -shared -o $(BIN) $(CFLAGS) $(OBJS)

debug: DEBUG_FLAGS=-g -DDEBUG
debug: $(OBJS)
	gcc -o $(BIN) $(CFLAGS) $(DEBUG_FLAGS) $(OBJS)

.c.o:
	$(CC) -c $(CFLAGS) $(DEBUG_FLAGS) -o $@ $<

clean:
	rm $(BIN) $(OBJS)
