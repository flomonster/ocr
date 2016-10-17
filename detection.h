# ifndef DETECTION_H_DEFINED
# define DETECTION_H_DEFINED
# include <stdio.h>
# include <stdlib.h>
# include "bitmap.h"

struct s_element;
typedef struct s_element element;

struct s_queu;
typedef struct s_queu queu;

struct s_queuOfQueu;
typedef struct s_queuOfQueu queuOfQueu;

queu newQueu();

void enQueu(queu *, bitmap);

element deQueu(queu *);

# endif

