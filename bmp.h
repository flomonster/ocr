# include <stdio.h>
# include <stdlib.h>

struct s_color
{
  unsigned char r;
  unsigned char g;
  unsigned char b;
};
typedef struct s_color color;

color new_color(unsigned char r, unsigned char g, unsigned char b)
{
  color c;
  c.r = r;
  c.g = g;
  c.b = b;
  return c;
}

struct s_bmp
{
  unsigned width;
  unsigned height;
  color *bitmap;
};

typedef struct s_bmp bmp;

bmp new_bmp(unsigned int width, unsigned int height, color *bitmap)
{
  bmp img;
  img.width = width;
  img.height = height;
  img.bitmap = bitmap;
  return img;
}

void draw(bmp *img)
{
  for (unsigned i = 0; i < img->width * img->height; i++)
  {
      if (i % img->width == 0 && i != 0)
        printf("\n");
      char c = img->bitmap[i].r == 0 ? '#' : ' ';
      printf("%c", c); 
  }
  printf("\n");
}

void binarize(bmp *img)
{
  // TO DO
}

void resize(bmp *img)
{
  // TO DO
}

bmp loading(char *path)
{
  // TO DO
}
