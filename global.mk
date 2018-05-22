TGT=focs
BIN=lib$(TGT).so

CC=gcc
RM=rm -f
LDCONFIG=/sbin/ldconfig

INC_DIR=include
SRC_DIR=src
DOC_DIR=docs
CK_DIR=test

.c.o:
	$(CC) -c $(CFLAGS) $(LIBS) $(DEBUG_FLAGS) -o $@ $<
