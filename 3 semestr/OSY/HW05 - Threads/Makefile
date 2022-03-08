HW=prod-cons
CC=gcc
CFLAGS+= -Wall -g -O2 -pthread
EXTRA_CFLAGS+= -fsanitize=address -fno-omit-frame-pointer

$(HW): prod-cons.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) prod-cons.c -o $(HW)
