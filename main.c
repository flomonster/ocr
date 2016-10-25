# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <time.h>
# include "ocr.h"
# include "detection.h"

float **createSamples(char *path, int *nbSample)
{
  bitmap *img = loadBmp(path);
  queue *text = segmentation(img);
  *nbSample = 0;
  element *el = text->first;
  for (int i = 0; i < text->length; i++)
  {
    queue *q = el->obj;
    *nbSample += q->length;
    el = el->next;
  }

  float **samples = malloc(sizeof(float *) * *nbSample);
  for (int i = 0; i < *nbSample; i++)
    samples[i] = malloc(sizeof(float) * 256);

  int sample = 0;
  while (text->length > 0)
  {
    queue *line = deQueue(text);
    while (line->length > 0)
    {
      bitmap *lettre = deQueue(line);
      resize(lettre);
      binarize(lettre);
      for (int i = 0; i < 256; i++)
        samples[sample][i] = lettre->content[i].r;
      sample++;
      freeBitmap(lettre);
    }
    free(line);
  }
  free(text);
  freeBitmap(img);
  return samples;
}

float **createResults(char *text, int nbSample)
{
  int nbOutput = 2;

  float **results = malloc(sizeof(float *) * nbSample);
  for (int i = 0; i < nbSample; i++)
  {
    if (text[i] == 0)
      errx(1, "The expected output is not valid");

    results[i] = malloc(sizeof(float) * nbOutput);
    for (int j = 0; j < nbOutput; j++)
      results[i][j] = 0;
    results[i][getCharIndex(text[i])] = 1;
  }
  return results;
}

void freeSamples(float **samples, int nbSample)
{
  for (int i = 0; i < nbSample; i++)
    free(samples[i]);
  free(samples);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
    return 0;
  if (argv[1][0] == '0')
  {
    if (argc != 4)
      return 0;

    unsigned *layers = malloc(sizeof(unsigned) * 3);
    layers[0] = 256;
    layers[1] = 15;
    layers[2] = 2;
    network *n = newNetwork(3, layers);
    int *nbSample = malloc(sizeof(int));
    float **inputs = createSamples(argv[2], nbSample);
    float **outputs = createResults(argv[3], *nbSample);
    clock_t chrono = clock();
    learn(n, inputs, outputs, *nbSample, .5, .1);
    printf("LEARNING :\n");
    printf("  - Time : %.6f (seconds)\n", (clock() - chrono) / 1000000.0F);
    
    printf("  - Creation of network.save\n");
    saveNetwork("network.save", n);

    printf("  - DONE\n");

    freeSamples(inputs, *nbSample);
    freeSamples(outputs, *nbSample);
    free(nbSample);
    freeNetwork(n);
  }
  else if (argv[1][0] == '1')
  {
    if (argc != 3)
      return 0;
    network *n = loadNetwork("network.save");
    char txt[256];
    int i = 0;

    bitmap *img = loadBmp(argv[2]);
    draw(img);
    queue *q = segmentation(img);
    while (q->length > 0)
    {
      queue *ligne = deQueue(q);  
      while (ligne->length > 0)
      {
        bitmap *lettre = deQueue(ligne);
        resize(lettre);
        binarize(lettre);
        draw(lettre);
        txt[i] = ocr(lettre, n);
        freeBitmap(lettre);
        printf("%c\n", txt[i]);
        i++;
      }
      txt[i] = '\n';
      i++;
    }
    printf("--------------------\n");
    printf("%s", txt);
    printf("--------------------\n");
    freeBitmap(img);
    freeNetwork(n);
  }
  return 0;
}
