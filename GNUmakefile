CC = gcc
CFLAGS = -Wall -g

build: main.o hash.o hashtable.o
	$(CC) $^ -o tema1

main.o: main.c
	$(CC) $(CFLAGS) -c $<

hash.o: hash.c
	$(CC) $(CFLAGS) -c $<

hashtable.o: hashtable.c
	$(CC) $(CFLAGS) -c $<

clean: tema1
	rm -f *.o tema1
