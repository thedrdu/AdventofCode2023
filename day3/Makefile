CC=gcc
CFLAGS=-I.
DEPS = regex.h
OBJ = day3.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

day3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJ) day3
