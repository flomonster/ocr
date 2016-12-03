/**
 * \file detection.h 
 * \brief Detect text and only it in a image
 * \author issarn_t
 * \date 09/17/2016
 */
# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

/**
 * \brief return a queue which contain all letter 
 *
 * \param src is the original image
 * \param nbCharacter is the number of character
 * \param nbLetter is the number of letter
 */
queue *detectText(bitmap *, size_t *, size_t *);

/**
 * \brief Create a queue with all letter in a bitmap
 *
 * \param img the full image
 * \param nbCharacter the number of character in the image
 * \param nbLetter the number of letter int the image
 * \param q is the queue where is stocked all letter 
 * \param cutedImage is the the image that we use 
 * \param pos is the x min
 */
void segmentation(bitmap *, size_t *, size_t *, queue *, bitmap *, int);

# endif
