# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

void putLineMarker(bitmap *, char *);

void putColumnMarker(bitmap *, unsigned, unsigned, char *);

bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

float letterLengthAverage(char *, unsigned);

queue *segmentation(bitmap *, size_t *, size_t *);

# endif

