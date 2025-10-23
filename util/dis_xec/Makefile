## This makefile must be executed with gmake (gnu make).
## On JLab systems, make sure you do "setup cernlib/2005" before you do anything else.
## On other systems, make sure you have appropriate Cernlib stuff installed and
## paths define
RM        = rm -f 
SHELL     = /bin/sh


my_objs = ../../src/xsec_model.o ../../src/shared/stringlib.o

MYOS := $(subst -,,$(shell uname))
#mess $(MYOS)

ifeq ($(MYOS),Linux)
  FFLAGSA=-O -ffixed-line-length-132 -ff2c -fno-automatic -fdefault-real-8
  FFLAGS= $(INCLUDES) $(FFLAGSA)
  FFLAG1=$(FFLAGS) -c
  OTHERLIBS =  -L/usr/lib
  FC  := gfortran
  F77 :=gfortran
endif

%.o: %.f
	$(F77) $(FFLAGS) -c $< -o $@

all: calc_dis_xsec

calc_dis_xsec: $(my_objs) Makefile calc_dis_xsec.o
	$(F77) -o $@ $(FFLAGS) calc_dis_xsec.o $(my_objs) $(OTHERLIBS)

clean:
	rm -f *.o calc_dis_xsec

