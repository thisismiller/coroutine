/*
*/

#ifndef CORO_DEFINED
#define CORO_DEFINED 1

#include "buildopts.h"
//#include "Common.h"
//#include "PortableUContext.h"


// Needed for ucontext to compile
#ifndef _BSD_SOURCE
#  define _BSD_SOURCE
#endif

#ifdef USE_UCONTEXT
#  include <ucontext.h>
#endif
#if USE_SETJMP
#  include <setjmp.h>
#endif
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <sys/utsname.h>
#include <inttypes.h>
#include <signal.h>

#if defined(__sun__)
#	define __EXTENSIONS__ 1 /* SunOS */
#	if defined(__SunOS5_6__) || defined(__SunOS5_7__) || defined(__SunOS5_8__)
		/* NOT USING #define __MAKECONTEXT_V2_SOURCE 1 / * SunOS */
#	else
#		define __MAKECONTEXT_V2_SOURCE 1
#	endif
#endif

//#define USE_UCONTEXT 1

#if defined(__OpenBSD__)
#undef USE_UCONTEXT
#define USE_UCONTEXT 0
#endif

#if defined(__APPLE__)
#include <AvailabilityMacros.h>
#if defined(MAC_OS_X_VERSION_10_5)
#undef USE_UCONTEXT
#define USE_UCONTEXT 0
#endif
#endif

#define nil ((void*)0)
#define nelem(x) (sizeof(x)/sizeof((x)[0]))

#if defined(__FreeBSD__) && __FreeBSD__ < 5
extern	int		getmcontext(mcontext_t*);
extern	void		setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
extern	int		swapcontext(ucontext_t*, const ucontext_t*);
extern	void		makecontext(ucontext_t*, void(*)(), int, ...);
#endif

#if defined(__APPLE__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined(__i386__)
#		include "386-ucontext.h"
#	elif defined(__x86_64__)
#		include "amd64-ucontext.h"
#	else
#		include "power-ucontext.h"
#	endif	
#endif

#if defined(__OpenBSD__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined __i386__
#		include "386-ucontext.h"
#	else
#		include "power-ucontext.h"
#	endif
extern pid_t rfork_thread(int, void*, int(*)(void*), void*);
#endif

#if defined(__arm__)
int getmcontext(mcontext_t*);
void setmcontext(const mcontext_t*);
#define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif

#if !defined(__MINGW32__) && defined(WIN32)
#if defined(BUILDING_CORO_DLL) || defined(BUILDING_IOVMALL_DLL)
#define CORO_API __declspec(dllexport)
#else
#define CORO_API __declspec(dllimport)
#endif

#else
#define CORO_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (CoroStartCallback)(void *);

typedef struct CallbackBlock
{
	CoroStartCallback *func;
	void *context;
} CallbackBlock;

typedef struct Coro Coro;

struct Coro
{
	size_t requestedStackSize;
	size_t allocatedStackSize;
	void *stack;
  CallbackBlock cbBlock;

#ifdef USE_VALGRIND
	unsigned int valgrindStackId;
#endif

#if defined(USE_FIBERS)
	void *fiber;
#elif defined(USE_UCONTEXT)
	ucontext_t env;
#elif defined(USE_SETJMP)
	jmp_buf env;
#endif

	unsigned char isMain;
};

CORO_API Coro *Coro_new(void);
CORO_API void Coro_init(Coro *self);
CORO_API void Coro_free(Coro *self);
CORO_API void Coro_destroy(Coro *self);

// stack

CORO_API void *Coro_stack(Coro *self);
CORO_API size_t Coro_stackSize(Coro *self);
CORO_API void Coro_setStackSize(Coro *self, size_t sizeInBytes);
CORO_API size_t Coro_bytesLeftOnStack(Coro *self);
CORO_API int Coro_stackSpaceAlmostGone(Coro *self);
CORO_API void Coro_setDefaultStackSize(size_t defaultSize);

CORO_API void Coro_initializeMainCoro(Coro *self);


CORO_API void Coro_setup(Coro *self, CoroStartCallback *callback, void *context);
CORO_API void Coro_switchTo(Coro *self, Coro *next);

#ifdef __cplusplus
}
#endif
#endif
