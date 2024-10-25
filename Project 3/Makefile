CC=gcc
CFLAGS=-Wall -pedantic -g
OBJECTS=encode.o decode.o

all: encode decode
decode: decode.o 
	$(CC) $(CFLAGS) -o decode decode.o	
encode: encode.o 
	$(CC) $(CFLAGS) -o encode encode.o	
encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c
decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c
.PHONY: clean
clean:
	rm *.o encode decode

submission: 
	tar czvf prog3.tgz ./