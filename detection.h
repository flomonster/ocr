# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

struct s_histogram
{
  unsigned BC; 
  unsigned xMin;
  unsigned deltaX;
  unsigned yMin;
  unsigned deltaY;
  unsigned DC;
  unsigned TC; 
  unsigned Class;
};
typedef struct s_histogram histogram;

void putLineMarker(bitmap *, char *);

void putColumnMarker(bitmap *, unsigned, unsigned, char *);

bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

float letterLengthAverage(char *, unsigned);

queue *segmentation(bitmap *, size_t *, size_t *);

void widthTravel(bitmap *, bitmap *);

void heigthTravel(bitmap *, bitmap *);

void merge(bitmap *, bitmap *, bitmap *);

# endif

