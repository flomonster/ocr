/**
 * \file detection.c
 * \brief Detect text and only it in a text 
 * \author issarn_t
 * \date 09/17/2016
 */
# include <err.h>
# include "bitmap.h"
# include "queue.h"
# include "detection.h"

/**
 * \struct s_histogram
 * \brief All information on a text block is in
 */
struct s_histogram
{
  unsigned x; /*!< min x pos of the block */
  unsigned y; /*!< min y pos of the block */
  unsigned deltaX; /*!< width of the block */
  unsigned deltaY; /*!< height of the block */
  unsigned dc; /*!< number of black pixel in the block */
  unsigned tc; /*!< number of dif between the original and the rlsa block */
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
 * \param pos the x min pos in the original image
 * \param width the width of the cuted Image
 */
void putLineMarker(bitmap *img, char *array, int pos, unsigned width)
{
  for(unsigned i = 0; i < img->height; i++)
  {
    unsigned j = pos;
    while (j < pos + width && img->content[i * img->width + j].r)
      j++;
    array[i] = j == pos + width ? 0 : 1;
  }
}

/**
 * \brief Put a marker for each column with a letter
 *
 * \param img one line of the full image
 * \param min x min
 * \param max x max
 * \param array where marker is put in function of the img 
 * \param pos the x min pos in the original image
 * \param width the width of the cuted Image
 */
void putColumnMarker(bitmap *img, unsigned min, unsigned max, char *array,
    int pos, unsigned width)
{
  for (unsigned i = pos; i < pos + width; i++)
  {
    unsigned j = min;
    while (j < max && img->content[j * img->width + i].r)
      j++;
    array[i] = j == max ? 0 : 1;
  }
}


/**
 * \brief Put a marker for each column with a letter
 *
 * \param img the full image  
 * \param min x min
 * \param max y min
 * \param width the width of the cuted Image
 * \param height the height of the image 
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
float letterAverage(char *columnMarker, unsigned width)
{
  float nbLetter = 0;
  float sumWidthLetter = 0;

  unsigned i = 0;
  while (i < width && !columnMarker[i])
      i++;
  while (i < width)
  {
    while (i < width && columnMarker[i])
    {
      sumWidthLetter++;
      i++;
    }
    nbLetter++;
    while (i < width && !columnMarker[i])
      i++;
  }
  return !nbLetter ? 0 : sumWidthLetter / nbLetter;
}

/**
 * \brief frame a rectangle
 *
 * \param img the full image  
 * \param min x min
 * \param max y min
 * \param width the width of the rectangle
 * \param height the height of the rectangle
 * */
void colorRectangle(bitmap *src, int x, int y, unsigned width, 
    unsigned height)
{
  color pixel = newColor(0, 0, 255);
  for (unsigned i = y; i < y + height && i < src->height; i++)
  {
    if ((int)i == y || (int)i == (int)height + y - 1)
      for (unsigned j = x + 1; j < x + width - 1 && 
          j < src->width; j++)
        src->content[i * src->width + j] = pixel;
    if (x > 0)  
      src->content[i * src->width + x] = pixel;
    if (x + (int)width - 1 < (int)src->width)
      src->content[i * src->width + x + width - 1] = pixel;
  }
}

/**
 * \brief Create a queue with all letter in a bitmap
 *
 * \param img the full image
 * \param nbCharacter the number of character in the image
 * \param nbLetter the number of letter int the image
 * \param q is the queue where is stocked all letter 
 * \param cutedImage is the the image that we use 
 * \param pos is the x min
 */
void segmentation(bitmap *img, size_t *nbCharacter,
    size_t *nbLetter, queue *q, bitmap *cutedImage, int pos)
{
  *nbCharacter = 0;
  *nbLetter = 0;
  bitmap *bmp = binerizeCopy(img);
  char lineMarker[cutedImage->height];
  char columnMarker[cutedImage->width];
  float widthAverage;
  unsigned y, x;

  putLineMarker(bmp, lineMarker, pos, cutedImage->width);
  float heightAverage = letterAverage(lineMarker, cutedImage->height);
  queue *paragraph = newQueue();

  unsigned i = 0;
  while (i < cutedImage->height)
  { 
    if (i < cutedImage->height && lineMarker[i])
    {
      y = i;
      while (i < cutedImage->height && lineMarker[i])
        i++;

      putColumnMarker(bmp, y, i, columnMarker, pos, cutedImage->width);
      widthAverage = letterAverage(columnMarker, cutedImage->width);
      queue *word = newQueue();

      unsigned j = pos;
      while (j < cutedImage->width + pos)
      {
        if (j < cutedImage->width + pos && columnMarker[j] == 1)
        {
          x = j;
          while (j < cutedImage->width + pos && columnMarker[j] == 1)
            j++;

          bitmap *bmpResult = cutBmp(img, x, y, j - x, i - y);
          colorRectangle(img, x - 1, y - 1, j - x + 2, i - y + 2);
          enQueue(word, bmpResult);
          (*nbCharacter)++;
        }
        else
        {
          x = j;
          while (j < cutedImage->width + pos && !columnMarker[j])
            j++;
          if ((float)j - x > widthAverage && j < cutedImage->width + pos && 
              word->length )
          {
            enQueue(paragraph, word);
            word = newQueue();
            (*nbLetter)++;
          }
          j++;
        }
      }
      enQueue(paragraph, word);
      (*nbLetter)++;
    }
    else 
    {
      y = i;
      while (i < cutedImage->height && !lineMarker[i])
        i++;
      if ((float)i - y > heightAverage && i < cutedImage->height && 
          paragraph->length)
      {
        enQueue(q, paragraph);
        paragraph = newQueue();
        (*nbLetter)++;
      }
      i++;
    }
  }
  enQueue(q, paragraph);
  (*nbLetter)++;
  *nbLetter += *nbCharacter;
  freeBitmap(bmp);
  saveBmp("segmented.bmp", img);
}

/**
 * \brief create a new bitmap with width offset 
 *
 * \param src the original bitmap
 */
bitmap *widthTravel(bitmap *src)
{
  int step = 25;
  color *content = malloc(sizeof(color) * src->width * src->height);
  for (unsigned i = 0; i < src->height * src->width; i++)
    content[i] = newColor(255, 255, 255);

  for (unsigned i = 0; i < src->height; i++)
  {
    int j = 0, min = 0, max = 0, terminal;
    while (j < (int)src->width)
    {
      if (!src->content[i * src->width + j].r)
      {
        int k = j;
        terminal = j - step < min ? min : j - step;
        while (k > terminal)
        {
          content[i * src->width + k] = newColor(0, 0, 0);
          k--;
        }

        k = max < j ? j : max;
        terminal = j + step > (int)src->width ? (int)src->width : j + step;
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
  int step = 50;
  color *content = malloc(sizeof(color) * src->width * src->height);
  for (unsigned i = 0; i < src->width * src->height; i++)
    content[i] = newColor(255, 255, 255);

  for (unsigned i = 0; i < src->width; i++)
  {
    int j = 0, min = 0, max = 0, terminal;
    while (j < (int)src->height)
    {
      if (!src->content[j * src->width + i].r)
      {
        int k = j;
        terminal = j - step < min ? min : j - step;
        while (k > terminal)
        {
          content[k * src->width + i] = newColor(0, 0, 0);
          k--;
        }

        k = max < j ? j : max;
        terminal = j + step > (int)src->height ? (int)src->height : j + step;
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
  return 2 > rm  && histo->deltaY < 3 * (unsigned)(*hm);
}

void columnCut(bitmap *src, queue *imgQueue, queue *posQueue)
{
  char columnMarker[src->width];
  putColumnMarker(src, 0, src->height, columnMarker, 0, src->width);
  unsigned x = 0;

  unsigned j = 0;
  while (j < src->width)
  {
    if (j < src->width && columnMarker[j])
    {
      x = j;
      while (j < src->width && columnMarker[j])
        j++;
         
      bitmap *bmpResult = cutBmp(src, x, 0, j - x, src->height);
      enQueue(imgQueue, bmpResult);
      unsigned *pos = malloc(sizeof(unsigned));
      *pos = x;
      enQueue(posQueue, pos);
    } 
    else
      while (j < src->width && columnMarker[j] == 0)
        j++;
  }
}

/**
 * \brief make the histogram of one block and add it in a queue
 *
 * \param bmp is the text block  
 * \param original is copy binarize of the original image 
 * \param x higth left corner of the block on the copy in abscisse 
 * \param y higth left corner of the block on the copy in ordonate
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
      if (!original->content[(i + y) * original->width + j + x].r)
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
 * \param histoQueue is the queue with all histogram of a image
 * \param src the cuted image
 * \param original is the original image
 * \param hm is the height average of text block
 * \param pos is the x min pos 
 * */
void textToHisto(queue *histoQueue, bitmap *src, bitmap *original,
    float *hm, unsigned pos) 
{
  unsigned nbLine = 0;
  bitmap *bmp = binerizeCopy(original); 
  char lineMarker[src->height];
  char columnMarker[src->width];
  putLineMarker(src, lineMarker, 0, src->width); 
  unsigned y, x;

  unsigned i = 0;
  while (i < src->height)
  {
    if (i < src->height && lineMarker[i])
    {
      y = i;
      while (i < src->height && lineMarker[i])
        i++;

      putColumnMarker(src, y, i, columnMarker, 0, src->width);
      unsigned j = 0;
      while (j < src->width)
      {
        if (j < src->width && columnMarker[j])
        {
          x = j;
          while (j < src->width && columnMarker[j])
            j++;
         
          bitmap *bmpResult = cutBmp(src, x + pos, y, j - x, i - y);
          makeHistogram(bmpResult, bmp, x + pos, y, histoQueue);
          nbLine++;
          *hm += bmpResult->height;
          freeBitmap(bmpResult);
        } 
        else
          while (j < src->width && !columnMarker[j])
            j++;
      }
    }
    else 
      while (i < src->height && !lineMarker[i])
        i++;
  }
  freeBitmap(bmp);
  *hm = !nbLine ? 0 : (*hm) /nbLine;
}

/**
 * \brief make image from a queue of histogram
 *
 * \param final is the image that we modify
 * \param src is the original image
 * \param histoQueue is the queue with contain all histograms
 * \param hm is the average height of block in the image
 */
void histoToImage(bitmap *final, bitmap *src, queue *histoQueue, float *hm)
{
  while (histoQueue->length > 0)
  {
    histogram *histo = deQueue(histoQueue);
    if (checkClass(histo, hm))
    {
      for (unsigned i = histo->y; i < histo->y + histo->deltaY; i++)
        for (unsigned j = histo->x; j < histo->x + histo->deltaX; j++)
          final->content[i * src->width + j] = src->content[i * src->width +j];
    }
    free(histo);
  }
}

/**
 * \brief return a image without line and draw
 *
 * \param src is the original image
 * \param imgQueue is the queue where we stock images
 * \param posQueue is the queue where we stock x min position of each images
 */
bitmap *rlsa(bitmap *src, queue *imgQueue, queue *posQueue)
{
  bitmap *copy = binerizeCopy(src);
  bitmap *first = widthTravel(copy);
  bitmap *second = heightTravel(copy);
  bitmap *fusion = merge(first, second);
  saveBmp("fusion.bmp", fusion);

  queue *histoQueue = newQueue();
  color *content = malloc(sizeof(color) * src->width * src->height);
  for (unsigned i = 0; i < src->width * src->height; i++)
    content[i] = newColor(255, 255, 255);
  bitmap *final = newBitmap(src->width, src->height, content);

  columnCut(fusion, imgQueue, posQueue);
  while (imgQueue->length)
  {
    bitmap *cutedImage = deQueue(imgQueue);
    unsigned *pos = deQueue(posQueue);
    float hm[1] = {0};
    textToHisto(histoQueue, cutedImage, src, hm, *pos);
    histoToImage(final, src, histoQueue, hm);
    freeBitmap(cutedImage);
    free(pos);
  }
  free(histoQueue);
  freeBitmap(copy);
  freeBitmap(first);
  freeBitmap(second);
  freeBitmap(fusion);
  return final;
}

/**
 * \brief return a queue which contain all letter 
 *
 * \param src is the original image
 * \param nbCharacter is the number of character
 * \param nbLetter is the number of letter
 */
queue *detectText(bitmap * src, size_t * nbCharacter, size_t * nbLetter)
{
  queue *posQueue = newQueue();
  queue *imgQueue = newQueue();
  bitmap *rlsaImage = rlsa(src, imgQueue, posQueue);
  saveBmp("rlsaImage.bmp", rlsaImage);
  *nbLetter = 0;
  *nbCharacter = 0;

  queue *q = newQueue();
  segmentation(rlsaImage, nbCharacter, nbLetter, q, rlsaImage, 0);

  free(posQueue);
  free(imgQueue);
  freeBitmap(rlsaImage);
  return q;
}
