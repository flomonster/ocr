# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include "bitmap.h"
# include "queue.h"
# include "detection.h"

/*
 * \brief binerize the copy of an image
 *
 * \param src is the image to copy
 */
bitmap *binerizeCopy(bitmap *src)
{
  color *content = malloc(sizeof(color) * src->height * src->width);
  for (unsigned c = 0; c < src->height * src->width; c++)
    content[c] = src->content[c];
  bitmap *bmp = newBitmap(src->width, src->height, content);
  binarize(bmp);
  return bmp;
}

/*
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

/*
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
    while (j < max && img->content[i].r == img->content[j * img->width + i].r)
      j++;
    array[i] = j == max ? 0 : 1;
  }
}

/*
 * \brief Put a marker for each column with a letter
 *
 * \param img one line of the full image
 * \param array where marker is put in function of the img *
 */
void checkBlackLine(bitmap *img, char *array)
{
  for (unsigned i = 0; i < img->height; i++)
  {
    unsigned j = 0; 
    while (j < img->width && img->content[i * img->width + j].r != 0)
      j++;
    array[i] = img->content[i * img->width + j].r != 0 ? 0 : 1;
  }
}

/*
 * \brief check if there is a black pixel for the rlsa 
 *
 * \param img one line of the full image
 * \param min X min 
 * \param max X max
 * \param array where marker is put in function of the img *
 */
void checkBlackColumn(bitmap *img, unsigned min, unsigned max, char *array)
{
  for(unsigned i = 0; i < img->width; i++)
  {
    unsigned j = min;
    while (j < max && img->content[j * img->width + i].r != 0)
      j++;
    array[i] = img->content[j* img->width + i].r != 0 ? 0 : 1;
  }
}

/*
 * \brief Create a bitmap from an other
 *
 * \param img full img
 * \param X min x
 * \param Y min y
 * \param width the width of the new bitmap
 * \param height the height of the new bitmap
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

/*
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
  if (nbLetter == 0)
    return 0;
  return sumWidthLetter / nbLetter;
}

/*
 * \brief find the height average of a line
 *
 * \param lineMarker array where there are marker for each pixel
 * \param height is the height of the lineMarker
 */
