#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitmap.h"
#include "network.h"

// Sigmoid function
float sigmoid(float x)
{
  return 1 / (1 + exp(x));
}

// Forward propagation
void feedForward(network *n, float *inputs)
{
  for (unsigned i = 0; i < n->layers[0]; i++)
    n->out[0][i] = inputs[i];

  for (unsigned i = 1; i < n->nblayer; i++)
    for (unsigned j = 0; j < n->layers[i]; j++)
    {
      n->out[i][j] = n->treshold[i-1][j];
      for (unsigned k = 0; k < n->layers[i-1]; k++)
        n->out[i][j] += n->weight[i-1][k][j] * n->out[i-1][k];
      n->out[i][j] = sigmoid(n->out[i][j]);
    }
}

// Main function for recognition
char ocr(BITMAP *img)
{
  // TO DO
  return 'a';
}
