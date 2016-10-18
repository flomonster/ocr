# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include "bitmap.h"
# include "detection.h"

queuE *newQueuE()
{
  queuE *q =  malloc(sizeof(queuE));
  q->length = 0;
  q->first = NULL;
  q->last = NULL;
  return q;
}

void enQueuE(queuE *q, bitmap img)
{
  element *el = malloc(sizeof(element));
  el->img = img;
  el->next = NULL;

  if (q->length == 0)
    q->first = el;
  else 
  {
    element *bLast = q->last;
    bLast->next = el;
  }
  q->last = el;
  q->length++;
}

element deQueuE(queuE *q)
{
  if (q->length > 0)
  {
    element *save = q->first->next;
    element el = *q->first;
    free(q->first);
    q->first = save;
    q->length--;
    return el;
  }
  else 
    exit(EXIT_FAILURE);
}

void putLineMarker(bitmap *img, char *array)
{
  for(unsigned i = 0; i < img->height; i++)
  {
    unsigned j = 1; 
    while (j < img->width - 1 && 
        img->content[i * img->width].r == img->content[i * img->width + j].r)
      j++;
    array[i] = j == img->width - 1 ? 0 : 1;
  }
}

void putCollumnMarker(bitmap *img, char *array)
{
  for(unsigned i = 0; i < img->width; i++)
  {
    unsigned j = 1; 
    while (j < img->height - 1 && 
        img->content[i].r == img->content[j * img->width + i].r)
      j++;
    array[i] = j == img->height - 1 ? 0 : 1;
  }
}

void cutCollumn(bitmap *img, char *array, queuE *q)
{
  // i -> parcour toute l'image de gauche a droite
  // j -> permet de connaitre la largeur de la lettre 
  // k -> parcour la new image de gauche a droite 
  // l -> parcour la new image de haut en bas
  for(unsigned i = 0; i < img->width; i++)
  {
    if (array[i] == 1)
    {
      unsigned j = 0; 
      while (array[j + i] == 1)
        j++;
      color *content = malloc(sizeof(color) * img->height * j);
      
      for (unsigned k = 0; k < j; k++)
        for (unsigned l = 0; l < img->height; l++)
          content[l * j + k] = img->content[l * img->width + i + k];
      i += j;
      bitmap bmp = newBitmap(j, img->height, content);
      enQueuE(q, bmp);
    }
  }
}

void cutLine(bitmap *img, char *array, queuE *q)
{
  // i -> parcour toute l'image de haut en bas
  // j -> permet de connaitre la hauteur de l'image
  // k -> parcour la new image de haut en bas
  // l -> parcour l'image de gauche a droite
  for (unsigned i = 0; i < img->height; i++)
  {
    if (array[i] == 1)
    {
      unsigned j = 0; 
      while (array[j + i] == 1)
        j++;
      color *content = malloc(sizeof(color) * img->width * j);
      
      for (unsigned k = 0; k < j; k++)
        for (unsigned l = 0; l < img->width; l++)
          content[k * img->width + l] = img->content[(k + i) * img->width + l];
      i += j;
      bitmap bmp = newBitmap(img->width, j, content);
      enQueuE(q, bmp);
    }
  }
}

