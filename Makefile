CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main
SRCS = main.c bloom_filter.c hash.c

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
