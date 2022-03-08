CFLAGS+=  -pedantic -Wall -std=c99 -O3
HW=hw04
ZIP=zip

$(HW): main.c 
	$(CC) $(CFLAGS) main.c -o $(HW)

zip:
	$(ZIP) $(HW)-alg.zip main.c

clean:
	$(RM) -f *.o
	$(RM) -f $(HW)
	$(RM) -f $(HW)-alg.zip

.PHONY: clean zip
