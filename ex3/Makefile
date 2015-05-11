CC=g++

LIBHDRS=general.h blockchain.h block.h hash.h
LIBSRC=blockchain.cpp block.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)
LIBLIBS = -lhash -lcrypto -lpthread

INCS=-I.
CFLAGS = -std=c++11 -Wall -g -O0 $(INCS)
LOADLIBES = -L.

LIB = libblockchain.a
TARGETS = $(LIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex2.tar
TARSRCS=$(LIBSRC) $(LIBHDRS) Makefile README

all: $(TARGETS)

$(TARGETS): hash.h $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	
test: test.o
	$(CC) blockchain.o block.o test.o -L. -lhash -lcrypto -lpthread -o test

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(TARGETS) $(OSMLIB) $(OBJ) $(LIBOBJ) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
