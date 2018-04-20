include global.mk

LIBS=-lpthread
CFLAGS=-std=c99 -I $(SRC_DIR) -fpic -Wall -pedantic

SRCS=$(addprefix $(SRC_DIR)/, list/linked_list.c sync/rwlock.c)
OBJS=$(SRCS:.c=.o)

.PHONY: all debug clean check

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(OBJS)

debug: DEBUG_FLAGS=-g -DDEBUG
debug: $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) $(OBJS)

clean:
	-rm $(BIN) $(OBJS)
	$(MAKE) -C $(CK_DIR) clean

check:
	$(MAKE) -C $(CK_DIR)
	$(MAKE) -C $(CK_DIR) check
