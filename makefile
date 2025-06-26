CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lSDL2

all: engine

engine: main.o helpers.o services.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o: src/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

helpers.o: src/helpers.c
	$(CC) $(CFLAGS) -c -o $@ $<

services.o: src/services.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	rm -rf *.o engine
