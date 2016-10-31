# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <err.h>
# include "bitmap.h"
# include "network.h"
# include "ocr.h"

char alphabet[66] = {
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', ',', '?',
'!'
};

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
 * \brief the activation function of a neural network
 *
 * \param n the neural network
 * \param inputs values for the input layer
 */
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

/**
 * \brief entrain a neural network with samples
 *
 * \param n the neural network
 * \param samples the inputs of samples
 * \param results the outputs of samples
 * \param nbSample the number of samples
 */
void learn(network *n, float **samples, float **results, unsigned nbSample,
    float speed, float goal)
{
  float error = 1;
  while (error > goal)
  {
    printf("  - STATUS : %d%%\n", (int) ((1 - error) / (1 - goal) * 100));
    for (int i = 0; i < 1000; i++)
      for (unsigned j = 0; j < nbSample; j++)
      {
        feedForward(n, samples[j]);
        backPropagation(n, results[j]);
        update(n, speed);
      }
    error = evaluate(n, samples, results, nbSample);
  }
  printf("  - STATUS : 100%%\n");
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
  for (size_t i = 0; i < sizeof(alphabet); i++)
    if (alphabet[i] == c)
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
  return alphabet[i];
}
