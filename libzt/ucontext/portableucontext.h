/* 
This software was developed as part of a project at MIT.
 
 Copyright (c) 2005-2006 Russ Cox,
 Massachusetts Institute of Technology
 
 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
												 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef PORTABLEUCONTEXT_DEFINED 
#define PORTABLEUCONTEXT_DEFINED 1

#if defined(linux) || defined(__NetBSD__) || defined(__FreeBSD__) || (defined(__SVR4) && defined (__sun))
# define HAS_UCONTEXT 1
#endif

#if defined(__APPLE__)
# define APPLE_MISSING_UCONTEXT 1
#endif

#if defined(__FreeBSD__) ||  defined(APPLE_MISSING_UCONTEXT)
#include <string.h>				/* memmove */
#include <inttypes.h>			/* intptr_t */
//#include <ucontext.h>
#include <signal.h>
typedef unsigned long ulong;

#  if defined(__FreeBSD__) && __FreeBSD__ < 5
    extern	int		getmcontext(mcontext_t*);
    extern	void	setmcontext(mcontext_t*);
#   define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#   define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
    extern	int		swapcontext(ucontext_t*, ucontext_t*);
    extern	void	makecontext(ucontext_t*, void(*)(), int, ...);
#endif	/* defined(__FreeBSD__) && __FreeBSD__ < 5 */

#if defined(APPLE_MISSING_UCONTEXT)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined(__i386__)
#     define INCLUDE_386
#	else
#     define INCLUDE_PPC
#	endif	
#endif	/* defiend(APPLE_MISSING_UCONTEXT) */

#if defined(__OpenBSD__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#	if defined __i386__
#     define INCLUDE_386
#	else
#     define INCLUDE_PPC
#	endif
extern pid_t rfork_thread(int, void*, int(*)(void*), void*);
#endif	/* defined(__OpenBSD__) */

#if 0 &&  defined(__sun__)
#	define mcontext libthread_mcontext
#	define mcontext_t libthread_mcontext_t
#	define ucontext libthread_ucontext
#	define ucontext_t libthread_ucontext_t
#   define INCLUDE_SPARC
#endif	/* 0 &&  defined(__sun__) */

#if defined(__arm__)
    int getmcontext(mcontext_t*);
    void setmcontext(const mcontext_t*);
#   define	setcontext(u)	setmcontext(&(u)->uc_mcontext)
#   define	getcontext(u)	getmcontext(&(u)->uc_mcontext)
#endif	/* defined(__arm__) */

// --------------------------

#if defined(APPLE_MISSING_UCONTEXT) && defined(__i386__)
#   define NEEDX86MAKECONTEXT
#   define NEEDSWAPCONTEXT
#   define HAS_UCONTEXT 1
#endif	/* defined(APPLE_MISSING_UCONTEXT) && defined(__i386__) */

#if defined(APPLE_MISSING_UCONTEXT) && defined(__ppc__)
#   define NEEDPOWERMAKECONTEXT
#   define NEEDSWAPCONTEXT
#   define HAS_UCONTEXT 1
#endif	/* defined(APPLE_MISSING_UCONTEXT) && defined(__ppc__) */

#if defined(__FreeBSD__) && defined(__i386__) && __FreeBSD__ < 5
#   define NEEDX86MAKECONTEXT
#   define NEEDSWAPCONTEXT
#   define HAS_UCONTEXT 1
#endif	/* defined(__FreeBSD__) && defined(__i386__) && __FreeBSD__ < 5 */

#endif	/* defined(__FreeBSD__) ||  defined(APPLE_MISSING_UCONTEXT) */

/*
  capture any include information needed
 */

/* 386 only entries */
#if defined(INCLUDE_386)
# include <libzt/ucontext/portableucontext386.h>
#endif

#if defined(INCLUDE_PPC)
# include <libzt/ucontext/portableucontextppc.h>
#endif

#if defined(INCLUDE_SPARC)
#endif	/* INCLUDE_SPARC */

#endif	/* PORTABLEUCONTEXT_DEFINED */

