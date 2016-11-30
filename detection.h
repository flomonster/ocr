# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

bitmap *binerizeCopy(bitmap *);

void putLineMarker(bitmap *, char *);

void putColumnMarker(bitmap *, unsigned, unsigned, char *);

void checBlackLine(bitmap *, char *);

void checBlackColumn(bitmap *, char *, unsigned, unsigned);

bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

float letterAverage(char *, unsigned);

queue *segmentation(bitmap *, size_t *, size_t *);

bitmap *widthTravel(bitmap *);

bitmap *heightTravel(bitmap *);

bitmap *merge(bitmap *, bitmap *);

void makeHistogram(bitmap *, bitmap *, unsigned, unsigned, queue *);

void textToHisto(queue *, bitmap *, bitmap *, float *, unsigned);

void histoToImage(bitmap *, bitmap *, queue *, float *);

bitmap *rlsa(bitmap *, queue *, queue *);

# endif
