CC = gcc
CFLAGS  = -Wall -Werror -Wextra

default: my_ls

my_ls: main.o my_ls.o
	$(CC) $(CFLAGS) -o my_ls main.o my_ls.o

my_ls.o: my_ls.c
	$(CC) $(CFLAGS) -c my_ls.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o my_ls