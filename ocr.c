# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <err.h>
# include "bitmap.h"
# include "network.h"
# include "ocr.h"

char characters[95] = 
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQR"
"STUVWXYZ0123456789.,?!_-&\"#{([|\'\\@)]°}+=$%*;/:<>";

/**
 * \brief Sigmoid function
 *
 * \param x a real number
 */
__attribute__((const)) float sigmoid(float x)
{
  return 1 / (1 + exp(-x));
}

/**
 * \brief Softmax function
 *
 * \param k the vector dimesion
 * \param z the vector
 */
void softmax(unsigned k, float *z)
{
  float sum = 0;
  for (unsigned i = 0; i < k; i++)
    sum += exp(z[i]); 
  sum = sum == 0 ? 1 : sum;
  for (unsigned i = 0; i < k; i++)
    z[i] = exp(z[i]) / sum;
}

/**
 * \brief the activation function of a neural network
 *
 * \param n the neural network
 * \param inputs values for the input layer
 */
void feedForward(network *n, float *inputs)
{
  for (unsigned i = 0; i < n->layers[0]; i++)
    n->out[0][i] = inputs[i];

  for (unsigned i = 1; i < n->nblayer - 1; i++)
    for (unsigned j = 0; j < n->layers[i]; j++)
    {
      n->out[i][j] = n->threshold[i - 1][j];
      for (unsigned k = 0; k < n->layers[i - 1]; k++)
        n->out[i][j] += n->weight[i - 1][k][j] * n->out[i - 1][k];
      n->out[i][j] = sigmoid(n->out[i][j]);
    }
  unsigned last = n->nblayer - 1;
  for (unsigned j = 0; j < n->layers[last]; j++)
  {
    n->out[last][j] = n->threshold[last - 1][j];
    for (unsigned k = 0; k < n->layers[last - 1]; k++)
      n->out[last][j] += n->weight[last - 1][k][j] * n->out[last - 1][k];
  }
  softmax(n->layers[last], n->out[last]);
}

/**
 * \brief calculates the gradient of the loss function
 *
 * \param n the neural network
 * \param outputs expected values
 */
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

/**
 * \brief calculates a new value for weight and threshold
 *
 * \param n the neural network
 * \param speed the learning rate
 */
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

/**
 * \brief Evaluate a neural network with samples
 *
 * \param n the neural network
 * \param samples the inputs of samples
 * \param results expected outputs of samples
 * \param nbSample the number of samples
 */
float evaluate(network *n, float **samples, float **results, size_t nbSample)
{
  float error = 0;
  unsigned last = n->nblayer - 1;
  for (size_t i = 0; i < nbSample; i++)
  {
    float dist = 0;
    feedForward(n, samples[i]);
    for (unsigned j = 0; j < n->layers[last]; j++)
      dist += pow(n->out[last][j] - results[i][j], 2);
    error += sqrt(dist);
  }
  return error / (nbSample * 2);
}

/**
 * \brief optical character recognition
 *
 * \param img a picture of a character (16x16)
 * \param n the neural network
 */
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
  return getCharFromIndex(best);
}

/**
 * \brief give the index corresponding with the character c
 *
 * \param c the character
 */
int getCharIndex(char c)
{
  for (size_t i = 0; i < sizeof(characters); i++)
    if (characters[i] == c)
      return i;
  errx(1, "The character is not valid !");
}

/**
 * \brief give the character corresponding with the index i
 *
 * \param i the index
 */
char getCharFromIndex(int i)
{
  return characters[i];
}
