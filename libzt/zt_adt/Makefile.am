noinst_LTLIBRARIES = libzt_adt.la

libzt_adt_la_SOURCES = 	\
	zt_chash.c	\
	zt_table.c

DEFS = @DEFS@ -I/usr/include -I. -I$(srcdir) -I../..

pkgincludedir = ${includedir}/libzt/zt_adt
pkginclude_HEADERS =

include $(top_srcdir)/Makefile.extra
