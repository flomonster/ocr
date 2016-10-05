# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include "bitmap.h"
# include <err.h>

// Create a new color with RGB as argument 
color newColor(unsigned char r, unsigned char g, unsigned char b)
{
  color col;
  col.r = r;
  col.g = g;
  col.b = b;
  return col;
}

// Create a new bitmap with width, height and and color array of it
bitmap newBitmap(unsigned width, unsigned height, color *content)
{
  bitmap img;
  img.width = width;
  img.height = height;
  img.content = content;
  return img;
}

void freeBitmap(bitmap *img)
{
  free(img->content);
}

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

// Print a # for each pixel with R == 0 in a bitmap in argument
void draw(bitmap *img)
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

void binarize(bitmap *img)
{
  // TO DO
}

void resize(bitmap *img)
{
  // TO DO
}

// Load a bmp file with a path in argument 
bitmap loadBmp(char *path)
{
	FILE *fp = fopen(path, "rb");
	bitmapFileHeader fileHeader;
	bitmapInfoHeader infoHeader;
	bitmap bmp;
	
	fread(&fileHeader, sizeof(bitmapFileHeader), 1, fp);
	fread(&infoHeader, sizeof(bitmapInfoHeader), 1, fp);
	fseek(fp, fileHeader.bfOffBytes, SEEK_SET);
	
  bmp.width = infoHeader.biWidth;
	bmp.height = infoHeader.biHeight;
	bmp.content = malloc(sizeof(color) * bmp.width * bmp.height);
  
  color px;
  for (int i = bmp.height - 1; i >= 0; i--)
  {
    for (unsigned j = 0; j < bmp.width; j++)
    {
	    fread(&px, 3, 1, fp);
      bmp.content[i * bmp.width + j] = px;
    }
    fseek(fp, 2, SEEK_CUR);
  } 
	
	fclose(fp);	
	return bmp;
}