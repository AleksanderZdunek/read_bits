TARGET = main
OBJ = 	main.o read_bits.o

CC = gcc
CFLAGS = -std=c11 -O3 -Wall -Wpedantic

all: $(TARGET)

read_bits.o: read_bits.h
main.o: read_bits.h

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
