/**
 * \file bitmap.h
 * \brief header of bitmap.c
 * \author astain_d and issarn_t
 * \date 10/01/2016
 */



# ifndef BITMAP_H_DEFINED
# define BITMAP_H_DEFINED
# include <stdio.h>
# include <stdlib.h>

# pragma pack(push, 1)
/**
 * \struct s_color
 * \brief The color of a pixel in a bitmap
 */
struct s_color
{
  unsigned char b; /*!< Th blue color */
  unsigned char g; /*!< The green color */
  unsigned char r; /*!< The red color */
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
/**
 * \struct s_bitmap
 * \brief A picture
 */
struct s_bitmap
{
  unsigned width; /*!< The width of the picture */
  unsigned height; /*!< The height of the picture */
  color *content; /*!< The table of color */
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
 * \param img the bitmap to free
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
 * \param img the picture to save
 * \param path the name for the saved picture
 */
void saveBmp(char *, bitmap *);

/**
 * \brief equalize the histogram of the bitmap to raise the contrast
 *
 * \param img the bitmap to contrast
 */
void autoContrast(bitmap *);

/**
 * \brief rotate the bitmap by angle degree
 *
 * \param img the bitmap to rotate
 * \param angle the angle of the rotation, in degree
 */
void rotate(bitmap *, double);
# endif
