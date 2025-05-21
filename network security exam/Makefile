CC=gcc
CFLAGS=-Iinclude

all: caesar

caesar: src/caesar.c
	$(CC) $(CFLAGS) src/caesar.c -o build/caesar

clean:
	rm -f build/caesar
