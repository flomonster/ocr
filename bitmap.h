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

bitmap newBitmap(unsigned, unsigned, color *);

void freeBitmap(bitmap *);

struct s_bitmapFileHeader;
typedef struct s_bitmapFileHeader bitmapFileHeader;

struct s_bitmapInfoHeader;
typedef struct s_bitmapInfoHeader bitmapInfoHeader;

void draw(bitmap *img);

void binarize(bitmap *img);

void resize(bitmap *img);

bitmap loadBmp(char *path);
# endif
