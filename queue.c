/**
 * \file main.c
 * \brief dynamic storage
 * \author issarn_t
 * \date 09/29/2016
 */
# include "queue.h"

/**
 * \brief  Create new queue
 */
queue *newQueue()
{
  queue *q =  malloc(sizeof(queue));
  q->length = 0;
  return q;
}

/**
 * \brief  Append element to a queue
 *
 * \param q the queue to be modified
 * \param obj the element to be added
 */
void enQueue(queue *q, void *obj)
{
  element *elt = malloc(sizeof(element));
  elt->obj = obj;

  if (q->length == 0)
    q->first = elt;
  else
    q->last->next = elt;
  q->last = elt;
  q->length++;
}

/**
 * \brief Remove and give you the first element of a queue
 *
 * \param q the queue to be  modified
 */
void *deQueue(queue *q)
{
  if (q->length > 0)
  {
    void *obj = q->first->obj;
    element *save = q->first;
    q->first = q->first->next;
    free(save);
    q->length--;
    return obj;
  }
  else
    errx(1, "Can not dequeue : length = 0 !");
}
