# ifndef QUEUE_H_DEFINED
# define QUEUE_H_DEFINED
# include <stdlib.h>
# include <err.h>

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

# endif
