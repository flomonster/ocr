# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include "bitmap.h"
# include "detection.h"

// Put a marker for each line with a letter
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

// Put a marker for each column with a letter
void putColumnMarker(bitmap *img, unsigned min, unsigned max, char *array)
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

// Create a bitmap from an other 
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

float columnSpaceAverage(char *columnMarker, unsigned length)
{
  float share = 0;
  float nbSpaces = 0;
  float spaces = 0;
  char b = 1;

  unsigned i = 0;
  while (i < length)
  {
    if (columnMarker[i] == 0 && b == 1)
    {
      while (i < length && columnMarker[i] == 0)
        i++;
    }

    if (columnMarker[i] == 1 && b == 1)
    {
      b = 0;
      while (i < length && columnMarker[i] == 1)
        i++;
    }

    if (columnMarker[i] == 0 && b == 0)
    {
      share = 0;
      while (columnMarker[i] == 0 && b == 0)
      {
        share++;
        i++;
      }
      if (i != length)
      {
        spaces += share;
        nbSpaces++;
      }
    }

    if (columnMarker[i] == 1 && b == 0)
    {
      while (i < length && columnMarker[i] == 1)
        i++;
    }
  }
  return spaces / nbSpaces;
}

// Create a queue with all letter in a bitmap
queue *segmentation(bitmap *img)
{
  color content[img->height * img->width];
  for (unsigned c = 0; c < img->height * img->width; c++)
    content[c] = img->content[c];
  bitmap *bmp = newBitmap(img->width, img->height, content);
  binarize(bmp);

  queue *q = newQueue();
  char lineMarker[img->height];
  char columnMarker[img->width];
  putLineMarker(bmp, lineMarker);
  unsigned Y, X;
  float columnSpace;

  unsigned i = 0; 
  while (i < img->height)
  {
    if (i < img->height && lineMarker[i] == 1)
    {
      Y = i;
      while (i < img->height && lineMarker[i] == 1)
        i++;
      
      queue *line = newQueue();
      putColumnMarker(bmp, Y, i, columnMarker);

      columnSpace = columnSpaceAverage(columnMarker, img->width);
      queue *word = newQueue();

      unsigned j = 0;
      while (j < img->width)
      {
        if (j < img->width && columnMarker[j] == 1)
        {
          X = j;
          while (j < img->width && columnMarker[j] == 1)
            j++;

          bitmap *bmpResult = cutBmp(img, X, Y, j - X, i - Y);
          enQueue(word, bmpResult);
        }
        else 
        {
          X = j;
          while (j < img->width && columnMarker[j] == 0)
            j++;

          if (j - X > columnSpace && j < img->width && word->length > 0)
          {
            enQueue(line, word);
            word = newQueue();
          }
        }
      }
      enQueue(line, word);
      enQueue(q, line);
    }
    i++;
  }
  free(bmp);
  return q;
}
