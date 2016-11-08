# ifndef LEARNING_H_INCLUDED
# define LEARNING_H_INCLUDED_H_INCLUDED
# include "bitmap.h"
# include "network.h"
# include "queue.h"

void learning(char *[], size_t);

int createSamples(queue *, float **);

float **createResults(char[], int);

void freeSamples(float **, int);
# endif
