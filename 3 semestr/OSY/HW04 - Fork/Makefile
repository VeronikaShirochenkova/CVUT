CC=gcc
CFLAGS=-g -Wall
HW=forkpipe

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(HW) nsd

$(HW): forkpipe.o
	gcc -o $(HW) forkpipe.c $(CFLAGS) $(EXTRA_CFLAGS)

nsd: nsd_main.o nd.o nsd.o nd.h nsd.h
	gcc -o nsd nsd_main.o nsd.o nd.o $(CFLAGS) $(EXTRA_CFLAGS)