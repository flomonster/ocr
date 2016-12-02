# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

bitmap *binerizeCopy(bitmap *);

void putLineMarker(bitmap *, char *, int, unsigned);

void putColumnMarker(bitmap *, unsigned, unsigned, char *, int, unsigned);

bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

float letterAverage(char *, unsigned);

void segmentation(bitmap *, size_t *, size_t *, queue *, bitmap *, int);

bitmap *widthTravel(bitmap *);

bitmap *heightTravel(bitmap *);

bitmap *merge(bitmap *, bitmap *);

void makeHistogram(bitmap *, bitmap *, unsigned, unsigned, queue *);

void textToHisto(queue *, bitmap *, bitmap *, float *, unsigned);

void histoToImage(bitmap *, bitmap *, queue *, float *);

bitmap *rlsa(bitmap *, queue *, queue *);

queue *doTheThings(bitmap *, size_t *, size_t *);

# endif
