CC = gcc

%.o: %.c
	$(CC) -g -c -o $@ $<

main: btree.o in-out.o main.o
	$(CC) -g -o $@ $^

clean:
	rm *.o
