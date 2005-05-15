noinst_LTLIBRARIES = libzt_bstream.la

libzt_bstream_la_SOURCES = \
		zt_bstream_private.c \
		zt_bstream_file.c \
		zt_bstream_socket.c \
		zt_bstream_string.c

DEFS := @DEFS@ -I/usr/include -I. -I$(srcdir) -I../..

pkgincludedir = ${includedir}/zt_bstream
pkginclude_HEADERS = \
		zt_bstream_private.h \
		zt_bstream_file.h \
		zt_bstream_socket.h \
		zt_bstream_string.h

include $(top_srcdir)/Makefile.extra
