CC=gcc
CFLAGS=-I. -g
DEPS = hashtable.h
OBJ = day3.o hashtable.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

day3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) day3
