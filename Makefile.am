ACLOCAL_AMFLAGS = -I m4

SUBDIRS = $(PKGCONFIG_SUBDIRS)

# main srcs

include Makefile.am.fragment

lib_LIBRARIES = libutility.a

pkginclude_HEADERS = \
	include/lmdb/lmdb_types.h \
	include/lmdb/lmdb_wrapper.h \
	include/utils/async_worker.h \
	include/utils/bitvector.h \
	include/utils/cleanup.h \
	include/utils/debug_utils.h \
	include/utils/mkdir.h \
	include/utils/non_movable.h \
	include/utils/serialize_endian.h \
	include/utils/threadlocal_cache.h \
	include/utils/time.h

EXTRA_DIST = \
	.gitignore \
	autogen.sh

CLEANFILES = *~ */*~ */*/*~ .gitignore~

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = utility.pc

libutility_a_SOURCES = $(utility_CCS)

AM_CPPFLAGS = $(lmdb_CFLAGS) $(utility_CFLAGS)
LDADD = $(lmdb_LIBS) $(Catch2_LIBS)

#$(EXTRA_SRCS:.cc=.o) : $(XH_FILES)
#$(MAIN_CCS:.cc=.o) : $(XH_FILES)

$(utility_TEST_CCS:.cc=.o) : CXXFLAGS += $(catch2_CFLAGS)

test_SOURCES = $(utility_CCS) $(utility_TEST_CCS) test.cc
test.o : CXXFLAGS += $(Catch2_CFLAGS)

check_PROGRAMS = test
TESTS = test

#clean-local:
#	$(RM)

README: README.md

#.PHONY: doc
#doc:
#	doxygen Doxyfile




