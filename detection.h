# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

struct s_histogram
{
  unsigned x;
  unsigned y;
  unsigned deltaX;
  unsigned deltaY;
  unsigned DC;
  unsigned TC;
};
typedef struct s_histogram histogram;

bitmap *binerizeCopy(bitmap *);

void putLineMarker(bitmap *, char *);

void putColumnMarker(bitmap *, unsigned, unsigned, char *);

bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

float letterLengthAverage(char *, unsigned);

float lineHeigthAverage(char *, unsigned);

queue *segmentation(bitmap *, size_t *, size_t *);

bitmap *widthTravel(bitmap *);

bitmap *heightTravel(bitmap *);

bitmap *merge(bitmap *, bitmap *);

char checkClass(histogram *, float *);

void makeHistogram(bitmap *, bitmap *, unsigned, unsigned, queue *);

queue *textToHisto(bitmap *, bitmap *, float *);

bitmap *histoToImage(bitmap *, queue *, float *);

bitmap *rlsa(bitmap *);

# endif

