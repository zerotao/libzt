AUTOMAKE_OPTIONS=foreign

SUBDIRS = \
	doc     \
	tools	\
	replace \
	libzt   \
	examples \
	tests

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
	aux_files/missing

EXTRA_DIST = \
	aux_files/install-sh \
	aux_files/missing \
	aux_files/config.sub \
	aux_files/config.guess \
	aux_files/ltmain.sh \
	Makefile.extra \
	tools/libzt.pc \
	tools/libzt.pc.in \
	tools/authors.xml \
	tools/uncrustify.cfg \
	tools/ut_info.py \
	tools/zt_mkc

# pkg:
# 	(mkdir @PACKAGE@_fakeinstall; \
# 	 make prefix=@srcdir@/@PACKAGE@_fakeinstall@prefix@ install; \
# 	 mkepmlist -g root -u root @PACKAGE@_fakeinstall > files.list; \
# 	 mkepm -f native @PACKAGE@.list;)

test:
	$(MAKE)
	$(MAKE) -C tests test

coverage::
	mkdir -p coverage
	lcov --compat-libtool --directory $(top_builddir)/libzt --zerocounters
	-$(MAKE) test
	lcov --compat-libtool --directory $(top_builddir)/libzt --capture --output-file $(top_builddir)/coverage/unit_tests_tmp.info
	lcov --compat-libtool --extract $(top_builddir)/coverage/unit_tests_tmp.info "*/libzt*" -o $(top_builddir)/coverage/unit_tests.info
	genhtml -o coverage --num-spaces 2 $(top_builddir)/coverage/unit_tests.info


