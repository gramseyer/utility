ACLOCAL_AMFLAGS = -I m4

SUBDIRS = $(PKGCONFIG_SUBDIRS)

# main srcs

lib_LIBRARIES = libutility.a

pkginclude_HEADERS = \
	include/utils/async_worker.h \
	include/utils/bitvector.h \
	include/utils/cleanup.h \
	include/utils/debug_utils.h \
	include/utils/mkdir.h

EXTRA_DIST = \
	.gitignore \
	autogen.sh

CLEANFILES = *~ */*~ */*/*~ .gitignore~

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = utility.pc

UTILS_SRCS = \
	utils/cleanup.cc \
	utils/mkdir.cc

libutility_a_SOURCES = $(UTILS_SRCS)

SRCS = \
	$(UTILS_SRCS)

$(SRCS:.cc=.o): CXXFLAGS += -I./include
#$(MAIN_CCS:.cc=.o) : CXXFLAGS += -I./include

#AM_CPPFLAGS = $(xdrpp_CFLAGS) $(libsodium_CFLAGS) $(lmdb_CFLAGS)
#LDADD = $(xdrpp_LIBS) $(libsodium_LIBS)  $(lmdb_LIBS)

#$(EXTRA_SRCS:.cc=.o) : $(XH_FILES)
#$(MAIN_CCS:.cc=.o) : $(XH_FILES)
#$(TEST_SRCS:.cc=.o) : CXXFLAGS += $(Catch2_CFLAGS)

#main/test.o : CXXFLAGS += $(Catch2_CFLAGS)

#test_SOURCES = $(SRCS) $(EXTRA_SRCS) main/test.cc

#test_LDADD = $(LDADD) $(Catch2_LIBS)

#check_PROGRAMS = test
#TESTS = test

#clean-local:
#	$(RM)

README: README.md

#.PHONY: doc
#doc:
#	doxygen Doxyfile




