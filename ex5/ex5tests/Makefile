CC = g++ -Wall
FLAG = -std=c++11
LIBSRC = srftp.cpp clftp.cpp

all: srftp clftp 

srftp: iosafe.o srftp.o
	$(CC) $(FLAG)  srftp.cpp -lpthread -L./ -o srftp

clftp: 
	$(CC) $(FLAG) clftp.cpp -o clftp

PORT = 3333
SIZE = 2000000000
HNAME = ubuntu
FNAME = ftest
SNAME = stest

create:
	./createfiles

createbig:
	./createfiles 0

server:
	./srftp $(PORT) $(SIZE)

test0:
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1 
	./clftp $(PORT) $(HNAME) $(FNAME)10 $(SNAME)10 
	./clftp $(PORT) $(HNAME) $(FNAME)100 $(SNAME)100 
	./clftp $(PORT) $(HNAME) $(FNAME)1000 $(SNAME)1000 
	./clftp $(PORT) $(HNAME) $(FNAME)10000 $(SNAME)10000
test0diff:
	diff $(FNAME)1 $(SNAME)1 
	diff $(FNAME)10 $(SNAME)10 
	diff $(FNAME)100 $(SNAME)100 
	diff $(FNAME)1000 $(SNAME)1000 
	diff $(FNAME)10000 $(SNAME)10000

test0big:
	./clftp $(PORT) $(HNAME) $(FNAME)big $(SNAME)big
	diff $(FNAME)big $(SNAME)big

test1:
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1A &
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1B &
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1C &
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1D &
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1E 
test1diff:
	diff $(FNAME)1 $(SNAME)1A
	diff $(FNAME)1 $(SNAME)1B
	diff $(FNAME)1 $(SNAME)1C
	diff $(FNAME)1 $(SNAME)1D
	diff $(FNAME)1 $(SNAME)1E 

test11:
	./clftp $(PORT) $(HNAME) $(FNAME)100 $(SNAME)100A &
	./clftp $(PORT) $(HNAME) $(FNAME)100 $(SNAME)100B &
	./clftp $(PORT) $(HNAME) $(FNAME)100 $(SNAME)100C &
	./clftp $(PORT) $(HNAME) $(FNAME)100 $(SNAME)100D &
	./clftp $(PORT) $(HNAME) $(FNAME)100 $(SNAME)100E 
test11diff:
	diff $(FNAME)100 $(SNAME)100A 
	diff $(FNAME)100 $(SNAME)100B
	diff $(FNAME)100 $(SNAME)100C 
	diff $(FNAME)100 $(SNAME)100D 
	diff $(FNAME)100 $(SNAME)100E 

test111:
	./clftp $(PORT) $(HNAME) $(FNAME)10000 $(SNAME)10000A &
	./clftp $(PORT) $(HNAME) $(FNAME)10000 $(SNAME)10000B &
	./clftp $(PORT) $(HNAME) $(FNAME)10000 $(SNAME)10000C &
	./clftp $(PORT) $(HNAME) $(FNAME)10000 $(SNAME)10000D &
	./clftp $(PORT) $(HNAME) $(FNAME)10000 $(SNAME)10000E 

test111diff:
	diff $(FNAME)10000 $(SNAME)10000A 
	diff $(FNAME)10000 $(SNAME)10000B 
	diff $(FNAME)10000 $(SNAME)10000C 
	diff $(FNAME)10000 $(SNAME)10000D 
	diff $(FNAME)10000 $(SNAME)10000E 

test1big:
	./clftp $(PORT) $(HNAME) $(FNAME)big $(SNAME)bigA &
	./clftp $(PORT) $(HNAME) $(FNAME)big $(SNAME)bigB &
	./clftp $(PORT) $(HNAME) $(FNAME)big $(SNAME)bigC &
	./clftp $(PORT) $(HNAME) $(FNAME)big $(SNAME)bigD &
	./clftp $(PORT) $(HNAME) $(FNAME)big $(SNAME)bigE 

test1bigdiff:
	diff $(FNAME)big $(SNAME)bigA 
	diff $(FNAME)big $(SNAME)bigB 
	diff $(FNAME)big $(SNAME)bigC 
	diff $(FNAME)big $(SNAME)bigD 
	diff $(FNAME)big $(SNAME)bigE 

test2:
	./clftp $(PORT) $(HNAME) image.jpg stestimage.jpg

test2diff:
	diff image.jpg stestimage.jpg

test3:
	./clftp $(PORT) $(HNAME) music.mp4 stestmusic.mp4

test3diff:
	diff music.mp4 stestmusic.mp4

testerrors:
	./errors
	diff myoutput testerrorsoutput

server2:
	./srftp $(PORT) 10

testerrors2:
	./clftp $(PORT) $(HNAME) $(FNAME)1 $(SNAME)1 

testerrors3:
	./clftp $(PORT) $(HNAME) ftestshouldntbesent stestshouldntbehere/
	

kill:
	fuser $(PORT)/tcp -k 


TAR=tar
TARFLAGS=-cvf
TARNAME=ex5.tar
TARSRCS=$(LIBSRC) Makefile README

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)

tartest:
	tar -cvf ex5tests.tar Makefile errors myoutput create* image.jpg

cleanfiles:
	$(RM) ftest* 

cleanbinarey:
	$(RM) image.jpg music.mp4

cleantest:
	$(RM) stest*

clean:
	$(RM) *.o srftp clftp  *~ *core


.PHONY: clftp clftp.o srftp srftp.o iosafe.o
