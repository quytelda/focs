include global.mk

LIBS=-lpthread
CFLAGS=-std=c99 -I $(SRC_DIR) -fpic -Wall -pedantic

SRCS=$(addprefix $(SRC_DIR)/, list/linked_list.c sync/rwlock.c)
OBJS=$(SRCS:.c=.o)

PREFIX:=$(if $(PREFIX), $(PREFIX), /usr/lib)

.PHONY: all debug install uninstall clean check

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(OBJS)

debug: DEBUG_FLAGS=-g -DDEBUG
debug: $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) $(OBJS)

install: $(BIN) $(PREFIX)
	install --owner=0 --group=0 --mode=644 $(BIN) $(PREFIX)
	ldconfig

uninstall: $(PREFIX)/$(BIN)
	rm -f $(PREFIX)/$(BIN)
	ldconfig

clean:
	-$(RM) $(BIN) $(OBJS)
	$(MAKE) -C $(CK_DIR) clean

check:
	$(MAKE) -C $(CK_DIR)
	$(MAKE) -C $(CK_DIR) check
