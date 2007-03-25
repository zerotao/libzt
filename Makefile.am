AUTOMAKE_OPTIONS=foreign

SUBDIRS = \
	doc     \
	tools	\
	replace \
	libzt   \
	examples

MAINTAINERCLEANFILES = \
	configure \
	aclocal.m4 \
	stamp-h.in \
	config.h.in \
	Makefile.in \
	aux_files/config.guess \
	aux_files/config.sub \
	aux_files/ltmain.sh \
	aux_files/install-sh \
	aux_files/missing \
	files.list

EXTRA_DIST = \
	aux_files/install-sh \
	aux_files/missing \
	aux_files/config.sub \
	aux_files/config.guess \
	aux_files/ltmain.sh \
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
