CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -pg -g
LINKS = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lm

SOURCES = $(wildcard src/**/*.c)

all: boat

boat: $(SOURCES)
	$(CC) $(CFLAGS) -o boat $(SOURCES) $(LINKS)

run: boat
	./boat

clean:
	rm -f boat
	rm -f gmon.out