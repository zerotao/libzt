noinst_LTLIBRARIES = libzt_cfg.la

libzt_cfg_la_SOURCES = \
		cfg_private.c \
		cfg_ini.c

DEFS = @DEFS@
CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

pkgincludedir = ${includedir}/libzt/zt_cfg
pkginclude_HEADERS = \
		cfg_private.h \
		cfg_interface.h \
		cfg_ini.h

include $(top_srcdir)/Makefile.extra
