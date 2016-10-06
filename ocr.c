# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "bitmap.h"
# include "network.h"

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
      n->out[i][j] = n->threshold[i-1][j];
      for (unsigned k = 0; k < n->layers[i-1]; k++)
        n->out[i][j] += n->weight[i-1][k][j] * n->out[i-1][k];
      n->out[i][j] = sigmoid(n->out[i][j]);
    }
}

// Backward propagation
void backPropagation(network *n, float *outputs)
{
  unsigned last = n->nblayer - 1;
  for (unsigned i = 0; i < n->layers[last]; i++)
  {
    float out = n->out[last][i];
    n->delta[last][i] = out * (1 - out) * (out - outputs[i]);
  }

  for (int i = last - 1; i > 0; i--)
    for (unsigned j = 0; j < n->layers[i]; j++)
    {
      n->delta[i-1][j] = 0;
      for (unsigned k = 0; k < n->layers[i]; k++)
        n->delta[i-1][j] += n->weight[i][j][k] * n->delta[i][k];
      n->delta[i-1][j] *= n->out[i][j] * (1 - n->out[i][j]); 
    }
}

// Main function for recognition
char ocr(bitmap *img)
{
  // TO DO
  return 'a';
}
