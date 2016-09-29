# Compilers and options
CC=gcc
CPPFLAGS= -MMD
CFLAGS= -Wall -Wextra -std=c99 -pedantic -O2 'pkg-config --cflags gtk+-3.0'
LDFLAGS=
LDLIBS='pkg-config --libs gtk+-3.0'
 
SRC = main.c graphical.c network.c treatment.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
 
all: main
 
main: ${OBJ}
 
-include ${DEP}
 
clean:
	rm -f ${OBJ} ${DEP}
	rm -f main
