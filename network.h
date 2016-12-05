/**
 * \file network.h
 * \brief The header of network.c 
 * \author amsall_f and astain_d
 * \date 09/17/2016
 */
# ifndef NETWORK_H_INCLUDED
# define NETWORK_H_INCLUDED
# include <stdio.h>
# include <stdlib.h>

/**
 * \struct s_network
 * \brief The structure of a neural network
 */
struct s_network
{
  unsigned nblayer; /*!< The number of layer */
  unsigned *layers; /*!< The number of neurone for each layer */
  float **threshold; /*!< The threshold or bias of the neural network */
  float **out; /*!< The activation value of each neurone */
  float **delta; /*!< The deltas of each neurone */
  float ***weight; /*!< The weight of each neurone */
};

typedef struct s_network network;

/**
 * \brief create a new neural network
 *
 * \param nblayer the number of layers
 * \param layers a table of the number of neurone in each layer
 */
network *newNetwork(unsigned  nblayer, unsigned *layers);

/**
 * \brief free all components of a network
 *
 * \param n the neural network
 */
void freeNetwork(network *n);

/**
 * \brief load a neural network from a save
 *
 * \param path location of the save
 */
network *loadNetwork(char *path);

/**
 * \brief save weights anf thresholds of a neural network
 *
 * \param path location of the new file
 * \param n the neural network to save
 */
void saveNetwork(char *path, network *n);

/**
 * \brief generate a network of three layer
 *
 * \param c1 the number of neurone for the first layer
 * \param c1 the number of neurone for the second layer
 * \param c1 the number of neurone for the third layer
 */
void generateNetwork();
# endif
