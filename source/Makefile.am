bin_PROGRAMS =
lib_LTLIBRARIES = libcoroutine.la
include_HEADERS = coroutine.h coroutine++.h

libcoroutine_la_SOURCES = 386-ucontext.h amd64-ucontext.h asm.S context.c Coro.c Coro.h power-ucontext.h buildopts.h coroutine++.cpp coroutine++.h
libcoroutine_la_LDFLAGS =

clean-local:
	rm -f coroutine.h

header_verbose = $(header_verbose_$(V))
header_verbose_ = $(header_verbose_$(AM_DEFAULT_VERBOSITY))
header_verbose_0 = @echo "  HEADER" $@;

coroutine.h: buildopts.h Coro.h
	$(header_verbose)cat $^ > $@; sed -i 's/#include "buildopts.h"//' $@

coroutine++.cpp coroutine++.h: coroutine.h
