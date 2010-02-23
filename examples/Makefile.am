LDADD = $(top_builddir)/libzt/libzt.la 

DEFS = @DEFS@
AM_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

EXAMPLE_APPS = 		\
		events		\
		mem_pools	\
		mem_timing	\
		types

all: $(EXAMPLE_APPS)

#	bytearray_test
noinst_PROGRAMS = $(EXAMPLE_APPS)

events_SOURCES = \
	events.c

mem_pools_SOURCES = \
	mem_pools.c

mem_timing_SOURCES = \
	mem_timing.c

types_SOURCES = \
	types.c


include $(top_srcdir)/Makefile.extra


