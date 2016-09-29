#include <gtk/gtk.h>

struct s_network;
typedef struct s_network network;

char ocr(GtkImage*);

void save(char*, network);

network load(char*);
