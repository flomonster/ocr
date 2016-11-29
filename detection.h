# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"
# include "queue.h"

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
bitmap *binerizeCopy(bitmap *);

/**
 * \brief Put a marker for each line with a letter
 *
 * \param img the full image
 * \param array where marker is put in function of the img
 */
void putLineMarker(bitmap *, char *);

/**
 * \brief Put a marker for each column with a letter
 *
 * \param img one line of the full image
 * \param min X min
 * \param max X max
 * \param array where marker is put in function of the img *
 */
void putColumnMarker(bitmap *, unsigned, unsigned, char *);

/**
 * \brief Put a marker for each column with a letter
 *
 * \param img one line of the full image
 * \param array where marker is put in function of the img *
 */
void checBlackLine(bitmap *, char *);

/**
 * \brief check if there is a black pixel for the rlsa 
 *
 * \param img one line of the full image
 * \param array where marker is put in function of the img *
 * \param min X min 
 * \param max X max
 */
void checBlackColumn(bitmap *, char *, unsigned, unsigned);

/**
 * \brief Create a bitmap from an other
 *
 * \param img full img
 * \param X min x
 * \param Y min y
 * \param width the width of the new bitmap
 * \param height the height of the new bitmap
 */
bitmap *cutBmp(bitmap *, unsigned, unsigned, unsigned, unsigned);

/**
 * \brief find the length average of letters for one line
 *
 * \param columnMarker array where there are marker for each pixel
 * \param width is the width of the columnMarker
 */
float letterLengthAverage(char *, unsigned);

/**
 * \brief find the height average of a line
 *
 * \param lineMarker array where there are marker for each pixel
 * \param height is the height of the lineMarker
 */
float lineHeigthAverage(char *, unsigned);

/**
 * \brief Create a queue with all letter in a bitmap
 *
 * \param img the full image
 */
queue *segmentation(bitmap *, size_t *, size_t *);

/**
 * \brief create a new bitmap with width offset 
 *
 * \param src the original bitmap
 */
bitmap *widthTravel(bitmap *);

/**
 * \brief create a new bitmap with height offset 
 *
 * \param src the orginal bitmap
 */
bitmap *heightTravel(bitmap *);

/**
 * \brief Fusion two bitmap in one other 
 *
 * \param src1 the first bitmap 
 * \param src2 the second bitmap
 */
bitmap *merge(bitmap *, bitmap *);

/**
 * \brief check is a block is a block of text 
 *
 * \param histo is the hsitogram of a block 
 * \param hm is the average height of blocks
 */
char checkClass(histogram *, float *);

/**
 * \brief make the histogram of one block and add it in a queue
 *
 * \param bmp is the block 
 * \param original is copy binarize of the original image 
 * \param X higth left corner of the block on the copy in abscisse 
 * \param Y higth left corner of the block on the copy in ordonate
 * \param q is the queue where we add the hsitogram
 */
void makeHistogram(bitmap *, bitmap *, unsigned, unsigned, queue *);

/**
 * \brief Return an image without black line et draw
 *
 * \param src the original image
 */
queue *textToHisto(bitmap *, bitmap *, float *);

/**
 * \brief make image from a queue of histogram
 *
 * \param src is the original image
 * \histoQueue is the queue with contain all histograms
 * \hm is the average height of block in the image 
 */
bitmap *histoToImage(bitmap *, queue *, float *);

/**
 * \brief return a image with line and draw
 *
 * \param src is the orignal image
 */
bitmap *rlsa(bitmap *);

# endif

