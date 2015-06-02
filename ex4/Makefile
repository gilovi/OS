CC=g++

HDRS = cache.h
SRCS = CachingFileSystem.cpp cache.cpp
OBJS = $(SRC:.cpp=.o)
ARGS = /tmp/root /tmp/mount 3 6 &

INCS=-I.
CFLAGS = -std=c++11 -Wall -g `pkg-config fuse --cflags --libs` -O0 $(INCS)
LOADLIBES = -L.

TAR=tar
TARFLAGS=-cvf
TARNAME=ex4.tar
TARSRCS=$(LIBSRC) $(LIBHDRS) Makefile README

all: $(TARGETS)

$(TARGETS): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

caching: cache.o CachingFileSystem.cpp
	g++ -Wall -std=c++11 cache.o CachingFileSystem.cpp `pkg-config fuse --cflags --libs` -o caching

run: caching
	caching $(ARGS)

copy:
	mkdir /tmp/mount
	mkdir /tmp/root
	cp /cs/stud/moshemandel/OS/ex4/root/* /tmp/root

unmount:
	fusermount -u /tmp/mount

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(TARGETS) $(OSMLIB) $(OBJ) $(LIBOBJ) caching *.o *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
