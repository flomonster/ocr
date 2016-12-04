/**
 * \file main.h
 * \brief dynamic storage
 * \author issarn_t
 * \date 09/29/2016
 */
# ifndef QUEUE_H_DEFINED
# define QUEUE_H_DEFINED
# include <stdlib.h>
# include <err.h>

/**
 * \struct s_element
 * \brief is an element of an queue
 */
struct s_element
{
  void *obj; /*!< obj which is stock in the element */
  struct s_element *next; /*!< is the next element of the queue */
};
typedef struct s_element element;

/**
 * \struct s_queue
 * \brief is the queue
 */
struct s_queue
{
  int length; /*!< the length of the queue */
  element *first; /*!< the fisrt element of the queue */
  element *last;  /*!< the last element of the queue */
};
typedef struct s_queue queue;

/**
 * \brief  Create new queue
 */
queue *newQueue();

/**
 * \brief  Append element to a queue
 *
 * \param q the queue to be modified
 * \param obj the element to be added
 */
void enQueue(queue *, void *);

/**
 * \brief Remove and give you the first element of a queue
 *
 * \param q the queue to be  modified
 */
void *deQueue(queue *);

# endif
