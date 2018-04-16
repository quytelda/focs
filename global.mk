TGT=focs
BIN=lib$(TGT).so

CC=gcc
SRC_DIR=src
CK_DIR=test

.PHONY: debug

debug: DEBUG_FLAGS=-g -DDEBUG

.c.o:
	$(CC) -c $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) -o $@ $<
