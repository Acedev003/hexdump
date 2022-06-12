CC     = gcc
CFLAGS = -Wall -Wextra -O3

hexdump: src/main.c
	$(CC) $(CFLAGS) -o $@ src/main.c

clean:
	rm -f *.o bin/hexdump

win-clean:
	del *.o
	del	bin\hexdump.exe