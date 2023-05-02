CC = gcc
CFLAGS = -Wall -std=c99 -Iinclude -pg -g
LINKS = -lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lm

SOURCES = $(wildcard src/**/*.c)

all: boat

boat: $(SOURCES)
	$(CC) $(CFLAGS) -o boat $(SOURCES) $(LINKS)

run: boat
	./boat

profile: boat
	./boat
	mkdir -p prof
	gprof boat > prof/time.txt
	gprof2dot prof/time.txt > prof/time.dot
	xdot prof/time.dot

clean:
	rm -f boat
	rm -f gmon.out
	rm -rf prof