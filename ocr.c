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
  return error / nbSample;
}

// Learning
void learn(network *n, float **samples, float **results, unsigned nbSample,
    float speed, float goal)
{
  float iter = 0;
  while (iter < 500 || evaluate(n, samples, results, nbSample) > goal)
  {
    for (unsigned j = 0; j < nbSample; j++)
    {
      feedForward(n, samples[j]);
      backPropagation(n, results[j]);
      update(n, speed);
    }
    iter++;
  }
}

// Main function for recognition
char ocr(bitmap *img, network *n)
{
  float *input = malloc(sizeof(float) * img->width * img->height);
  for (unsigned i = 0; i < img->width * img->height; i++)
    input[i] = img->content[i].r;

  feedForward(n, input);

  int best = 0;
  for (unsigned i = 1; i < n->layers[n->nblayer - 1]; i++)
    if (n->out[n->nblayer - 1][best] < n->out[n->nblayer - 1][i]) 
      best = i;

  free(input);
  return best + 'A';
}

int getCharIndex(char c)
{
  int i = c - 'A';
  if (i < 0 || i > 1)
    errx(1, "Not valid character");
  return i;
}
