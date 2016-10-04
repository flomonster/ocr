# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include "bitmap.h"


COLOR newColor(unsigned char r, unsigned char g, unsigned char b)
{
  COLOR col;
  col.r = r;
  col.g = g;
  col.b = b;
  return col;
}

BITMAP newBitmap(unsigned width, unsigned height, COLOR *content)
{
  BITMAP img;
  img.width = width;
  img.height = height;
  img.content = content;
  return img;
}

# pragma pack(push, 1)
struct S_BITMAPFILEHEADER
{
	uint16_t bfType;  
	uint32_t bfSize;  
	uint16_t bfReserved1;  
	uint16_t bfReserved2;  
	uint32_t bfOffBits;  
};
# pragma pack(pop)
typedef struct S_BITMAPFILEHEADER BITMAPFILEHEADER;

# pragma pack(push, 1)
struct S_BITMAPINFOHEADER
{
	uint32_t biSize;
	uint64_t biWidth;  
	uint64_t biHeight; 
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
typedef struct S_BITMAPINFOHEADER BITMAPINFOHEADER;

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

BITMAP loadBmp(char *path)
{
	FILE *filePtr;
	BITMAPINFOHEADER bitmapInfoHeader;
  BITMAPFILEHEADER bitmapFileHeader;
	BITMAP bitmap;
  filePtr = fopen(path, "rb");

  /*if (filePtr == NULL)
		return NULL;*/

  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	/*printf("%d \n", bitmapFileHeader.bfType);
	printf("%d \n", bitmapFileHeader.bfSize);
	printf("%d \n", bitmapFileHeader.bfReserved1);
	printf("%d \n", bitmapFileHeader.bfReserved2);
	printf("%d \n", bitmapFileHeader.bfOffBits);*/
  /*if (bitmapFileHeader.bfType !=0x4D42)
  {
		fclose(filePtr);
		return NULL;
  }*/

  fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr); 
	printf("%d \n", bitmapInfoHeader.biSize);
  printf("%d \n", bitmapInfoHeader.biWidth);
  printf("%d \n", bitmapInfoHeader.biHeight);
  printf("%d \n", bitmapInfoHeader.biPlanes);
  printf("%d \n", bitmapInfoHeader.biBitCount);
	printf("%d \n", bitmapInfoHeader.biCompression);
  printf("%d \n", bitmapInfoHeader.biSizeImage);
  printf("%d \n", bitmapInfoHeader.biXPelsPerMeter);
  printf("%d \n", bitmapInfoHeader.biYPelsPerMeter);
  printf("%d \n", bitmapInfoHeader.biClrUsed);
	printf("%d \n", bitmapInfoHeader.biClrImportant);






	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	bitmap.width = bitmapInfoHeader.biWidth;
	bitmap.height = bitmapInfoHeader.biHeight;
	bitmap.content = malloc(sizeof(COLOR) * (bitmap.width * bitmap.height));
	unsigned char *pixel = malloc(sizeof(char) * (bitmap.width * bitmap.height * 3)); 
  printf("%d, %d \n", (int)bitmap.height, (int)bitmap.width);
	printf("%d \n",sizeof(bitmap.content));
	printf("%d \n",sizeof(pixel));
	/*if (!pixel)
  {
    free(pixel);
		free(bitmap->content);
    fclose(filePtr);
    return NULL;
  }*/

  fread(pixel, bitmapInfoHeader.biSizeImage, 1, filePtr);

  /*if (pixel == NULL)
  {
    fclose(filePtr);
    return NULL;
  }*/
  for (unsigned i = 0; i < bitmapInfoHeader.biSizeImage; i+=3)
	{
		COLOR col = newColor(pixel[i+2], pixel[i+1], pixel[i]);
		//printf("%d, %d, %d \n", col.r, col.g, col.b);
		bitmap.content[i/3] = col;
	}
	free(pixel);
  fclose(filePtr);
	return bitmap;
}
