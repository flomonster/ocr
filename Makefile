# Compilers and options
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3 $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS=
LDLIBS= $(shell pkg-config --libs gtk+-3.0) -lm

SRC = main.c ocr.c network.c detection.c bitmap.c queue.c learning.c graphical.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: main

main: ${OBJ}

-include ${DEP}

clean:
	rm -f ${OBJ} ${DEP}
	rm -f main