float lineHeigthAverage(char *lineMarker, unsigned height)
{
  float nbLine = 0;
  float sumHeightLine = 0;

  unsigned i = 0;
  while (i < height && lineMarker[i] == 0)
    i++;
  while (i < height)
  {
    while (i < height && lineMarker[i] == 1)
    {
      sumHeightLine++;
      i++;
    }
    nbLine++;
    while (i < height && lineMarker[i] == 0)
      i++;
  }
  if (nbLine == 0)
    return 0;
  return sumHeightLine / nbLine;
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
  bitmap *bmp = binerizeCopy(img);

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

/*
 * \brief create a new bitmap with width offset 
 *
 * \param src the orginal bitmap
 */
bitmap *widthTravel(bitmap *src)
{ 
  color *content = malloc(sizeof(color) * src->width * src->height);
  int pos, C;
  for (unsigned i = 0; i < src->height; i++)
    for (unsigned j = 0; j < src->width; j++)
    {
      if ((src->content)[i * src->width + j].r == 0)
        content[i * src->width + j] = newColor(0, 0, 0);
      else
      {
        if (j < 5 || j > src->width - 5)
          content[i * src->width + j] = newColor(255, 255, 255);
        else 
        {
          C = 20;  /*doit reussir a trouver la valeur exacte de C*/
          pos = j - 1;
          while (pos > 0 && C > 0 && 
              (src->content)[i * src->width + pos].r != 0)
          {
            pos--;
            C--;
          }
          if (pos > 0 && (src->content)[i * src->width + pos].r == 0)
            content[i * src->width + j] = newColor(0, 0, 0);
          else
          {
            C = 20; /*doit reussir a trouver la valeur exacte de C*/
            pos = j + 1;
            while ((unsigned)pos < src->width && C > 0 && 
              (src->content)[i * src->width + pos].r != 0)
            {
              pos++;
              C--;
            }
            if ((unsigned)pos < src->width &&
              (src->content)[i * src->width + pos].r == 0)
              content[i * src->width + j] = newColor(0, 0, 0);
            else
              content[i * src->width + j] = newColor(255, 255, 255);
          }
        }
      }
    }
  bitmap *dst = malloc(sizeof(bitmap));
  dst->width = src->width;
  dst->height = src->height;
  dst->content = content;
  return dst;
}

/*
 * \brief create a new bitmap with height offset 
 *
 * \param src the orginal bitmap
 */
bitmap *heightTravel(bitmap *src)
{
  color *content = malloc(sizeof(color) * src->width * src->height);
  int pos, C;
  for (unsigned i = 0; i < src->width; i++)
    for (unsigned j = 0; j < src->height; j++)
    {
      if ((src->content)[j * src->width + i].r == 0)
        content[j * src->width + i] = newColor(0, 0, 0);
      else
      {
        if (j < 5 || j > src->height - 5)
          content[j * src->width + i] = newColor(255, 255, 255);
        else  
        {
          C = 20;  /*doit reussir a trouver la valeur exacte de C*/
          pos = j - 1;
          while (pos > 0 && C > 0 && 
              (src->content)[pos * src->width + i].r != 0)
          {
            pos--;
            C--;
          }
          if (pos > 0 && (src->content)[pos * src->width + i].r == 0)
            content[j * src->width + i] = newColor(0, 0, 0);
          else
          {
            C = 20; /*doit reussir a trouver la valeur exacte de C*/
            pos = j + 1;
            while ((unsigned)pos < src->height && C > 0 && 
               (src->content)[pos * src->width + i].r != 0)
            {
              pos++;
              C--;
            }
            if ((unsigned)pos < src->height &&
              (src->content)[pos * src->width + i].r == 0)
              content[j * src->width + i] = newColor(0, 0, 0);
            else
              content[j * src->width + i] = newColor(255, 255, 255);
          }
        }
      }
    }
  bitmap *dst = malloc(sizeof(bitmap));
  dst->width = src->width;
  dst->height = src->height;
  dst->content = content;
  return dst;
}

/*
 * \brief Fusion two bitmap in one other 
 *
 * \param src1 the first bitmap 
 * \param src2 the second bitmap
 */
bitmap *merge(bitmap *src1, bitmap *src2)
{
  color *content = malloc(sizeof(color) * src1->width * src1->height);
  for (unsigned i = 0; i < src1->height; i++)
    for (unsigned j = 0; j < src1->width; j++)
    {
      if ((src1->content)[i * src1->width + j].r == 0 && 
          (src2->content)[i * src2->width + j].r == 0)
        content[i * src1->width + j] = newColor(0, 0, 0);
      else 
        content[i * src1->width + j] = newColor(255, 255, 255);
    }
  bitmap *dst = malloc(sizeof(bitmap));
  dst->width = src1->width;
  dst->height = src1->height;
  dst->content = content;
  return dst;
}

char checkClass(histogram *histo, float* hm)
{
  float Rm = histo->DC / histo->TC;
  return histo->deltaX < 3 * Rm && histo->deltaY < 3 * (*hm);
}

/*
 * \brief make the histogram of one block and add it in a queue
 *
 * \param bmp is the block 
 * \param original is copy binarize of the original image 
 * \param X higth left corner of the block on the copy in abscisse 
 * \param Y higth left corner of the block on the copy in ordonate
 * \param q is the queue where we add the hsitogram
 */
void makeHistogram(bitmap *bmp, bitmap *original, unsigned X, unsigned Y,
    queue *q)
{
  draw(bmp);
  draw(original);
  unsigned DC = 0, TC = 0;
  histogram *histo = malloc(sizeof(histogram));
  histo->x = X;
  histo->y = Y;
  histo->deltaX = bmp->width;
  histo->deltaY = bmp->height;
  for (unsigned i = 0; i < bmp->height; i++)
    for (unsigned j = 0; j < bmp->width; j++)
    {
      if (original->content[(i + X) * bmp->width + j + Y].r == 0)
        DC++;
      if (original->content[(i + X) * bmp->width + j + Y].r != 
          bmp->content[i * bmp->width + j].r)
        TC++;
    }
  histo->DC = DC;
  histo->TC = TC;
  enQueue(q, histo);
}

/*
 * \brief Return an image without black line et draw
 *
 * \param src the original image
 */
queue *textToHisto(bitmap *src, bitmap *original, float *hm) 
{
  unsigned nbLine = 0;

  queue *histoQueue = newQueue();
  bitmap *bmp = binerizeCopy(original); 

  char lineMarker[src->height];
  char columnMarker[src->width];
  putLineMarker(bmp, lineMarker);
  unsigned Y, X, Y_, X_;

  // Premier passage afin d'extraire les block noir 
  unsigned i = 0;
  while (i < src->height)
  {
    if (i < src->height && lineMarker[i] == 1)
    {
      Y = i;
      while (i < src->height && lineMarker[i] == 1)
        i++;

      putColumnMarker(bmp, Y, i, columnMarker);
      unsigned j = 0;
      while (j < src->width)
      {
        if (j < src->width && columnMarker[j] == 1)
        {
          X = j;
          while (j < src->width && columnMarker[j] == 1)
            j++;
         
          bitmap *bmpResult = cutBmp(src, X, Y, j - X, i - Y);
          char lineMarker_[bmpResult->height];
          char columnMarker_[bmpResult->width];
          checkBlackLine(bmpResult, lineMarker_);
          
          // Deuxieme passage afin d'etre sur que les bloc noir sont individuel 
          unsigned k = 0;
          while (k < bmpResult->height)
          {
            if (k < bmpResult->height && lineMarker_[k] == 1)
            {
              Y_ = k;
              while (k < bmpResult->height && lineMarker_[k] == 1)
                k++;

              checkBlackColumn(bmpResult, Y_, k, columnMarker_);
              unsigned l = 0;
              while (l < bmpResult->width)
              {
                if (l < bmpResult->width && columnMarker_[l] == 1)
                {
                  X_ = l;
                  while (l < bmpResult->width && columnMarker_[l] == 1)
                    l++;
                  bitmap *final = cutBmp(bmpResult, X_, Y_, l - X_, k - Y_);
                  makeHistogram(final, bmp, X_ + X, Y_ + Y, histoQueue);
                  nbLine++;
                  *hm += final->height;
                }
                else
                  while (l < bmpResult->width && columnMarker_[l] == 0)
                    l++;
              }
            }
            else
              while (k < bmpResult->height && lineMarker_[k] == 0)
                k++;
          }
        }
        else
          while (j < src->width && columnMarker[j] == 0)
            j++;
      }
    }
    else 
      while (i < src->height && lineMarker[i] == 0)
        i++;
  }
  if (nbLine == 0)
    *hm = 0;
  else 
    *hm /= nbLine;
  return histoQueue;
}

bitmap *histoToImage(bitmap *src, queue *histoQueue, float *hm)
{
  color *content = malloc(sizeof(color) * src->width * src->height);
  for (unsigned i = 0; i < src->width * src->height; i++)
    content[i] = newColor(255, 255, 255);
  while (histoQueue->length > 0)
  {
    histogram *histo = deQueue(histoQueue);
    printf("x : %u / y : %u / deltaX : %u / deltaY : %u / DC : %u /TC : %u \n",
        histo->x, histo->y, histo->deltaX, histo->deltaY, histo->DC, histo->TC);
    if (checkClass(histo, hm))
    {
      printf("Vraie \n");
      for (unsigned i = histo->y; i < histo->deltaY; i++)
        for (unsigned j = histo->x; j < histo->deltaX; j++)
          content[i * src->width + j] = src->content[i * src->width +j];
    }
  }
  bitmap *result = newBitmap(src->width, src->height, content);
  return result;
}

bitmap *rlsa(bitmap *src)
{
  bitmap *first = widthTravel(src);
  //draw(first);
  bitmap *seconde = heightTravel(src);
  //draw(seconde);
  bitmap *fusion = merge(first, seconde);
  //draw(fusion);

  float hm = 0;
  queue *histoQueue = textToHisto(fusion, src, &hm);
  bitmap *final = histoToImage(src, histoQueue, &hm);
  return final;
}
