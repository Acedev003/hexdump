CC     = gcc
CFLAGS = -Wall -Wextra -O3

hexdump: src/main.c
	$(CC) $(CFLAGS) -o $@ src/main.c

clean:
	rm -f *.exe

win-clean:
	del	*.exe