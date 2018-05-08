include global.mk

LIBS = -lpthread
CFLAGS := -std=gnu99 -I $(INC_DIR) -fpic -Wall $(CFLAGS)

SRCS=$(addprefix $(SRC_DIR)/, \
	list/single_list.c    \
	list/double_list.c    \
	list/ring_buffer.c    \
	map/hash_map.c        \
	sync/rwlock.c)
OBJS=$(SRCS:.c=.o)

# Documentation Type (default to 'html')
# This variable may be overriden by the user
# in the environmental variable "$DOC_TYPE"
DOC_TYPE := $(if $(DOC_TYPE), $(DOC_TYPE), html)
DOXYFILE = $(DOC_DIR)/doxyfile

# Installation Prefix (default to '/usr')
# This variable may be overriden by the user
# in the environmental variable "$PREFIX"
# A common override value is /usr/local
PREFIX := $(if $(PREFIX), $(PREFIX), /usr)
LIB_PREFIX := $(PREFIX)/lib
INC_PREFIX := $(PREFIX)/include

.PHONY: all debug docs install uninstall clean check

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(OBJS)

debug: DEBUG_FLAGS = -g -DDEBUG
debug: $(OBJS)
	$(CC) -shared -o $(BIN) $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) $(OBJS)

docs: $(DOXYFILE)
	doxygen $(DOXYFILE)
	$(MAKE) -C $(DOC_DIR) $(DOC_TYPE)

install: $(BIN) $(LIB_PREFIX) $(INC_PREFIX)
	install --owner=0 --group=0 --mode=644 $(BIN) $(LIB_PREFIX)
	ldconfig

	cp $(INC_DIR)/focs.h $(INC_PREFIX)
	cp -R $(INC_DIR)/focs $(INC_PREFIX)
	cp -R $(INC_DIR)/list $(INC_PREFIX)
	cp -R $(INC_DIR)/sync $(INC_PREFIX)

uninstall: $(LIB_PREFIX)/$(BIN)
	rm -f $(LIB_PREFIX)/$(BIN)
	ldconfig

	rm -f $(INC_PREFIX)/focs.h
	rm -rf $(INC_PREFIX)/focs
	rm -rf $(INC_PREFIX)/list
	rm -rf $(INC_PREFIX)/sync

clean:
	-$(RM) $(BIN) $(OBJS)
	-$(RM) -r $(DOC_DIR)/_build $(DOC_DIR)/xml
	-find $(DOC_DIR)/sphinx/breathe \
		-name "__pycache__"     \
		-type d                 \
		-exec rm -rv {} +
	$(MAKE) -C $(CK_DIR) clean

check:
	$(MAKE) -C $(CK_DIR)
	$(MAKE) -C $(CK_DIR) check
