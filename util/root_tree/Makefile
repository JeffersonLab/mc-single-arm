## This makefile must be executed with gmake (gnu make).

## This tells make not to delete these target files on error/interrupt (see man page)
.PRECIOUS: *.o 
RM        = rm -f 
SHELL     = /bin/sh

OBJ	= make_root_tree.o
 
my_objs	=  $(OBJ)

my_deps = $(my_objs:.o=.d)

MYOS := $(subst -,,$(shell uname))

ifeq ($(ROOTSYS),)
  ifneq ($(shell command -v root-config 2>/dev/null),)
    ROOT_CONFIG := $(shell command -v root-config)
  else ifneq ($(wildcard /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/root/6.30.04-gcc11.4.0/bin/root-config),)
    ROOT_CONFIG := /cvmfs/oasis.opensciencegrid.org/jlab/scicomp/sw/el9/root/6.30.04-gcc11.4.0/bin/root-config
  endif
else
  ROOT_CONFIG := $(ROOTSYS)/bin/root-config
endif

ifeq ($(ROOT_CONFIG),)
  $(error Unable to find root-config. Set ROOTSYS or add root-config to PATH)
endif

ROOTSYS ?= $(shell $(ROOT_CONFIG) --prefix)
PCRE_LIBS := $(shell pkg-config --libs libpcre 2>/dev/null || echo -lpcre)

#For use with gfortran compiler
# -fno-automatic - all program storage treated as static
ifeq ($(MYOS),Linux)
  FFLAGSA=-
  INCLUDES=
  FFLAGS = -O3 -fno-automatic
  OTHERLIBS = 
  FC  := gfortran
  F77 := gfortran
endif

CXX=g++
CXXFLAGS0 = -O3 -std=c++17
CXXFLAGS=$(CXXFLAGS0) $(DROOT)
ROOTDIR=$(ROOTSYS)
DMYROOT= -DMYROOT
ROOTLIBS     := $(shell $(ROOT_CONFIG) --prefix=$(ROOTDIR) --libs) $(PCRE_LIBS)
ROOTINCLUDE  := -I $(shell $(ROOT_CONFIG) --prefix=$(ROOTDIR) --incdir)

none: make_root_tree

all: make_root_tree

make_root_tree: $(my_objs) froot.co
	$(F77) $(LDFLAGS) -o make_root_tree $(my_objs) $(OTHERLIBS) froot.co $(ROOTLIBS) -lImt -lvdt -ltbb $(OTHERLIBS) $(CERNLIBS) -L/usr/lib64 -lstdc++

%.o: %.f
	$(FC) -c $(FFLAGS) $<

%.co: %.c
	$(CXX) -c $(CXXFLAGS) $(DMYROOT) $(ROOTINCLUDE) -o $@ $<

clean:
	$(RM) *.o *.co make_root_tree
