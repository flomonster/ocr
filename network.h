#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

struct s_network
{
  unsigned nblayer;
  unsigned *layers;
  float **treshold;
  float **out;
  float **delta;
  float ***weight;
};

typedef struct s_network network;

network newNetwork(unsigned  nblayer, unsigned *layers);

void freeNetwork(network *n);

network loadNetwork(char *path);

void saveNetwork(char *path, network n);

#endif
