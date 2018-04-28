include ../global.mk

LIBS = -lcheck -lpthread -lm -lrt -L .. -l$(TGT)

# Check for the presence of libsubunit (optional dependancy of libcheck).
# If it exists on this system, the tests should link to it.
subunit = subunit
libsubunit_test = $(shell /sbin/ldconfig -p | \
			grep libsubunit     | \
			cut -d ' ' -f 1     | \
			tail -1             | \
			tr -d '[:space:]')
ifeq ($(libsubunit_test), lib$(subunit).so)
LIBS := $(LIBS) -l$(subunit)
endif

CFLAGS = -I ../$(INC_DIR) -g

TESTS = $(TEST_LINKLIST_BIN) $(TEST_RB_BIN)

# The test suite for doubly linked lists
TEST_LINKLIST_BIN = double_list
TEST_LINKLIST_SRCS = list/double_list.c
TEST_LINKLIST_OBJS = $(TEST_LINKLIST_SRCS:.c=.o)

# The test suite for ring buffers
TEST_RB_BIN = ring_buffer
TEST_RB_SRCS = list/ring_buffer.c
TEST_RB_OBJS = $(TEST_RB_SRCS:.c=.o)


all: $(TESTS)

$(TEST_LINKLIST_BIN): $(TEST_LINKLIST_OBJS)
	$(CC) -o $(TEST_LINKLIST_BIN) $(TEST_LINKLIST_OBJS) $(CFLAGS) $(LIBS)

$(TEST_RB_BIN): $(TEST_RB_OBJS)
	$(CC) -o $(TEST_RB_BIN) $(TEST_RB_OBJS) $(CFLAGS) $(LIBS)

check: $(TESTS)
	@for test in $(TESTS); do LD_LIBRARY_PATH=.. ./$$test; done

clean:
	-$(RM) $(TESTS) $(TEST_LINKLIST_OBJS)
