SUBDIRS = 		\
	zt_adt		\
	zt_cfg 		\
	zt_log 		\
	.		\
	t 
#	cfg 

lib_LTLIBRARIES = libzt.la 

libzt_la_LIBADD = 	\
	../replace/libreplace.la 	\
	zt_log/libzt_log.la		\
	zt_cfg/libzt_cfg.la		\
	zt_adt/libzt_adt.la

#CURRENT[:REVISION[:AGE]]
#before release
#if code change REVISION++
#if interface add, remove or change CURRENT++, REVISION = 0
#if interface add AGE++
#if interface remove AGE = 0
# check eev
libzt_la_LDFLAGS = -version-info 1:0:0 \
		-export-dynamic

libzt_la_SOURCES = 		\
	zt_assert.c 		\
	zt_cfg.c 		\
	zt_daemon.c		\
	zt_except.c 		\
	zt_log.c 		\
	zt_malloc.c 		\
	zt_mem.c		\
	zt_opts.c 		\
	zt_progname.c 		\
	zt_strings.c 		\
	zt_time.c

# 	zt_cfg/cfg_private.c 	
# 	zt_cfg/ini.c 		
# 	zt_log/file.c		
# 	zt_log/log_private.c 	
# 	zt_log/stderr.c 	
# 	zt_log/syslog.c 
#	mem.c 

pkgincludedir = ${includedir}/libzt
pkginclude_HEADERS = \
	zt.h		\
	zt_assert.h 	\
	zt_bytearray.h	\
	zt_cfg.h 	\
	zt_common.h 	\
	zt_daemon.h 	\
	zt_except.h	\
	zt_hash.h	\
	zt_list.h	\
	zt_log.h 	\
	zt_mem.h	\
	zt_opts.h 	\
	zt_progname.h 	\
	zt_strings.h 	\
	zt_table.h	\
	zt_time.h	\
        zt_macros.h

#noinst_HEADERS = \
#	mem.h

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
