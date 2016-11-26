# ifndef GRAPHICAL_H_INCLUDED
# define GRAPHICAL_H_INCLUDED

#include "ocr.h"
#include <gtk/gtk.h>
#include "bitmap.h"
#include "detection.h"
#include "queue.h"
#include "network.h"
#include "main_functions.h"

typedef struct {
	GtkWidget *pWindow;
	GtkWidget *image;
	GtkWidget *text;
}Zone;
/* Save the output (data) in file located at path */
void saveFile(GtkWidget *widget, gpointer data);


/* Gets the path of a chosen file */
void cbOpen(GtkWidget *widget, gpointer data);

/* Build the interface and calls the functions whenever necessary */
int start(int argc, char **argv);

/* Launch a leave dialog confirmation */
void leaveDialog(GtkWidget *data);

/* Saves the file whereever the user has chosen */
void saveFile(GtkWidget *parent_window, char *data);

# endif
