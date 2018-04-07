CC=gcc
CFLAGS=-lcurl -lm
DEPS = requests.h API.h cJSON.h
OBJ = cJSON.o requests.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

request: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
