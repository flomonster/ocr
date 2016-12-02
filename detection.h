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

# endif
