TGT=focs
BIN=lib$(TGT).so

CC=gcc
RM=rm -f

INC_DIR=include
SRC_DIR=src
DOC_DIR=docs
CK_DIR=test

.c.o:
	$(CC) -c $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) -o $@ $<
