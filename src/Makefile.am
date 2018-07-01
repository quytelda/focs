libfocs_la_CPPFLAGS = -I$(top_srcdir)/include
if DEBUG
libfocs_la_CPPFLAGS += -DDEBUG
libfocs_la_CFLAGS = -g3 -O0
endif

lib_LTLIBRARIES = libfocs.la
libfocs_la_SOURCES = \
	list/double_list.c \
	list/ring_buffer.c \
	list/single_list.c \
	sync/rwlock.c
