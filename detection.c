# include <err.h>
# include "bitmap.h"
# include "queue.h"
# include "detection.h"

bitmap *binerizeCopy(bitmap *src)
{
  color *content = malloc(sizeof(color) * src->height * src->width);
  for (unsigned c = 0; c < src->height * src->width; c++)
    content[c] = src->content[c];
  bitmap *bmp = newBitmap(src->width, src->height, content);
  binarize(bmp);
  return bmp;
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

void checkBlackColumn(bitmap *img, char *array, unsigned min, unsigned max)
{
  for(unsigned i = 0; i < img->width; i++)
  {
    unsigned j = min;
    while (j < max && img->content[j * img->width + i].r != 0)
      j++;
    array[i] = img->content[j* img->width + i].r != 0 ? 0 : 1;
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
  freeBitmap(bmp);
  return q;
}

bitmap *widthTravel(bitmap *src)
{
  unsigned step = 150;
  color *content = malloc(sizeof(color) * src->width * src->height);
  for (unsigned i = 0; i < src->height * src->width; i++)
    content[i] = newColor(255, 255, 255);

  for (unsigned i = 0; i < src->height; i++)
  {
    unsigned j = 0, min = 0, max = 0, terminal;
    while (j < src->width)
    {
      if (!src->content[i * src->width + j].r)
      {
        unsigned k = j;
        terminal = j - step < min ? min : j - step;
        while (k > terminal)
        {
          content[i * src->width + k] = newColor(0, 0, 0);
          k--;
        }

        k = max < j ? j : max;
        terminal = j + step > src->width ? src->width : j + step;
        while (k < terminal)
        {
          content[i * src->width + k] = newColor(0, 0, 0);
          k++;
        }
        max = j + step;
        min = j + step;
      }
      j++;
    }
  }
  return newBitmap(src->width, src->height, content);
}

bitmap *heightTravel(bitmap *src)
{
  unsigned step = 50;
  color *content = malloc(sizeof(color) * src->width * src->height);
  for (unsigned i = 0; i < src->width * src->height; i++)
    content[i] = newColor(255, 255, 255);

  for (unsigned i = 0; i < src->width; i++)
  {
    unsigned j = 0, min = 0, max = 0, terminal;
    while (j < src->height)
    {
      if (!src->content[j * src->width + i].r)
      {
        unsigned k = j;
        terminal = j - step < min ? min : j - step;
        while (k > terminal)
        {
          content[k * src->width + i] = newColor(0, 0, 0);
          k--;
        }

        k = max < j ? j : max;
        terminal = j + step > src->height ? src->height : j + step;
        while (k < terminal)
        {
          content[k * src->width + i] = newColor(0, 0, 0);
          k++;
        }
        max = j + step;
        min = j + step;
      }
      j++;
    }
  }
  return newBitmap(src->width, src->height, content);
}

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
  float rm;
  if (histo->TC == 0)
    return 0; 
  rm = (float)histo->DC / histo->TC;
  return 2 < 10 * rm  && histo->deltaY < 3 * (unsigned)(*hm);
}

void makeHistogram(bitmap *bmp, bitmap *original, unsigned X, unsigned Y,
    queue *q)
{
  unsigned DC = 0, TC = 0;
  histogram *histo = malloc(sizeof(histogram));
  histo->x = X;
  histo->y = Y;
  histo->deltaX = bmp->width;
  histo->deltaY = bmp->height;
  for (unsigned i = 0; i < bmp->height; i++)
    for (unsigned j = 0; j < bmp->width; j++)
    {
      if (original->content[(i + Y) * original->width + j + X].r == 0)
        DC++;
      if (original->content[(i + Y) * original->width + j + X].r != 
          bmp->content[i * bmp->width + j].r)
        TC++;
    }
  histo->DC = DC;
  histo->TC = TC;
  enQueue(q, histo);
}

queue *textToHisto(bitmap *src, bitmap *original, float *hm) 
{
  unsigned nbLine = 0;
  queue *histoQueue = newQueue();
  bitmap *bmp = binerizeCopy(original); 
  char lineMarker[src->height];
  char columnMarker[src->width];
  putLineMarker(bmp, lineMarker);
  unsigned Y, X;

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
          makeHistogram(bmpResult, bmp, X, Y, histoQueue);
          nbLine++;
          *hm += bmpResult->height;
          freeBitmap(bmpResult);
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
  freeBitmap(bmp);
  if (nbLine == 0)
    *hm = 0;
  else 
    *hm = (*hm) / nbLine;
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
    if (checkClass(histo, hm))
    {
      for (unsigned i = histo->y; i < histo->y + histo->deltaY; i++)
        for (unsigned j = histo->x; j < histo->x + histo->deltaX; j++)
          content[i * src->width + j] = src->content[i * src->width +j];
    }
    free(histo);
  }
  bitmap *result = newBitmap(src->width, src->height, content);
  free(histoQueue);
  return result;
}

bitmap *rlsa(bitmap *src)
{
  binarize(src);
  saveBmp("Save.bmp",src);
  bitmap *first = widthTravel(src);
  saveBmp("Savewidth.bmp", first);
  bitmap *second = heightTravel(src);
  saveBmp("Saveheight.bmp", second);
  bitmap *fusion = merge(first, second);
  saveBmp("Savefusion.bmp", fusion);

  float hm[1] = {0};
  queue *histoQueue = textToHisto(fusion, src, hm);
  bitmap *final = histoToImage(src, histoQueue, hm);
  saveBmp("Savefinal.bmp", final);

  freeBitmap(first);
  freeBitmap(second);
  freeBitmap(fusion);
  return final;
}
