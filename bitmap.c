# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include "bitmap.h"


color newColor(unsigned char r, unsigned char g, unsigned char b)
{
  color col;
  col.r = r;
  col.g = g;
  col.b = b;
  return col;
}

bitmap newBitmap(unsigned width, unsigned height, color *content)
{
  bitmap img;
  img.width = width;
  img.height = height;
  img.content = content;
  return img;
}

# pragma pack(push, 1)
struct s_bitmapFileHeader
{
	uint16_t bfType;  
	uint32_t bfSize;  
	uint16_t bfReserved1;  
	uint16_t bfReserved2;  
	uint32_t bfOffBits;  
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
	uint64_t biXPelsPerMeter;
	uint64_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};
# pragma pack(pop)
typedef struct s_bitmapInfoHeader bitmapInfoHeader;

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

/*void binarize(bitmap *img)
{
  // TO DO
}

void resize(bitmap *img)
{
  // TO DO
}*/

bitmap loadBmp(char *path)
{
	FILE *fp = fopen(path, "rb");
	bitmapFileHeader fileHeader;
	bitmapInfoHeader infoHeader;
	bitmap bmp;
	
	fread(&fileHeader, sizeof(bitmapFileHeader), 1, fp);
	fread(&infoHeader, sizeof(bitmapInfoHeader), 1, fp);
	fseek(fp, fileHeader.bfOffBits, SEEK_SET);

	bmp.width = infoHeader.biWidth;
	bmp.height = infoHeader.biHeight;
	bmp.content = malloc(sizeof(color) * bmp.width * bmp.height);
	unsigned char *pixel = malloc(3 * bmp.width * bmp.height);
	fread(pixel, 3 * bmp.width * bmp.height, 1, fp);

	for (unsigned i = 0; i < 3 * bmp.width * bmp.height; i+=3)
	{
		color col = newColor(pixel[i+2], pixel[i+1], pixel[i]);
		bmp.content[i/3] = col;
	}
	
	fclose(fp);	
	return bmp;
}
