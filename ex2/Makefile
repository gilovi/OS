CC=g++
RANLIB=ranlib
TEST=test

LIBSRC=uthreads.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)

INCS=-I.
CFLAGS = -std=gnu++0x -Wall -g -O0 $(INCS)
LOADLIBES = -L./

LIB = libuthreads.a
TARGETS = $(LIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex1.tar
TARSRCS=$(LIBSRC) Makefile README

TESTSRC=uthreads.cpp priorityList.cpp thread.cpp main.cpp
TESTOBJ=$(TESTSRC:.cpp=.o)

all: $(TARGETS)

$(TARGETS):	$(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

test: $(TESTOBJ)
	$(CC) $(CFLAGS) $(TESTSRC) -o $@

clean:
	$(RM) $(TARGETS) $(TESTOBJ) $(OSMLIB) $(OBJ) $(LIBOBJ) $(TARNAME) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)