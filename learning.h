/**
 * \file learning.c
 * \brief The header of learning.c
 * \author amsall_f
 * \date 09/17/2016
 */
# ifndef LEARNING_H_INCLUDED
# define LEARNING_H_INCLUDED_H_INCLUDED
# include "bitmap.h"
# include "network.h"
# include "queue.h"

/**
 * \brief learn to the network "network.save"
 *
 * \param learnFiles the table of path to learn files
 * \param nbFile the number of file
 */
void learning(char *[], size_t);

/**
 * \brief fill the samples with a segmented image and return the size
 *
 * \param text the image segmented 
 * \param samples the table of  sample to fill 
 */
int createSamples(queue *, float **);

/**
 * \brief Create the results of samples
 *
 * \param text the expected text
 * \param nbsample the length of the text
 * \param nbOutput the number of possibile outputs
 */
float **createResults(char[], int);

/**
 * \brief free all components of a sample
 *
 * \param samples the inputs or outputs of a sample
 * \param nbSample the number of character in the sample
 */
void freeSamples(float **, int);
# endif
