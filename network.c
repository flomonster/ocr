# include <stdio.h>
# include <stdlib.h>
# include "network.h"

// Generate a neural network
network *newNetwork(unsigned nblayer, unsigned *layers)
{
  network *n = (network *) malloc(sizeof(network));

  float **t = (float **) malloc(sizeof(float *) * (nblayer - 1));
  for (unsigned i = 1; i < nblayer; i++)
  {
    float *neurone = (float *) malloc(sizeof(float) * layers[i]);
    for (unsigned j = 0; j < layers[i]; j++)
      neurone[j] = (float) rand() / (float) RAND_MAX;
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
        target[k] = (float) rand() / (float) RAND_MAX;
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

// Free the memory used by a neural network
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
  free(n);
}


network *loadNetwork(char *path);
/*{
  
  FILE *file = fopen(path, "r");
  unsigned nblayer;
  
  
  
  
  //nblayer
  fgets(
  network n;
  return n;
  
}
*/
// Save the network in a file
void saveNetwork(char *path, network *n)
{
  FILE *file = fopen(path, "w");
  if (file == NULL)
    errx("error during fopen");
  
  char s[5];
  //nblayer
  sprintf(s, "%d", n->nblayer); 
  fputs(s, file);
  fputc('\n', file);
  
  //*layers
  unsigned k = 0;
  unsigned l,m;
  while(k < n->nblayer - 1)
  {
    sprintf(s, "%d", n->layers[k]);
    fputs(s, file);
    fputc(' ', file);
    k++;
  }
  sprintf(s, "%d", n->layers[k]);
  fputs(s, file);
  fputc('\n', file);
  
  //**threshold
  for(k = 1; k < n->nblayer; k++)
  {
    l = 0;
    while(l < n->layers[k] - 1)
    {
      sprintf(s, "%f", n->threshold[k - 1][l]);
      fputs(s, file);
      fputc(' ', file);
      l++;
    }
    sprintf(s, "%f", n->threshold[k - 1][l]);
    fputs(s, file);
    fputc('\n', file);
  }

  //***weight
  for(k = 0; k < n->nblayer - 1; k++)
  {
    for(l = 0; l < n->layers[k]; l++)
    {
      m = 0;
      while(m < n->layers[k + 1] - 1)
      {
        sprintf(s, "%f", n->weight[k][l][m]);
        fputs(s, file);
        fputc(' ', file);
        m++;
      }
      sprintf(s, "%f", n->weight[k][l][m]);
      fputs(s, file);
      fputc('\n', file);
    }
  }
  

}





