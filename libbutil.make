# vim: set ft=make:
ifndef LIBBUTIL_MAKE_INCLUDED
LIBBUTIL_MAKE_INCLUDED:=1

srcdir?=
include $(srcdir)make/boost.make

.PHONY:butil
.PHONY:check-butil
.PHONY:examples-butil
.PHONY:info-butil

#BUTIL_MODS:=
#BUTIL_PKGS:=
#BUTIL_DEFINITIONS:=
#BUTIL_INCLUDE_DIRS:=
#BUTIL_LIBRARY_DIRS:=

BUTIL_DEFINITIONS:=$(BOOST_DEFINITIONS)
BUTIL_INCLUDE_DIRS:=$(srcdir)butil/include $(BOOST_INCLUDE_DIRS)

##################################################
# -- Library --
##################################################

SRCS:=$(wildcard $(srcdir)butil/src/*.cpp)

BUTIL:=$(call em_link_lib,butil,\
	$(call em_compile,$(SRCS),obj/butil$(FLGEXT)))

obj/butil$(FLGEXT):DEFINITIONS:=$(BUTIL_DEFINITIONS)
obj/butil$(FLGEXT):INCLUDE_DIRS:=$(BUTIL_INCLUDE_DIRS)
obj/butil$(FLGEXT):FLAGS:=-std=c++14

butil:$(BUTIL)

##################################################
# -- Unit tests --
##################################################

# Test program

SRCS:=$(wildcard $(srcdir)butil/check/*.cpp)
SRCS+=$(wildcard $(srcdir)butil/check/integer/*.cpp)
SRCS+=$(wildcard $(srcdir)butil/check/float/*.cpp)

BUTIL_CHECK:=$(call em_link_bin,butil/check,\
	$(call em_compile,$(SRCS),obj/butil_check$(FLGEXT)) $(BUTIL))

obj/butil_check$(FLGEXT):DEFINITIONS:=$(BUTIL_DEFINITIONS) BUTIL_FAIL_FAST_THROWS BUTIL_FAIL_FAST_PRINTS=0
obj/butil_check$(FLGEXT):INCLUDE_DIRS:=$(BUTIL_INCLUDE_DIRS)
obj/butil_check$(FLGEXT):FLAGS:=-std=c++14 -O3

# Try to compile each header separately (as c++)
# - detects missing includes

BUTIL_ALL_INCLUDE_DIRS:=$(srcdir)butil/include
BUTIL_ALL_INCLUDE_DIRS+=$(srcdir)butil/include/butil
BUTIL_ALL_INCLUDE_DIRS+=$(srcdir)butil/include/butil/fp
BUTIL_ALL_INCLUDE_DIRS+=$(srcdir)butil/include/butil/integer
BUTIL_ALL_INCLUDE_DIRS+=$(srcdir)butil/include/butil/integer/detail
BUTIL_ALL_INCLUDE_DIRS+=$(srcdir)butil/include/butil/io
BUTIL_ALL_INCLUDE_DIRS+=$(srcdir)butil/include/butil/str

#BUTIL_CHECK_HDRS:=$(foreach d,$(BUTIL_ALL_INCLUDE_DIRS),$(call em_compile,$(wildcard $(d)/*.h),obj/butil_headers$(FLGEXT)))
#$(BUTIL_CHECK_HDRS):LANG:=CXX

obj/butil_headers$(FLGEXT):DEFINITIONS:=$(BUTIL_DEFINITIONS)
obj/butil_headers$(FLGEXT):INCLUDE_DIRS:=$(BUTIL_INCLUDE_DIRS)
obj/butil_headers$(FLGEXT):FLAGS:=-std=c++14 -x c++

check-butil:$(BUTIL_CHECK) $(BUTIL_CHECK_HDRS)
	$<

##################################################
# -- Examples --
##################################################

butil-example = $(call em_link_bin,butil/$(1),\
	$(call em_compile,$(srcdir)butil/examples/$(1).cpp,obj/butil$(FLGEXT)) $(BUTIL))

examples-butil:$(call butil-example,io) $(call butil-example,subint) $(call butil-example,subtype)

endif # LIBBUTIL_MAKE_INCLUDED
