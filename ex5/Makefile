
HDRS = general.h clftp.h srftp.h
SRCS = srftp.cpp clftp.cpp
OBJS = $(SRCS:.cpp=.o)

INCS = -I.
CFLAGS = -std=c++11 -Wall -g -O0 $(INCS)
LIBS = -lpthread
LOADLIBS = -L.

TAR=tar
TARFLAGS=-cvf
TARNAME=ex5.tar
TARSRCS=$(SRCS) $(HDRS) Makefile README

all : server

server :
	$(CC) $(CFLAGS) srftp.cpp -o srftp

client : 
	$(CC) $(CFLAGS) clftp.cpp -o clftp

clean:
	$(RM) $(OBJS) *.o *~ *core

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
