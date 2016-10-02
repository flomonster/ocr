#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "network.h"


// Forward propagation
void feedForward(NETWORK *n, float *inputs)
{
  for (unsigned i = 0; i < n->layers[0]; i++)
    n->out[0][i] = inputs[i];
}

char ocr(BITMAP *img)
{
  // TO DO
  return 'a';
}
