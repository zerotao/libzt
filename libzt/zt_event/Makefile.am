noinst_LTLIBRARIES = libzt_event.la

libzt_event_la_SOURCES = \
		zt_event_private.c \
		zt_event_net.c \
		zt_event_timer.c \
		zt_event_signal.c \
		zt_event_unix.c

DEFS := @DEFS@ -I/usr/include -I. -I$(srcdir) -I../..

pkgincludedir = ${includedir}/zt_event
pkginclude_HEADERS = \
		zt_event_private.h \
		zt_event_net.h \
		zt_event_timer.h \
		zt_event_signal.h \
		zt_event_unix.h

include $(top_srcdir)/Makefile.extra
