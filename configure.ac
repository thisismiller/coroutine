AC_INIT(Coroutine, 1.0, millerdevel@gmail.com)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
LT_INIT
AC_LANG([C++])

AM_PROG_AS
AC_PROG_CC
AC_PROG_CXX

CFLAGS="-O0 -g -Wall -std=c99"
CXXFLAGS="-O0 -g -Wall -std=gnu++0x"

dnl Valgrind support {{{
AC_ARG_ENABLE([valgrind],AC_HELP_STRING([--enable-valgrind],[Valgrind support]),[ENABLE_VALGRIND="ENABLE_VALGRIND"],[ENABLE_VALGRIND="NO_ENABLE_VALGRIND"])
if test "x$ENABLE_VALGRIND" = "xENABLE_VALGRIND"; then
  AC_CHECK_PROG([HAVE_VALGRIND],[valgrind],["yes"],["no"])
  if test "x$HAVE_VALGRIND" = "xno"; then
    AC_MSG_ERROR([You need to install valgrind.])
  fi
fi
AC_SUBST([ENABLE_VALGRIND],$ENABLE_VALGRIND,[Valgrind support])
dnl }}}

dnl Coroutine Implementation {{{
AC_CHECK_FUNC([GetCurrentFiber],[HAVE_FIBERS="yes"],[HAVE_FIBERS="no"])

AC_MSG_CHECKING([for ucontext])
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([#include <sys/ucontext.h>], [ucontext_t ucontext])],
  [HAVE_UCONTEXT="yes"], [HAVE_UCONTEXT="no"])
AC_MSG_RESULT($HAVE_UCONTEXT)

AC_MSG_CHECKING([for setjmp])
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([#include <setjmp.h>], [jmp_buf jmpbuf])],
  [HAVE_UCONTEXT="yes"], [HAVE_UCONTEXT="no"])
AC_MSG_RESULT($HAVE_UCONTEXT)

if test "x$HAVE_FIBERS" = "xyes"; then
  CORO_IMPLEMENTATION="fibers"
  CORO_USE_TYPE="USE_FIBERS"
elif test "x$HAVE_UCONTEXT" = "xyes"; then
  CORO_IMPLEMENTATION="ucontext"
  CORO_USE_TYPE="USE_UCONTEXT"
else
  CORO_IMPLEMENTATION="setjmp"
  CORO_USE_TYPE="USE_SETJMP"
fi

AC_SUBST([CORO_IMPLEMENTATION],$CORO_IMPLEMENTATION,[The implementation method])
AC_SUBST([CORO_USE_TYPE],$CORO_USE_TYPE,[The define for the implementation method])
dnl }}}

AC_CONFIG_FILES(
  Makefile
  source/buildopts.h
  source/Makefile
  samples/Makefile
)

AC_OUTPUT
dnl vim: set ft=config foldmethod=marker foldmarker={{{,}}} :
