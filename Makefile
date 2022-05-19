CC=gcc

hexdump: main.o hexdump.o
	$(CC) main.o hexdump.o -o bin/hexdump

hexdump.o: src/hexdump.c
	$(CC) -c src/hexdump.c

main.o: src/main.c
	$(CC) -c src/main.c


debug:  dmain.o dhexdump.o
	$(CC) -g -Wall main.o hexdump.o -o bin/hexdump

dhexdump.o: src/hexdump.c
	$(CC) -g -Wall -c src/hexdump.c

dmain.o: src/main.c
	$(CC) -g -Wall -c src/main.c

clean:
	rm -f *.o bin/hexdump

win-clean:
	del *.o
	del	bin\hexdump.exe