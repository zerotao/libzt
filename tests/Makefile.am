DEFS = @DEFS@
AM_CPPFLAGS = -I$(top_builddir) -I$(top_builddir)/libzt -I$(top_srcdir) -I$(top_srcdir)/libzt -I$(srcdir)

unit_test_LDADD = \
	$(top_builddir)/libzt/.libs/libzt.a 	 \
	$(top_builddir)/libzt/.libs/libzt_unit.a \
	$(top_builddir)/libzt/.libs/libzt_coro.a \
	$(top_builddir)/libzt/.libs/libzt_gc.a

noinst_PROGRAMS = 	\
	unit_test

unit_test_SOURCES = 	\
	assert_test.c		\
	unit_test.c			\
	array_test.c		\
	table_test.c		\
	bstream_test.c		\
	cfg_test.c			\
	cstr_test.c			\
	event_test.c		\
	except_test.c		\
	format_test.c		\
	gc_test.c			\
	int_test.c			\
	list_test.c			\
	llist_test.c		\
	log_test.c			\
	macros_test.c		\
	opts_test.c			\
	progname_test.c 	\
	set_test.c			\
	stack_test.c		\
	time_test.c			\
	tree_test.c			\
	coro_test.c			\
	cothread_test.c		\
	sha1_test.c			\
	uuid_test.c

#	msg_queue_test.c

EXTRA_DIST = 		\
	cfg_test.ini

noinst_DATA = \
	cfg_test.ini

DUMMY_TEST = $(addprefix $(srcdir)/,$(noinst_DATA))

test.out:
	test

test: unit_test
	@echo Beginning Test Run
	@DTD=`cd $(srcdir) && pwd`;	\
	DTBD=`cd $(top_builddir)/tests && pwd`; \
	if [ $$DTD != $$DTBD ]; then \
		echo $$DTD;		\
		echo $$DTBD;	\
		cp -np $(DUMMY_TEST) $(top_builddir)/tests; \
	fi
	@$(RM) -f diff.out assert.out core
	@python $(top_srcdir)/tools/ut_info.py ./unit_test;

include $(top_srcdir)/Makefile.extra

CLEANFILES += \
	diff.out \
	assert.out \
	log.err

