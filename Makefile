CC = gcc

all: libsjk

libsjk: libsjk.o
	$(CC) -shared -o libsjk.so.1.0 libsjk.o
libsjk.o: libsjk.c
	$(CC) -fPIC -Wall -g -c libsjk.c
install:
	cp libsjk.so.1.0 /usr/local/lib/
	ln -sf /usr/local/lib/libsjk.so.1.0 /usr/local/lib/libsjk.so.1
	ln -sf /usr/local/lib/libsjk.so.1 /usr/local/lib/libsjk.so
	cp libsjk.h /usr/local/include/
clean:
	-rm libsjk.so* libsjk.o
