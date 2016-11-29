# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <time.h>
# include "ocr.h"
# include "detection.h"
# include "learning.h"

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
    generateNetwork(256, 80 , 95);

  else if (argv[1][0] == '1')
  {
    if (argc < 3)
      return 0;

    learning(argv + 2, argc - 2);
  }
  else if (argv[1][0] == '2')
  {
    if (argc != 3)
      return 0;
    network *n = loadNetwork("network.save");

    bitmap *img = loadBmp(argv[2]);
    //draw(img);
    size_t *length = malloc(sizeof(size_t));
    size_t *useless = malloc(sizeof(size_t));
    queue *q = segmentation(img, useless, length);
    int i = 0;
    char txt[*length + 1];
    txt[*length] = 0;
    while (q->length > 0)
    {
      queue *line = deQueue(q);
      while (line->length > 0)
      {
        queue *word = deQueue(line);
        while (word->length > 0)
        {
          bitmap *letter = deQueue(word);
          resize(letter);
          autoContrast(letter);
          binarize(letter);
          draw(letter);
          txt[i] = ocr(letter, n);
          freeBitmap(letter);
          printf("%c\n", txt[i]);
          i++;
        }
        free(word);
        txt[i] = ' ';
        i++;
      }
      free(line);
      txt[i] = '\n';
      i++;
    }
    printf("--------------------\n%s--------------------\n", txt);
    free(useless);
    free(length);
    free(q);
    freeBitmap(img);
    freeNetwork(n);
  }
  return 0;
}
