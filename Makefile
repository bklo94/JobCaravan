CC=gcc
CFLAGS=-lcurl
DEPS = requests.h API.h
OBJ = requests.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

request: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
