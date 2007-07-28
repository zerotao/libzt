noinst_LTLIBRARIES = libzt_log.la

libzt_log_la_SOURCES = \
		log_private.c \
		log_stderr.c \
		log_syslog.c \
		log_file.c

DEFS = @DEFS@ 

AM_CPPFLAGS = -I${top_builddir} -I${top_builddir}/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

pkgincludedir = ${includedir}/libzt/zt_log
pkginclude_HEADERS = \
		log_private.h \
		log_stderr.h \
		log_syslog.h \
		log_file.h \
		log_interface.h

include $(top_srcdir)/Makefile.extra
