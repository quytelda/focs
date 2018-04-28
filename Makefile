include global.mk

LIBS = -lpthread
CFLAGS = -std=c99 -I $(INC_DIR) -fpic -Wall -pedantic

SRCS = $(addprefix $(SRC_DIR)/, list/double_list.c sync/rwlock.c)
OBJS = $(SRCS:.c=.o)

# Installation Prefix (default to '/usr')
# This variable may be overriden by the user
# in the environmental variable "$PREFIX"
# A common override value is /usr/local
PREFIX := $(if $(PREFIX), $(PREFIX), /usr)
LIB_PREFIX := $(PREFIX)/lib
INC_PREFIX := $(PREFIX)/include

.PHONY: all debug install uninstall clean check

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(OBJS)

debug: DEBUG_FLAGS = -g -DDEBUG
debug: $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) $(OBJS)

install: $(BIN) $(LIB_PREFIX) $(INC_PREFIX)
	install --owner=0 --group=0 --mode=644 $(BIN) $(LIB_PREFIX)
	ldconfig

	cp $(INC_DIR)/focs.h $(INC_PREFIX)
	cp $(INC_DIR)/metadata.h $(INC_PREFIX)
	cp -R $(INC_DIR)/list $(INC_PREFIX)
	cp -R $(INC_DIR)/sync $(INC_PREFIX)

uninstall: $(LIB_PREFIX)/$(BIN)
	rm -f $(LIB_PREFIX)/$(BIN)
	ldconfig

	rm -f $(INC_PREFIX)/focs.h
	rm -f $(INC_PREFIX)/metadata.h
	rm -rf $(INC_PREFIX)/list
	rm -rf $(INC_PREFIX)/sync

clean:
	-$(RM) $(BIN) $(OBJS)
	$(MAKE) -C $(CK_DIR) clean

check:
	$(MAKE) -C $(CK_DIR)
	$(MAKE) -C $(CK_DIR) check
