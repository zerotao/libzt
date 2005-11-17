noinst_LTLIBRARIES = libzt_event.la

libzt_event_la_SOURCES = \
		private.c \
		select.c  \
		kqueue.c  \
		epoll.c	  \
		poll.c	  \
		devpoll.c \
		gtimer.c

DEFS = @DEFS@ 

CPPFLAGS = -I${top_builddir} -I${top_builddir}/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

pkgincludedir = ${includedir}/libzt/zt_event
pkginclude_HEADERS = \
		private.h \
		interface.h

include $(top_srcdir)/Makefile.extra
