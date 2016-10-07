# ifndef OCR_H_INCLUDED
# define OCR_H_INCLUDED
# include "bitmap.h"
# include "network.h"

float sigmoid(float);

void feedForward(network *, float *);

void backPropagation(network *, float *);

void update(network *, float *);

char ocr(bitmap *);

# endif
