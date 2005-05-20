noinst_LTLIBRARIES = libzt_adt.la

libzt_adt_la_SOURCES = 	\
	zt_hash.c	\
	zt_table.c	\
	zt_array.c	\
	zt_bstream.c


#	zt_bytearray.c

DEFS = @DEFS@ -I/usr/include -I. -I$(srcdir) -I../..

pkgincludedir = ${includedir}/libzt/adt
pkginclude_HEADERS =	\
	zt_array.h	\
	zt_list.h	\
	zt_hash.h	\
	zt_table.h	\
	zt_bstream.h

include $(top_srcdir)/Makefile.extra
