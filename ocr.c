# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <err.h>
# include "bitmap.h"
# include "network.h"

// Sigmoid function
float sigmoid(float x)
{
  return 1 / (1 + exp(-x));
}

// Forward propagation
void feedForward(network *n, float *inputs)
{
  for (unsigned i = 0; i < n->layers[0]; i++)
    n->out[0][i] = inputs[i];

  for (unsigned i = 1; i < n->nblayer; i++)
    for (unsigned j = 0; j < n->layers[i]; j++)
    {
      n->out[i][j] = n->threshold[i - 1][j];
      for (unsigned k = 0; k < n->layers[i - 1]; k++)
        n->out[i][j] += n->weight[i - 1][k][j] * n->out[i - 1][k];
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
    n->delta[last - 1][i] = out * (1 - out) * (out - outputs[i]);
  }

  for (int i = last - 1; i > 0; i--)
    for (unsigned j = 0; j < n->layers[i]; j++)
    {
      n->delta[i - 1][j] = 0;
      for (unsigned k = 0; k < n->layers[i + 1]; k++)
        n->delta[i - 1][j] += n->weight[i][j][k] * n->delta[i][k];
      n->delta[i - 1][j] *= n->out[i][j] * (1 - n->out[i][j]); 
    }
}

// Update weight and threshold
void update(network *n, float speed)
{
  for (unsigned i = 0; i < n->nblayer - 1; i++)
  {
    for (unsigned j = 0; j < n->layers[i]; j++)
      for (unsigned k = 0; k < n->layers[i + 1]; k++)
        n->weight[i][j][k] -= speed * n->out[i][j] * n->delta[i][k];

    for (unsigned j = 0; j < n->layers[i + 1]; j++)
      n->threshold[i][j] -= speed * n->delta[i][j];
  }
}

// Ealuates a neural network
float evaluate(network *n, float **samples, float **results, unsigned nbSample)
{
  float error = 0;
  unsigned last = n->nblayer - 1;
  for (unsigned i = 0; i < nbSample; i++)
  {
    feedForward(n, samples[i]);
    for (unsigned j = 0; j < n->layers[last]; j++)
      error += fabsf(n->out[last][j] - results[i][j]);
  }
  return error;
}

// Learning
void learn(network *n, float **samples, float **results, unsigned nbSample,
    float speed, float goal)
{
  while (evaluate(n, samples, results, nbSample) > goal)
  {
    for (unsigned j = 0; j < nbSample; j++)
    {
      feedForward(n, samples[j]);
      backPropagation(n, results[j]);
      update(n, speed);
    }
  }
}

/*
// Main function for recognition
char ocr(bitmap *img)
{
  // TO DO
  return 'a';
}
*/
