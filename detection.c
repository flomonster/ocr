# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include "bitmap.h"
# include "queue.h"
# include "detection.h"

/**
 * \brief Put a marker for each line with a letter
 *
 * \param img the full image
 * \param array where marker is put in function of the img
 */
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

/**
 * \brief Put a marker for each column with a letter
 *
 * \param img one line of the full image
 * \param min X min
 * \param max X max
 * \param array where marker is put in function of the img *
 */
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

/**
 * \brief Create a bitmap from an other
 *
 * \param img full img
 * \param X min x
 * \param Y min y
 * \param width the width of the new bitmap
 * \param height the height of the new bitmap
 * *
 */
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

/**
 * \brief find the length average of letters for one line
 *
 * \param columnMarker array where there are marker for each pixel
 * \param width is the width of the columnMarker
 */
float letterLengthAverage(char *columnMarker, unsigned width)
{
  float nbLetter = 0;
  float sumWidthLetter = 0;

  unsigned i = 0;
  while (i < width && columnMarker[i] == 0)
    i++;
  while (i < width)
  {
    while (i < width && columnMarker[i] == 1)
    {
      sumWidthLetter++;
      i++;
    }
    nbLetter++;
    while (i < width && columnMarker[i] == 0)
        i++;
  }
  return sumWidthLetter / nbLetter;
}

/*
 * \brief Create a queue with all letter in a bitmap
 *
 * \param img the full image
 */
queue *segmentation(bitmap *img, size_t *nbCharacter, size_t *nbLetter)
{
  *nbCharacter = 0;
  *nbLetter = 0;
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
  float letterAverage;

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
      letterAverage = letterLengthAverage(columnMarker, img->width);
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
          (*nbCharacter)++;
        }
        else
        {
          X = j;
          while (j < img->width && columnMarker[j] == 0)
            j++;
          if ((float)j - X > letterAverage / 1.5 && j < img->width &&
              word->length )
          {
            enQueue(line, word);
            word = newQueue();
            (*nbLetter)++;
          }
        }
      }
      enQueue(line, word);
      enQueue(q, line);
      *nbLetter += 2;
    }
    i++;
  }
  *nbLetter += *nbCharacter;
  free(bmp);
  return q;
}
