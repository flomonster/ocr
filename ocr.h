# ifndef OCR_H_INCLUDED
# define OCR_H_INCLUDED
# include "bitmap.h"
# include "network.h"

void feedForward(network *, float *);

void backPropagation(network *, float *);

void update(network *, float);

float evaluate(network *, float **, float **, unsigned);

char ocr(bitmap *, network *);

int getCharIndex(char);

char getCharFromIndex(int);

# endif
