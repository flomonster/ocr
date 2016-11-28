# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <err.h>
# include "bitmap.h"

# pragma pack(push, 1)
struct s_bitmapFileHeader
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBytes;
};
# pragma pack(pop)
typedef struct s_bitmapFileHeader bitmapFileHeader;

# pragma pack(push, 1)
struct s_bitmapInfoHeader
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};
# pragma pack(pop)
typedef struct s_bitmapInfoHeader bitmapInfoHeader;

/**
 * \brief Create a new color with RGB as argument
 *
 * \param r red component of the new color
 * \param g green component of the new color
 * \param b blue component of the new color
 */
color newColor(unsigned char r, unsigned char g, unsigned char b)
{
  color col;
  col.r = r;
  col.g = g;
  col.b = b;
  return col;
}

/**
 * \brief Create a new bitmap with width, height and and color array of it
 *
 * \param width is the with of the new bitmap
 * \param height is the height of the new bitmap
 * \param content is the full image
 */
bitmap *newBitmap(unsigned width, unsigned height, color *content)
{
  bitmap *img = malloc(sizeof(bitmap));
  img->width = width;
  img->height = height;
  img->content = content;
  return img;
}

void freeBitmap(bitmap *img)
{
  free(img->content);
  free(img);
}

/**
 * \brief Draw an image in console
 *
 * \param img the image which will be draw
 */
 void draw(bitmap *img)
{
  printf("+");
  for (unsigned i = 0; i < img->width + 2; i++)
    printf("-");
  printf("+\n| ");
  for (unsigned i = 0; i < img->width * img->height; i++)
  {
		if (i % img->width == 0 && i != 0)
      printf(" |\n| ");
    char c = img->content[i].r == 0 ? '#' : ' ';
    printf("%c", c);
  }
  printf(" |\n+");
  for (unsigned i = 0; i < img->width + 2; i++)
    printf("-");
  printf("+\n");
}

/**
 * \brief put the image in black and white
 *
 * \param img the image which will be binarize
 */
void binarize(bitmap *img)
{
  unsigned short c = 0;
  for (unsigned i = 0; i < img->width * img->height; i++)
  {
    c = img->content[i].r + img->content[i].g + img->content[i].b;
    c = c > 384 ? 255 : 0;
    img->content[i].r = c;
    img->content[i].g = c;
    img->content[i].b = c;
  }
}

/**
 * \brief resize an image in 16x16 pixels
 *
 * \pram img the image which will be resize
 */
void resize(bitmap *img)
{
  unsigned newWidth = 16;
  unsigned newHeight = 16;
  unsigned pos, pos2;
  unsigned count;
  unsigned short sumR, sumG, sumB;
  float rWidth = (float) img->width / newWidth;
  float rHeight = (float) img->height / newHeight;
  color *cont = malloc(sizeof(color) * newWidth * newHeight);

  for (unsigned y = 0; y < newHeight; y++)
  {
    for (unsigned x = 0; x < newWidth; x++)
    {
      sumR = 0;
      sumG = 0;
      sumB = 0;
      count = 0;
      for (unsigned i = x * rWidth; i < x * rWidth + rWidth; i++)
        for (unsigned j = y * rHeight; j < y * rHeight + rHeight; j++)
        {
          pos = i + j * img->width;
          sumR += img->content[pos].r;
          sumG += img->content[pos].g;
          sumB += img->content[pos].b;
          count++;
        }
      pos2 = x + y * newWidth;
      cont[pos2].r = sumR / count;
      cont[pos2].g = sumG / count;
      cont[pos2].b = sumB / count;
    }
  }
  free(img->content);
  img->content = cont;
  img->width = newWidth;
  img->height = newHeight;
}

/**
 * \brief Load a bmp file with a path in argument
 *
 * \param path is image path
 */
bitmap *loadBmp(char *path)
{
  FILE *fp = fopen(path, "rb");
  bitmapFileHeader fileHeader;
  bitmapInfoHeader infoHeader;
  bitmap *bmp = malloc(sizeof(bitmap));

  fread(&fileHeader, sizeof(bitmapFileHeader), 1, fp);

  if (fileHeader.bfType != 0x4D42)
  {
    fclose(fp);
    errx(1, "The expected file is not a .bmp");
  }

  fread(&infoHeader, sizeof(bitmapInfoHeader), 1, fp);
  fseek(fp, fileHeader.bfOffBytes, SEEK_SET);

  bmp->width = infoHeader.biWidth;
  bmp->height = infoHeader.biHeight;
  bmp->content = malloc(sizeof(color) * bmp->width * bmp->height);

  int padding = 4 - (bmp->width * 3) % 4;
  if (padding == 4)
    padding = 0;

  color px;
  for (int i = bmp->height - 1; i >= 0; i--)
  {
    for (unsigned j = 0; j < bmp->width; j++)
    {
      fread(&px, 3, 1, fp);
      bmp->content[i * bmp->width + j] = px;
    }
    fseek(fp, padding, SEEK_CUR);
  }

  fclose(fp);
  return bmp;
}

/*
 * \brief Equalize the histogram of the bitmap to raise the contrast
 *
 * \param bitmap *img : the bitmap
 *
*/
void autoContrast(bitmap *img)
{
  unsigned *histoR = malloc(256 * sizeof(unsigned ));
  unsigned *histoG = malloc(256 * sizeof(unsigned ));
  unsigned *histoB = malloc(256 * sizeof(unsigned ));

  unsigned *histoRC = malloc(256 * sizeof(unsigned));
  unsigned *histoGC = malloc(256 * sizeof(unsigned));
  unsigned *histoBC = malloc(256 * sizeof(unsigned));

  unsigned cR = 0;
  unsigned cG = 0;
  unsigned cB = 0;

  unsigned j;  
  unsigned n = img->width * img->height;

  for (j = 0; j < 256; j++)
  {
    histoR[j] = 0;
    histoG[j] = 0;
    histoB[j] = 0;
  }
  for (j = 0; j < 256; j++)
  {
    histoRC[j] = 0;
    histoGC[j] = 0;
    histoBC[j] = 0;
  }
  for (j = 0; j < n; j++)
  {
    histoR[img->content[j].r]++;
    histoG[img->content[j].g]++;
    histoB[img->content[j].b]++;
  }

  for (j = 0; j < 256; j++) 
  {
    cR += histoR[j];
    histoRC[j] = cR;
    cG += histoG[j];
    histoGC[j] = cG;
    cB += histoB[j];
    histoBC[j] = cB;
  }
  
  for (j = 0; j < n; j++)
  {
    img->content[j].r = 255 * histoRC[img->content[j].r] / n;
    img->content[j].g = 255 * histoGC[img->content[j].g] / n;
    img->content[j].b = 255 * histoBC[img->content[j].b] / n;
  }

  free(histoR);
  free(histoG);
  free(histoB);
  free(histoRC);
  free(histoGC);
  free(histoBC);
}
