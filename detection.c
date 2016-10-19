# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include "bitmap.h"
# include "detection.h"

queue *newQueue()
{
  queue *q =  malloc(sizeof(queue));
  q->length = 0;
  return q;
}

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

void putLineMarker(bitmap *img, char *array)
{
  for(unsigned i = 0; i < img->height; i++)
  {
    unsigned j = 1; 
    unsigned char mem = img->content[i * img->width].r;
    while (j < img->width && mem == img->content[i * img->width + j].r)
      j++;
    array[i] = j == img->width ? 0 : 1;
  }
}

void putCollumnMarker(bitmap *img, unsigned min, unsigned max, char *array)
{
  for(unsigned i = 0; i < img->width; i++)
  {
    unsigned j = min; 
    while (j < max &&
        img->content[i].r == img->content[j * img->width + i].r)
      j++;
    array[i] = j == max ? 0 : 1;
  }
}

bitmap *cutBmp(bitmap *img, unsigned X, unsigned Y,
    unsigned width, unsigned height)
{
  
  color *content = malloc(sizeof(color) * height * width);
  for (unsigned i = 0; i < height; i++)
  {
    for (unsigned j = 0; j < width; j++)
      content[i * width + j] = img->content[(i + Y) * img->width + j + X];
  }
  bitmap *bmp = newBitmap(width, height, content);
  return bmp;
}

queue *segmentation(bitmap *img)
{
  queue *q = newQueue();
  char lineMarker[img->height];
  char collumnMarker[img->width]; 
  putLineMarker(img, lineMarker);
  unsigned Y, X;

  unsigned i = 0; 
  while (i < img->height)
  {
    if (lineMarker[i] == 1)
    {
      Y = i;
      while (i < img->height && lineMarker[i] == 1)
        i++;
      
      queue *line = newQueue();
      putCollumnMarker(img, Y, i, collumnMarker);

      unsigned j = 0;
      while (j  < img->width)
      {
        if (collumnMarker[j] == 1)
        {
          X = j;
          while (collumnMarker[j] == 1)
            j++;
          bitmap *bmp = cutBmp(img, X, Y, j - X, i - Y);
          enQueue(line, bmp);
        }
        j++;
      }
      enQueue(q, line);
    }
    i++;
  }
  return q;
}
