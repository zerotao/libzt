noinst_LTLIBRARIES = libzt_adt.la

libzt_adt_la_SOURCES = 	\
	zt_hash.c			\
	zt_table.c			\
	zt_array.c			\
	zt_bstream.c		\
	zt_set.c			\
	zt_tree.c


#	zt_bytearray.c

DEFS = @DEFS@
AM_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

pkgincludedir = ${includedir}/libzt/adt
pkginclude_HEADERS =	\
	zt_array.h			\
	zt_list.h			\
	zt_stack.h			\
	zt_queue.h			\
	zt_hash.h			\
	zt_table.h			\
	zt_set.h			\
	zt_bstream.h		\
	zt_tree.h

include $(top_srcdir)/Makefile.extra
