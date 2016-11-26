# ifndef GRAPHICAL_H_INCLUDED
# define GRAPHICAL_H_INCLUDED

#include "ocr.h"
#include <gtk/gtk.h>
#include "bitmap.h"
#include "detection.h"
#include "queue.h"
#include "network.h"
#include "main_functions.h"

struct tuple{
GtkWidget *parent_window;
char *data;};
/* Save the output (data) in file located at path */
void save_to_file(char *path, char *data);

/* Launches the learning on the file located at path */
void launch_leaning(char *path);

/* Choose the file and then launch above function */
void learning(GtkWidget *parent_window);

/* Gets the path of a chosen file */
void get_path(struct tuple *tuple);

/* Build the interface and calls the functions whenever necessary */
int start(int argc, char **argv);

/* Launch a leave dialog confirmation */
void leave_dialog(GtkWidget *data);

/* Saves the file whereever the user has chosen */
void save_file(GtkWidget *parent_window, char *data);

# endif
