# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <math.h>
# include <err.h>
# include "bitmap.h"
# include "detection.h"
# include "network.h"
# include "ocr.h"
# include "queue.h"
# include "learning.h"

/**
 * \brief entrain a neural network with a batch of samples
 *
 * \param n the neural network
 * \param samples the inputs of samples
 * \param results the outputs of samples
 * \param nbSample the number of samples
 */
void learn(network *n, float **samples, float **results, unsigned nbSample,
    float speed, float goal)
{
  float error = 1;
  while (error > goal)
  {
    printf("  - STATUS : %d%%\n", (int) ((1 - error) / (1 - goal) * 100));
    for (int i = 0; i < 1000; i++)
      for (unsigned j = 0; j < nbSample; j++)
      {
        feedForward(n, samples[j]);
        backPropagation(n, results[j]);
        update(n, speed);
      }
    error = evaluate(n, samples, results, nbSample);
  }
  printf("  - STATUS : 100%%\n");
}

/**
 * \brief learn to the network "network.save"
 *
 * \param learnFiles the table of path to learn files
 * \param nbFile the number of file
 */
void learning(char *learnFiles[], size_t nbFile)
{
  network *n = loadNetwork("network.save");
  size_t length = 0;
  size_t *nbCharacter = malloc(sizeof(size_t));
  size_t *useless = malloc(sizeof(size_t));
  queue *segmented = newQueue();
  for (size_t j = 0; j < nbFile; j++)
  {
    char pathImg[100];
    size_t i = 0;

    FILE *fp = fopen(learnFiles[j] ,"r");
    do
    {
      fread(pathImg + i, 1, 1, fp);
      i++;
    } while (pathImg[i-1] != '\n');
    pathImg[i-1] = 0;
    fclose(fp);
    bitmap *img = loadBmp(pathImg);
    enQueue(segmented, segmentation(img, nbCharacter, useless));
    freeBitmap(img);
    length += *nbCharacter;
  }
  free(useless);
  free(nbCharacter);
  float **inputs = malloc(sizeof(float *) * length);
  int nbEnqueue = 0;
  for (size_t j = 0; j < nbFile; j++)
    nbEnqueue += createSamples((queue*) deQueue(segmented), inputs + nbEnqueue);
  free(segmented);

  char text[length + 1];
  size_t i = 0;
  for (size_t j = 0; j < nbFile; j++)
  {
    FILE *fp = fopen(learnFiles[j] ,"r");
    text[i] = ' ';
    while (text[i] != '\n')
      fread(text + i, 1, 1, fp);

    while (fread(text + i, 1, 1, fp))
    {
      if (text[i] != '\n' && text[i] != ' ')
        i++;
    }
    fclose(fp);
  }
  text[length] = 0;

  float **outputs = createResults(text, length);

  clock_t chrono = clock();
  printf("LEARNING :\n");
  learn(n, inputs, outputs, length, .2, .075);
  printf("  - Time : %.6f (seconds)\n", (clock() - chrono) / 1000000.0F);

  printf("  - Update of network.save\n");
  saveNetwork("network.save", n);

  printf("DONE\n");

  freeSamples(inputs, length);
  freeSamples(outputs, length);
  freeNetwork(n);
}

/**
 * \brief fill the samples with a segmented image and return the size
 *
 * \param text the image segmented 
 * \param samples the table of  sample to fill 
 */
int createSamples(queue *text, float **samples)
{
  float **origin = samples;
  while (text->length > 0)
  {
    queue *line = deQueue(text);
    while (line->length > 0)
    {
      queue *word = deQueue(line);
      while (word->length > 0)
      {
        bitmap *letter = deQueue(word);
        resize(letter);
        binarize(letter);
        *samples = malloc(sizeof(float) * 256);
        for (int i = 0; i < 256; i++)
          (*samples)[i] = letter->content[i].r;
        samples++;
      freeBitmap(letter);
      }
      free(word);
    }
    free(line);
  }
  return samples - origin;
}

/**
 * \brief Create the results of samples
 *
 * \param text the expected text
 * \param nbsample the length of the text
 * \param nbOutput the number of possibile outputs
 */
float **createResults(char text[], int nbSample)
{
  size_t nbOutput = 95;
  float **results = malloc(sizeof(float *) * nbSample);
  for (int i = 0; i < nbSample; i++)
  {
    if (text[i] == 0)
      errx(1, "The expected output is not valid");

    results[i] = malloc(sizeof(float) * nbOutput);
    for (size_t j = 0; j < nbOutput; j++)
      results[i][j] = 0;
    results[i][getCharIndex(text[i])] = 1;
  }
  return results;
}

/**
 * \brief free all components of a sample
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
