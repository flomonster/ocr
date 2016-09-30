# include <stdio.h>
# include <stdlib.h>

struct S_COLOR
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};
typedef struct S_COLOR COLOR;

COLOR newColor(unsigned char r, unsigned char g, unsigned char b)
{
  COLOR col;
  col.r = r;
  col.g = g;
  col.b = b;
  return col;
}

struct S_BITMAP
{
  unsigned width;
  unsigned height;
  color *content;
};

typedef struct S_BITMAP BITMAP;

BITMAP newBitmap(unsigned int width, unsigned int height, color *content)
{
  BITMAP img;
  img.width = width;
  img.height = height;
  img.content = content;
  return img;
}

void draw(BITMAP *img)
{
  for (unsigned i = 0; i < img->width * img->height; i++)
  {
      if (i % img->width == 0 && i != 0)
        printf("\n");
      char c = img->content[i].r == 0 ? '#' : ' ';
      printf("%c", c); 
  }
  printf("\n");
}

void binarize(BITMAP *img)
{
  // TO DO
}

void resize(BITMAP *img)
{
  // TO DO
}

BITMAP loading(char *path)
{
  // TO DO
}
