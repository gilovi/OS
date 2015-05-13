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

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(TARGETS) $(OSMLIB) $(OBJ) $(LIBOBJ) test *.o *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
