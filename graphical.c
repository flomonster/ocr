#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "ocr.h"

/* Create a dialog that allows the user to choose a file to open.
Once the file is open, launch the ocr algorithm on it */
void get_path(GtkWidget *parent_window)
{
	char *filename;
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;
	dialog = gtk_file_chooser_dialog_new ("Open File",
                                      GTK_WINDOW(parent_window),
                                      action,
                                      "_Cancel",
                                      GTK_RESPONSE_CANCEL,
                                      "_Open",
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
  {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    filename = gtk_file_chooser_get_filename(chooser);
  }
	gtk_widget_destroy (dialog);
	//FIX ME (remplacer par l'appel à la fonction avec filename)
	g_free(filename);
}

/* Create a dialog that make the user confirm if he really 
wants to quit the program*/
void leave_dialog()
{
	GtkWidget *pWindow;
  GtkWidget *pQuestion;
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);

  pQuestion = gtk_message_dialog_new(GTK_WINDOW(pWindow),
  															     GTK_RESPONSE_ACCEPT,
														         GTK_MESSAGE_QUESTION,
														         GTK_BUTTONS_YES_NO,
        		"Do you really want to\nleave this program?");
  /* Waiting for an Answer */
  switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
  {
    case GTK_RESPONSE_YES:
  	      /* Yes -> program killed */
          gtk_main_quit();
          break; 
    case GTK_RESPONSE_NO:
          /* No -> dialog destroyed*/
          gtk_widget_destroy(pQuestion);
          break;
    }
}

int start(int argc, char **argv)
{
	GtkWidget *pWindow;
  GtkWidget *pGrid;
  GtkWidget *pButton[4];
 
  gtk_init(&argc, &argv);
	/* Generate and configure the window */
  pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
  gtk_window_set_title(GTK_WINDOW(pWindow), "OCR Project");
  g_signal_connect(G_OBJECT(pWindow), "destroy", 
									 G_CALLBACK(gtk_main_quit), NULL);
 
  /* Creation et insertion de la table 3 lignes 2 colonnes */
  pGrid = gtk_grid_new();
	for(int i = 0; i < 50; i++)
	{
		gtk_grid_insert_row(GTK_GRID(pGrid), 0);
		gtk_grid_insert_column(GTK_GRID(pGrid), 0);
	}
	for(int i = 0; i < 50; i++)
  { 
    gtk_grid_insert_row(GTK_GRID(pGrid), 0);
  }
  gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pGrid));
	gtk_grid_set_row_spacing (GTK_GRID(pGrid), 10); 
  gtk_grid_set_column_spacing (GTK_GRID(pGrid), 100);
  /* Create buttons */
  pButton[0]= gtk_button_new_with_mnemonic("_Quit");
  pButton[1]= gtk_button_new_with_mnemonic("_Open a file");
  pButton[2]= gtk_button_new_with_mnemonic("_Launch Program");
 
  /* Insert buttons */
  gtk_grid_attach(GTK_GRID(pGrid), pButton[0],
      						560, 0, 3, 10);
  gtk_grid_attach(GTK_GRID(pGrid), pButton[1],
      						0, 0, 4, 10);
  gtk_grid_attach(GTK_GRID(pGrid), pButton[2],
      						280, 0, 4, 10);
	/* Show buttons */
  gtk_widget_show_all(pWindow);
  /* Associate a function to each button */
	g_signal_connect(G_OBJECT(pButton[0]), "clicked", 
									 G_CALLBACK(leave_dialog), (GtkWidget *) pWindow); 
	g_signal_connect(G_OBJECT(pButton[1]), "clicked", 
									 G_CALLBACK(get_path), pWindow);
	/*g_signal_connect(G_OBJECT(pButton[2]), "clicked", 
                   G_CALLBACK(REMPLACER PAR LA FONCTION QUI LANCE LE PROGRAMME),
																														NULL);*/
	gtk_main();
 
  return EXIT_SUCCESS;
}

