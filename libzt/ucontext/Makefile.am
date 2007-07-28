noinst_LTLIBRARIES = libzt_ucontext.la

libzt_ucontext_la_SOURCES = \
		portableucontext_asm.s	\
		portableucontext.c
DEFS = @DEFS@ 

AM_CPPFLAGS = -I${top_builddir} -I${top_builddir}/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

pkgincludedir = ${includedir}/libzt/ucontext
pkginclude_HEADERS = 			\
		portableucontext.h	\
		portableucontext386.h	\
		portableucontextppc.h

include $(top_srcdir)/Makefile.extra
