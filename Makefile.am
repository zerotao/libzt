SUBDIRS = \
	doc \
	replace \
	libzt 

docs:
	(cd doc; make)

MAINTAINERCLEANFILES = \
	configure \
	aclocal.m4 \
	stamp-h.in \
	config.h.in \
	Makefile.in \
	aux/config.guess \
	aux/config.sub \
	aux/ltmain.sh \
	aux/install-sh \
	aux/mkinstalldirs \
	aux/missing \
	files.list

EXTRA_DIST = \
	aux/install-sh \
	aux/mkinstalldirs \
	aux/missing \
	aux/config.sub \
	aux/config.guess \
	aux/ltmain.sh \
	Makefile.extra \
	tools/build-rpm \
	tools/project.spec \
	tools/project.spec.in \
	tools/libzt.pc \
	tools/libzt.pc.in \
	libzt.list


pkg:
	(mkdir @PACKAGE@_fakeinstall; \
	 make prefix=@srcdir@/@PACKAGE@_fakeinstall@prefix@ install; \
	 mkepmlist -g root -u root @PACKAGE@_fakeinstall > files.list; \
	 mkepm -f native @PACKAGE@.list;)

pkgconfigdir = $(libdir)/pkgconfig/
pkgconfig_DATA = \
	tools/libzt.pc