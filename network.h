#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

struct S_NETWORK
{
  unsigned nblayer;
  unsigned *layers;
  float **treshold;
  float **out;
  float **delta;
  float ***weight;
};

typedef struct S_NETWORK NETWORK;

NETWORK newNetwork(unsigned  nblayer, unsigned *layers);

void freeNetwork(NETWORK *n);

NETWORK loadNetwork(char *path);

void saveNetwork(char *path, NETWORK n);

#endif
