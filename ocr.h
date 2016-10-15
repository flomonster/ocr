# ifndef OCR_H_INCLUDED
# define OCR_H_INCLUDED
# include "bitmap.h"
# include "network.h"

float sigmoid(float);

void feedForward(network *, float *);

void backPropagation(network *, float *);

void update(network *, float);

float evaluate(network *, float **, float **, unsigned);

char ocr(bitmap *);

void learn(network *, float **, float **, unsigned, float, float);

# endif
