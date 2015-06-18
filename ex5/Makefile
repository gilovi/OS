CC=g++
HDRS = general.h clftp.h srftp.h
SRCS = srftp.cpp clftp.cpp
OBJS = $(SRCS:.cpp=.o)

INCS = -I.
CFLAGS = -std=c++11 -Wall -g $(INCS)
LIBS = -lpthread
LOADLIBS = -L.

TAR=tar
TARFLAGS=-cvf
TARNAME=ex5.tar
TARSRCS=$(SRCS) $(HDRS) performance.jpg Makefile README

all : server client

server :
	$(CC) $(CFLAGS) $(LIBS) srftp.cpp -o srftp

client : 
	$(CC) $(CFLAGS) clftp.cpp -o clftp

clean:
	$(RM) $(OBJS) srftp clftp *.o *~ *core

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)
