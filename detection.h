# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"


typedef struct s_element element;
struct s_element
{
  bitmap img; 
  element *next;
};

struct s_queuE
{
  int length; 
  element *first;
  element *last;
};
typedef struct s_queuE queuE;

queuE *newQueuE();

void enQueuE(queuE *, bitmap);

element deQueuE(queuE *);

void putLineMarker(bitmap *, char *);

void putCollumnMarker(bitmap *, char *);

void cutCollumn(bitmap *, char *, queuE *);

# endif

