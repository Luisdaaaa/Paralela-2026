CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
TARGET = programa

SRCS = main.c cola.c config.c pasajero.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)
