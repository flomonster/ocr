/**
 * \file bitmap.h
 * \brief header of bitmap.c
 * \author astain_d  and issarn_t
 * \date 10/01/2016
 */



# ifndef BITMAP_H_DEFINED
# define BITMAP_H_DEFINED
# include <stdio.h>
# include <stdlib.h>

# pragma pack(push, 1)
struct s_color
{
  unsigned char b;
  unsigned char g;
  unsigned char r;
};
# pragma pack(pop)
typedef struct s_color color;


/**
 * \brief Create a new color with RGB as argument
 *
 * \param r red component of the new color
 * \param g green component of the new color
 * \param b blue component of the new color
 */

color newColor(unsigned char, unsigned char, unsigned char);

# pragma pack(push, 1)
struct s_bitmap
{
  unsigned width;
  unsigned height;
  color *content;
};
# pragma pack(pop)

typedef struct s_bitmap bitmap;

/**
 * \brief Create a new bitmap with width, height and and color array of it
 *
 * \param width is the with of the new bitmap
 * \param height is the height of the new bitmap
 * \param content is the full image
 */

bitmap *newBitmap(unsigned, unsigned, color *);


/**
 * \brief free the bitmap
 *
 * \param bitmap *img the bitmap to free
 */

void freeBitmap(bitmap *);

struct s_bitmapFileHeader;
typedef struct s_bitmapFileHeader bitmapFileHeader;

struct s_bitmapInfoHeader;
typedef struct s_bitmapInfoHeader bitmapInfoHeader;

/**
 * \brief Draw an image in console
 *
 * \param img the image which will be draw
 */

void draw(bitmap *);

/**
 * \brief put the image in black and white
 *
 * \param img the image which will be binarize
 */

void binarize(bitmap *);

/**
 * \brief resize an image in 16x16 pixels
 *
 * \pram img the image which will be resize
 */

void resize(bitmap *);

/**
 * \brief Load a bmp file with a path in argument
 *
 * \param path is image path
 */

bitmap *loadBmp(char *);

/**
 * \brief save the bmp with the path name
 *
 * \param bitmap *img : the bitmap
 * \param char *path the name of the saved bmp
 */

void saveBmp(char *, bitmap *);

/**
 * \brief Equalize the histogram of the bitmap to raise the contrast
 *
 * \param bitmap *img : the bitmap
 */

void autoContrast(bitmap *img);

/**
 * \brief rotate the bmp by angle degree
 *
 * \param bitmap *ing the bmp to rotate
 * \param double angle the angle of the rotation, in degree
 */

void rotate(bitmap *img, double angle);
# endif
