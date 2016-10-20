# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"

struct s_element
{
  void *obj; 
  struct s_element *next;
};
typedef struct s_element element;

struct s_queue
{
  int length; 
  element *first;
  element *last;
};
typedef struct s_queue queue;

queue *newQueue();

void enQueue(queue *, void *);

void *deQueue(queue *);

void putLineMarker(bitmap *, char *);

void putColumnMarker(bitmap *, unsigned, unsigned, char *);

bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

queue *segmentation(bitmap *);

# endif

