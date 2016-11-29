# include <err.h>
# include "bitmap.h"
# include "queue.h"
# include "detection.h"

struct s_histogram
{
  unsigned x;
  unsigned y;
  unsigned deltaX;
  unsigned deltaY;
  unsigned dc;
  unsigned tc;
};
typedef struct s_histogram histogram;

/**
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
    while (j < max && img->content[i].r == img->content[j * img->width + i].r)
      j++;
    array[i] = j == max ? 0 : 1;
  }
}

/**
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

/**
 * \brief check if there is a black pixel for the rlsa 
 *
 * \param img one line of the full image
 * \param array where marker is put in function of the img *
 * \param min X min 
 * \param max X max
 */
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

/**
 * \brief Create a bitmap from an other
 *
 * \param img full img
 * \param X min x
 * \param Y min y
 * \param width the width of the new bitmap
 * \param height the height of the new bitmap
 */
bitmap *cutBmp(bitmap *img, unsigned x, unsigned y,
    unsigned width, unsigned height)
{
  color *content = malloc(sizeof(color) * height * width);
  for (unsigned i = 0; i < height; i++)
  {
    for (unsigned j = 0; j < width; j++)
      content[i * width + j] = img->content[(i + y) * img->width + j + x];
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
  return !nbLetter ? 0 : sumWidthLetter / nbLetter;
}

/**
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
  return !nbLine ? 0 : sumHeightLine / nbLine;
}

/**
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
  unsigned y, x;
  float letterAverage;

  unsigned i = 0;
  while (i < img->height)
  {
    if (i < img->height && lineMarker[i] == 1)
    {
      y = i;
      while (i < img->height && lineMarker[i] == 1)
        i++;

      queue *line = newQueue();
      putColumnMarker(bmp, y, i, columnMarker);
      letterAverage = letterLengthAverage(columnMarker, img->width);
      queue *word = newQueue();

      unsigned j = 0;
      while (j < img->width)
      {
        if (j < img->width && columnMarker[j] == 1)
        {
          x = j;
          while (j < img->width && columnMarker[j] == 1)
            j++;

          bitmap *bmpResult = cutBmp(img, x, y, j - x, i - y);
          enQueue(word, bmpResult);
          (*nbCharacter)++;
        }
        else
        {
          x = j;
          while (j < img->width && columnMarker[j] == 0)
            j++;
          if ((float)j - x > letterAverage / 1.5 && j < img->width &&
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

/**
 * \brief create a new bitmap with width offset 
 *
 * \param src the original bitmap
 */
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

/**
 * \brief create a new bitmap with height offset 
 *
 * \param src the orginal bitmap
 */
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

/**
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

/**
 * \brief check is a block is a block of text 
 *
 * \param histo is the hsitogram of a block 
 * \param hm is the average height of blocks
 */
char checkClass(histogram *histo, float* hm)
{
  float rm;
  if (histo->tc == 0)
    return 0; 
  rm = (float)histo->dc / histo->tc;
  return 2 < 10 * rm  && histo->deltaY < 3 * (unsigned)(*hm);
}

/**
 * \brief make the histogram of one block and add it in a queue
 *
 * \param bmp is the block 
 * \param original is copy binarize of the original image 
 * \param X higth left corner of the block on the copy in abscisse 
 * \param Y higth left corner of the block on the copy in ordonate
 * \param q is the queue where we add the hsitogram
 */
void makeHistogram(bitmap *bmp, bitmap *original, unsigned x, unsigned y,
    queue *q)
{
  unsigned dc = 0, tc = 0;
  histogram *histo = malloc(sizeof(histogram));
  histo->x = x;
  histo->y = y;
  histo->deltaX = bmp->width;
  histo->deltaY = bmp->height;
  for (unsigned i = 0; i < bmp->height; i++)
    for (unsigned j = 0; j < bmp->width; j++)
    {
      if (original->content[(i + y) * original->width + j + x].r == 0)
        dc++;
      if (original->content[(i + y) * original->width + j + x].r != 
          bmp->content[i * bmp->width + j].r)
        tc++;
    }
  histo->dc = dc;
  histo->tc = tc;
  enQueue(q, histo);
}

/**
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
  unsigned y, x;

  unsigned i = 0;
  while (i < src->height)
  {
    if (i < src->height && lineMarker[i] == 1)
    {
      y = i;
      while (i < src->height && lineMarker[i] == 1)
        i++;

      putColumnMarker(bmp, y, i, columnMarker);
      unsigned j = 0;
      while (j < src->width)
      {
        if (j < src->width && columnMarker[j] == 1)
        {
          x = j;
          while (j < src->width && columnMarker[j] == 1)
            j++;
         
          bitmap *bmpResult = cutBmp(src, x, y, j - x, i - y);
          makeHistogram(bmpResult, bmp, x, y, histoQueue);
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
  *hm = !nbLine ? 0 : (*hm) /nbLine;
  return histoQueue;
}

/**
 * \brief make image from a queue of histogram
 *
 * \param src is the original image
 * \histoQueue is the queue with contain all histograms
 * \hm is the average height of block in the image 
 */
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

/**
 * \brief return a image with line and draw
 *
 * \param src is the orignal image
 */
bitmap *rlsa(bitmap *src)
{
  bitmap *copy = binerizeCopy(src);
  bitmap *first = widthTravel(copy);
  bitmap *second = heightTravel(copy);
  bitmap *fusion = merge(first, second);

  float hm[1] = {0};
  queue *histoQueue = textToHisto(fusion, src, hm);
  bitmap *final = histoToImage(src, histoQueue, hm);

  freeBitmap(first);
  freeBitmap(second);
  freeBitmap(fusion);
  return final;
}
