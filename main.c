# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <time.h>
# include "ocr.h"
# include "detection.h"

/**
 * \brief Create samples from an image
 *
 * \param path location of the image
 * \param nbSample the number of character in the image
 */
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
      bitmap *letter = deQueue(line);
      resize(letter);
      binarize(letter);
      for (int i = 0; i < 256; i++)
        samples[sample][i] = letter->content[i].r;
      sample++;
      freeBitmap(letter);
    }
    free(line);
  }
  free(text);
  freeBitmap(img);
  return samples;
}

/**
 * \brief Create the results of samples
 *
 * \param text the expected text
 * \param nbsample the length of the text
 * \param nbOutput the number of possibile outputs
 */
float **createResults(char *text, int nbSample, int nbOutput)
{
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

/**
 * \brief free all components of an exemple
 *
 * \param samples the inputs or outputs of a sample
 * \param nbSample the number of character in the sample
 */
void freeSamples(float **samples, int nbSample)
{
  for (int i = 0; i < nbSample; i++)
    free(samples[i]);
  free(samples);
}

/**
 * \brief the main function
 *
 * \param argc the number of params
 * \param argv a table of string
 */
int main(int argc, char *argv[])
{
  if (argc < 2)
    return 0;
  if (argv[1][0] == '0')
  {
    unsigned *layers = malloc(sizeof(unsigned) * 3);
    layers[0] = 256;
    layers[1] = 60;
    layers[2] = 66;
    network *n = newNetwork(3, layers);
    
    printf("Generating the network :\n");
    printf("  - Layer size : %d | %d | %d\n", layers[0], layers[1], layers[2]);
    printf("  - Creation of network.save\n");
    saveNetwork("network.save", n);
    printf("DONE\n");

    freeNetwork(n);
  }
  else if (argv[1][0] == '1')
  {
    if (argc != 3)
      return 0;

    char pathImg[100];
    char text[1000];
    size_t i = 0;

    FILE *fp = fopen(argv[2] ,"r");
    do 
    {
      fread(pathImg + i, 1, 1, fp);
      i++;
    } while (pathImg[i-1] != '\n');
    pathImg[i-1] = 0;
    i = 0;

    do
    {
      fread(text + i, 1, 1, fp);
      i++;
    } while (text[i-1] != '\0');

    fclose(fp);

    network *n = loadNetwork("network.save");
    int *nbSample = malloc(sizeof(int));
    float **inputs = createSamples(pathImg, nbSample);
    int nbOutput = n->layers[n->nblayer - 1];
    float **outputs = createResults(text, *nbSample, nbOutput);
    clock_t chrono = clock();
    printf("LEARNING :\n");
    learn(n, inputs, outputs, *nbSample, .2, .075);
    printf("  - Time : %.6f (seconds)\n", (clock() - chrono) / 1000000.0F);
    
    printf("  - Update of network.save\n");
    saveNetwork("network.save", n);

    printf("DONE\n");

    freeSamples(inputs, *nbSample);
    freeSamples(outputs, *nbSample);
    free(nbSample);
    freeNetwork(n);
  }
  else if (argv[1][0] == '2')
  {
    if (argc != 3)
      return 0;
    network *n = loadNetwork("network.save");

    bitmap *img = loadBmp(argv[2]);
    draw(img);
    queue *q = segmentation(img);
    element *el = q->first;
    int i = 0;
    for (int j = 0; j < q->length; j++)
    {
      queue *q2 = el->obj;
      i += q2->length + 1;
      el = el->next;
    }
    char txt[i + 1];
    txt[i] = 0;
    i = 0;
    while (q->length > 0)
    {
      queue *line = deQueue(q);  
      while (line->length > 0)
      {
        bitmap *letter = deQueue(line);
        resize(letter);
        binarize(letter);
        draw(letter);
        txt[i] = ocr(letter, n);
        freeBitmap(letter);
        printf("%c\n", txt[i]);
        i++;
      }
      free(line);
      txt[i] = '\n';
      i++;
    }
    printf("--------------------\n%s--------------------\n", txt);
    free(q);
    freeBitmap(img);
    freeNetwork(n);
  }
  return 0;
}
