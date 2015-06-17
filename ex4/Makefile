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
TARSRCS= $(HDRS) $(SRCS) Makefile README 

CachingFileSystem: cache.o 
	g++ -Wall -std=c++11 cache.o CachingFileSystem.cpp `pkg-config fuse --cflags --libs` -o CachingFileSystem

unmount:
	fusermount -u /tmp/mount

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(OBJS) $(LIBOBJ) CachingFileSystem *.o *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
