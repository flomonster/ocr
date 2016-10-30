# Compilers and options
CC=gcc
CPPFLAGS= -MMD 
CFLAGS= -Wall -Wextra -std=c99 -O3 `pkg-config --cflags --libs gtk+-3.0`
LDFLAGS=
LDLIBS= -lm
 
SRC = main.c ocr.c network.c graphical.c detection.c bitmap.c queue.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
 
all: main
 
main: ${OBJ}
 
-include ${DEP}
 
clean:
	rm -f ${OBJ} ${DEP}
	rm -f main
