#ifndef BITMAP_H_DEFINED
#define BITMAP_H_DEFINED
# include <stdio.h>
# include <stdlib.h>

#pragma pack(push, 1)
struct S_COLOR
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};
#pragma pack(pop)
typedef struct S_COLOR COLOR;

COLOR newColor(unsigned char, unsigned char, unsigned char);

#pragma pack(push, 1)
struct S_BITMAP
{
  unsigned width;
  unsigned height;
  COLOR *content;
};
#pragma pack(pop)
typedef struct S_BITMAP BITMAP;

BITMAP newBitmap(unsigned, unsigned, COLOR *);

struct S_BITMAPFILEHEADER;
typedef struct S_BITMAPFILEHEADER BITMAPFILEHEADER;

struct S_BITMAPINFOHEADER;
typedef struct S_BITMAPINFOHEADER BITMAPINFOHEADER;

void draw(BITMAP *img);

void binarize(BITMAP *img);

void resize(BITMAP *img);

BITMAP loadBmp(char *path);
#endif
