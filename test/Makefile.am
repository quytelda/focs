FOCS_INCDIR = $(top_srcdir)/include
FOCS_LTLIB  = $(top_builddir)/src/libfocs.la

TESTS = double_list ring_buffer single_list
check_PROGRAMS = double_list ring_buffer single_list

double_list_SOURCES  = list/double_list.c
double_list_CPPFLAGS = -I$(FOCS_INCDIR)
double_list_CFLAGS   = @CHECK_CFLAGS@
double_list_LDADD    = $(FOCS_LTLIB) @CHECK_LIBS@

ring_buffer_SOURCES  = list/ring_buffer.c
ring_buffer_CPPFLAGS = -I$(FOCS_INCDIR)
ring_buffer_CFLAGS   = @CHECK_CFLAGS@
ring_buffer_LDADD    = $(FOCS_LTLIB) @CHECK_LIBS@

single_list_SOURCES  = list/single_list.c
single_list_CPPFLAGS = -I$(FOCS_INCDIR)
single_list_CFLAGS   = @CHECK_CFLAGS@
single_list_LDADD    = $(FOCS_LTLIB) @CHECK_LIBS@
