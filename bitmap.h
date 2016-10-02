# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>

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
  COLOR *content;
};

typedef struct S_BITMAP BITMAP;

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

BITMAP newBitmap(unsigned int width, unsigned int height, COLOR *content)
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

BITMAP loadBmp(char *path)
{
	FILE *filePtr;
	BITMAPINFOHEADER bitmapInfoHeader;
  BITMAPFILEHEADER bitmapFileHeader;
	BITMAP bitmap;
  filePtr = fopen(path, "rb");

  /*if (filePtr == NULL)
		return NULL;*/

  fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);

  /*if (bitmapFileHeader.bfType !=0x4D42)
  {
		fclose(filePtr);
		return NULL;
  }*/

  fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); 
  
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	bitmap.width = bitmapInfoHeader.biWidth;
	bitmap.height = bitmapInfoHeader.biHeight;
	bitmap.content = malloc((bitmapInfoHeader.biSizeImage)/3);
	unsigned char *pixel = malloc(bitmapInfoHeader.biSizeImage); 
  
	/*if (!pixel)
  {
    free(pixel);
		free(bitmap->content);
    fclose(filePtr);
    return NULL;
  }*/

  fread(pixel,bitmapInfoHeader.biSizeImage,1 ,filePtr);

  /*if (pixel == NULL)
  {
    fclose(filePtr);
    return NULL;
  }*/
  for (int i = 0, i < bitmapInfoHeader.biSizeImage, i+=3)
  {
		COLOR color = newColor(pixel[i+2], pixel[i+1], pixel[i]);
		bitmap.content[i/3] = color; 
  }

  fclose(filePtr);
	return bitmap;
}
