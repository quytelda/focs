CC = gcc
RM = rm -f

BIN = libfocs.so
INC_DIR = include
SRC_DIR = src
DOC_DIR = docs
CK_DIR  = test

.c.o:
	$(CC) -c $(CFLAGS) $(DEBUG_FLAGS) -o $@ $< $(LIBS)
