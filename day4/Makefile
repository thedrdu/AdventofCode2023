CC=gcc
CFLAGS=-I.
DEPS = hashtable.h
OBJ = day4.o hashtable.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

day4: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) day4
