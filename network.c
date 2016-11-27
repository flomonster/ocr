# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <math.h>
# include "network.h"

/**
 * \brief create a new neural network
 *
 * \param nblayer the number of layers
 * \param layers a table of the number of neurone in each layer
 */
network *newNetwork(unsigned nblayer, unsigned *layers)
{
  network *n = (network *) malloc(sizeof(network));

  float **t = (float **) malloc(sizeof(float *) * (nblayer - 1));
  for (unsigned i = 1; i < nblayer; i++)
  {
    float *neurone = (float *) malloc(sizeof(float) * layers[i]);
    for (unsigned j = 0; j < layers[i]; j++)
      neurone[j] = (float) rand() / (float) RAND_MAX * 2 - 1;
    t[i - 1] = neurone;
  }

  float **o = (float **) malloc(sizeof(float *) * nblayer);
  for (unsigned i = 0; i < nblayer; i++)
  {
    float *neurone = (float *) malloc(sizeof(float) * layers[i]);
    for (unsigned j = 0; j < layers[i]; j++)
      neurone[j] = 0;
    o[i] = neurone;
  }

  float **d = (float **) malloc(sizeof(float *) * (nblayer - 1));
  for (unsigned i = 1; i < nblayer; i++)
  {
    float *neurone = (float *) malloc(sizeof(float) * layers[i]);
    for (unsigned j = 0; j < layers[i]; j++)
      neurone[j] = 0;
    d[i - 1] = neurone;
  }

  float ***w = (float ***) malloc(sizeof(float **) * (nblayer - 1));
  for (unsigned i = 0; i < nblayer - 1; i++)
  {
    float **origin = (float **) malloc(sizeof(float **) * layers[i]);
    for (unsigned j = 0; j < layers[i]; j++)
    {
      float *target = (float *) malloc(sizeof(float) * layers[i+1]);
      for (unsigned k = 0; k < layers[i+1]; k++)
        target[k] = (float) rand() / (float) RAND_MAX * 2 - 1;
      origin[j] = target;
    }
    w[i] = origin;
  }

  n->nblayer = nblayer;
  n->layers = layers;
  n->threshold = t;
  n->out = o;
  n->delta = d;
  n->weight = w;
  return n;
}

/**
 * \brief free all components of a network
 *
 * \param n the neural network
 */
void freeNetwork(network *n)
{
  for (unsigned i = 0; i < n->nblayer - 1; i++)
    free(n->threshold[i]);
  free(n->threshold);
  for (unsigned i = 0; i < n->nblayer; i++)
    free(n->out[i]);
  free(n->out);
  for (unsigned i = 0; i < n->nblayer - 1; i++)
    free(n->delta[i]);
  free(n->delta);
  for (unsigned i = 0; i < n->nblayer - 1; i++)
  {
    for (unsigned j = 0; j < n->layers[i]; j++)
      free(n->weight[i][j]);
    free(n->weight[i]);
  }
  free(n->weight);
  free(n->layers);
  free(n);
}

/**
 * \brief load a neural network from a save
 *
 * \param path location of the save
 */
network *loadNetwork(char *path)
{

  FILE *file = fopen(path, "r");

  //nblayer
  unsigned nblayer;
  fread(&nblayer, sizeof(unsigned), 1, file);

  //*layers
  unsigned *layers = malloc(nblayer * sizeof(unsigned));
  fread(layers, sizeof(unsigned), nblayer, file);

  network *n = newNetwork(nblayer, layers);

  //**threshold
  for(unsigned k = 0; k < n->nblayer - 1; k++)
    fread(n->threshold[k], sizeof(float), n->layers[k + 1], file);

  //***weight
  for(unsigned k = 0; k < n->nblayer - 1; k++)
    for(unsigned l = 0; l < n->layers[k]; l++)
      fread(n->weight[k][l], sizeof(float), n->layers[k + 1], file);

  fclose(file);

  return n;
}

/**
 * \brief save weights anf thresholds of a neural network
 *
 * \param path location of the new file
 * \param n the neural network to save
 */
void saveNetwork(char *path, network *n)
{
  FILE *file = fopen(path, "w");
  if (file == NULL)
    errx(1, "error during fopen");

  //nblayer
  fwrite(&n->nblayer, sizeof(unsigned), 1, file);
  //fputc('\n', file);

  //*layers
  fwrite(n->layers, sizeof(unsigned), n->nblayer, file);

  //**threshold
  for(unsigned k = 1; k < n->nblayer; k++)
    fwrite(n->threshold[k - 1], sizeof(float), n->layers[k], file);

  //***weight
  for(unsigned k = 0; k < n->nblayer - 1; k++)
    for(unsigned l = 0; l < n->layers[k]; l++)
      fwrite(n->weight[k][l], sizeof(float), n->layers[k + 1], file);

  fclose(file);
}

/**
 * \brief generate a network of three layer
 *
 * \param c1 the number of neurone for the first layer
 * \param c1 the number of neurone for the second layer
 * \param c1 the number of neurone for the third layer
 */
void generateNetwork()
{
  unsigned *layers = malloc(sizeof(unsigned) * 3);
  layers[0] = 256;
  layers[1] = 80;
  layers[2] = 95;
  network *n = newNetwork(3, layers);

  printf("Generating the network :\n");
  printf("  - Layer size : %d | %d | %d\n", layers[0], layers[1], layers[2]);
  printf("  - Creation of network.save\n");
  saveNetwork("network.save", n);
  printf("DONE\n");

  freeNetwork(n);
}
