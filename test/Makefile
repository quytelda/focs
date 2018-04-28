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
