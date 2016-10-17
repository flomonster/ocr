# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include "bitmap.h"

typedef struct s_element element;
struct s_element
{
  bitmap img;
  element * next;
};

struct s_queu  
{
  int length;
  element * first;
  element * last;
};
typedef struct s_queu queu;

struct s_queuOfQueu
{
  int length; 
  queu * first; 
  queu * last;
};
typedef struct s_queuOfQueu queuOfQueu;

queu * newQueu()
{
  queu * q =  malloc(sizeof(queu));
  if (q == NULL)
    exit(EXIT_FAILURE); 
  q->length = 0;
  q->first = NULL;
  q->last = NULL;
  return q;
}

void enQueu(queu * q, bitmap img)
{
  element * el = malloc(sizeof(element));
  if (el == NULL)
    exit(EXIT_FAILURE);

  el->img = img;
  el->next = NULL;

  if (q->length == 0)
    q->first = el;
  else 
  {
    element * bLast = q->last;
    bLast->next = el;
  }
  q->last = el;
  q->length++;
}

element dequeu(queu * q)
{
  if (q == NULL)
    exit(EXIT_FAILURE); 
  if (q->first != NULL)
  {
    element * save = q->first->next;
    element el = *q->first;
    free(q->first);
    q->first = save;
    q->length--;
    return el;
  }
  else 
    exit(EXIT_FAILURE);
}
