# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([focs], [0.1.0], [quytelda@tamalin.org])
AC_CONFIG_SRCDIR([src/list/double_list.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

# Process configure options.
AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],
		[Enable debugging support.]),
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*)   AC_MSG_ERROR([Invalid value ${enableval}.]) ;;
	esac],
	[debug=false])
AM_CONDITIONAL([DEBUG], [test "x$debug" = "xtrue"])

# Check for programs.
AM_PROG_AR
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Check for libraries.
AC_CHECK_LIB([pthread], [
	pthread_mutex_init,
	pthread_mutex_destroy,
	pthread_mutex_lock,
	pthread_mutex_unlock,
	pthread_cond_init,
	pthread_cond_destroy,
	pthread_cond_wait,
	pthread_cond_broadcast
])
PKG_CHECK_MODULES([CHECK], [check >= 0.10.0], []
	[AC_MSG_WARN("Install 'check' if you intend to use 'make check'.")])

# Check for header files.
AC_CHECK_HEADER_STDBOOL
AC_CHECK_HEADERS([errno.h pthread.h stdint.h stdio.h stdlib.h string.h])
AC_CHECK_HEADERS([check.h])

# Check for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Check for library functions and symbols.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP

# ./configure should output these files.
AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_OUTPUT
