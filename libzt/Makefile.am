SUBDIRS = 		\
	adt			\
	zt_cfg 		\
	zt_log 		\
  zt_threads \
	.


lib_LTLIBRARIES = libzt.la libzt_unit.la libzt_gc.la

libzt_la_LIBADD = 					\
		../replace/libreplace.la 	\
		adt/libzt_adt.la			\
		zt_log/libzt_log.la			\
		zt_cfg/libzt_cfg.la

if PTHREADS
libzt_la_LIBADD += zt_threads/libzt_threads_pthreads.la
endif 

#CURRENT[:REVISION[:AGE]]
#before release
#if code change REVISION++
#if interface add, remove or change CURRENT++, REVISION = 0
#if interface add AGE++
#if interface remove AGE = 0
# check eev
libzt_la_LDFLAGS = -version-info 2:0:0  -export-dynamic
libzt_gc_la_LDFLAGS = -version-info 1:0:0  -export-dynamic
libzt_unit_la_LDFLAGS = -version-info 1:0:0 -export-dynamic

DEFS = @DEFS@
AM_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

libzt_unit_la_SOURCES = 	\
		zt_unit.c

libzt_gc_la_SOURCES = 	\
		zt_gc.c

libzt_la_SOURCES = 		\
		zt_assert.c 	\
		zt_cfg.c 		\
		zt_daemon.c		\
		zt_except.c 	\
		zt_format.c		\
		zt_log.c 		\
		zt_malloc.c 	\
		zt_mem.c		\
		zt_opts.c 		\
		zt_progname.c 	\
		zt_cstr.c 		\
		zt_time.c		\
		zt_gc.c			\
		zt_sha1.c		\
		zt_uuid.c		\
		zt_exceptions.c \
		zt_ipv4_tbl.c \
		zt_ez_mempool.c \
    zt_threads.c \
		zt_atexit.c

pkgincludedir = ${includedir}/libzt
pkginclude_HEADERS = 	\
		zt.h			\
		zt_internal.h	\
		zt_unit.h		\
		zt_assert.h		\
		zt_cfg.h		\
		zt_common.h		\
		zt_daemon.h		\
		zt_except.h		\
		zt_format.h		\
		zt_log.h		\
		zt_mem.h		\
		zt_opts.h		\
		zt_progname.h	\
		zt_cstr.h		\
		zt_time.h		\
		zt_macros.h		\
		zt_gc.h			\
		zt_int.h		\
		zt_sha1.h		\
		zt_uuid.h		\
		zt_exceptions.h \
		zt_ez_mempool.h \
		zt_ipv4_tbl.h	\
    zt_threads.h \
		zt_atexit.h


# The following is required in the Makefile.am for common.h
# Regenerate common.h with config.status whenever common-h.in changes.
#common.h: stamp-common
#	@:
zt_common.h: zt_common_h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS= CONFIG_OTHER=$(subdir)/$@ $(SHELL) ./config.status


# Don't distribute common.h, since it is build host dependent!
dist-hook:
	rm -f $(distdir)/zt_common.h

EXTRA_DIST = zt_common_h.in

include $(top_srcdir)/Makefile.extra
MAINTAINERCLEANFILES += zt_common.h stamp-common
DISTCLEANFILES += zt_common.h
