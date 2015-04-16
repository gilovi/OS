CC = g++ -Wall -std=c++11
default: all

TEST = test1 test2 test3 test4 test5

all: $(TEST) clean

LIB = ../libuthreads.a

uthreads.o: ../uthreads.h ../uthreads.cpp
	$(CC) -c ../uthreads.cpp

test1.o: ../uthreads.h ./test1.cpp
	$(CC) -c ./test1.cpp
		
	
test1: test1.o uthreads.o
	$(CC) test1.o uthreads.o -o test1 $(LIB)
	./test1 > test1.out 2>&1
	diff test1.out mytest1.out
	
test2.o: ../uthreads.h ./test2.cpp
	$(CC) -c ./test2.cpp
		
	
test2: test2.o uthreads.o
	$(CC) test2.o uthreads.o -o test2 $(LIB)
	./test2 > test2.out 2>&1
	python test2.py

test3.o: ../uthreads.h ./test3.cpp
	$(CC) -c ./test3.cpp
		
	
test3: test3.o uthreads.o
	$(CC) test3.o uthreads.o -o test3 $(LIB)
	./test3 > test3.out 2>&1
	python test3.py
	
test4.o: ../uthreads.h ./test4.cpp
	$(CC) -c ./test4.cpp
		
	
test4: test4.o uthreads.o
	$(CC) test4.o uthreads.o -o test4 $(LIB)
	./test4 > test4.out 2>&1
	diff test4.out mytest4.out
	
test5.o: ../uthreads.h ./test5.cpp
	$(CC) -c ./test5.cpp
		
	
test5: test5.o uthreads.o
	$(CC) test5.o uthreads.o -o test5 $(LIB)
	./test5 > test5.out 2>&1
	diff test5.out mytest5.out
###	.test5
	
lib: test1.o test2.o test3.o test4.o test5.o clean
	$(CC) test1.o ../libuthreads.a -o test1
	$(CC) test2.o ../libuthreads.a -o test2
	$(CC) test3.o ../libuthreads.a -o test3
	$(CC) test4.o ../libuthreads.a -o test4
	$(CC) test5.o ../libuthreads.a -o test5
	./test1 > test1.out 2>&1
	diff test1.out mytest1.out
	./test2 > test2.out 2>&1
	python test2.py
	./test3 > test3.out 2>&1
	python test3.py
	./test4 > test4.out 2>&1
	diff test4.out mytest4.out
	./test > test.out 2>&1
	diff test5.out mytest5.out
	
.PHONY: default test1 test2 test3 test4 test5 lib

clean:
	$(RM) *.o $(TEST)
