include ../global.mk

LIBS=-lcheck -L .. -l$(TGT)
CFLAGS=-I ../$(INC_DIR) -g

TESTS=$(TEST_LINKLIST_BIN)

TEST_LINKLIST_BIN=linklist
TEST_LINKLIST_SRCS=list/linked_list.c
TEST_LINKLIST_OBJS=$(TEST_LINKLIST_SRCS:.c=.o)

all: $(TESTS)

$(TEST_LINKLIST_BIN): $(TEST_LINKLIST_OBJS)
	$(CC) -o $(TEST_LINKLIST_BIN) $(TEST_LINKLIST_OBJS) $(CFLAGS) $(LIBS)

check: $(TESTS)
	@for test in $(TESTS); do LD_LIBRARY_PATH=.. ./$$test; done

clean:
	-$(RM) $(TESTS) $(TEST_LINKLIST_OBJS)
