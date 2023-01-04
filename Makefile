CC=g++
CFLAGS=-I.
DEPS = graphutil.h
OBJ = graphutil.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

algos: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)