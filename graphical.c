#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "ocr.h"

void leaveButton(GtkWidget *pWindow)
{
  GtkWidget *pQBtn;

  pQBtn = gtk_button_new_with_mnemonic("_Quitter");
  g_signal_connect(G_OBJECT(pQBtn), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_add(GTK_CONTAINER(pWindow), pQBtn);
}


void recuperer_chemin(GtkWidget *file_selection)
{
    const gchar* chemin;
    GtkWidget *dialog;
    chemin = gtk_file_selection_get_filename(GTK_FILE_SELECTION(file_selection));

    dialog = gtk_message_dialog_new(GTK_WINDOW(file_selection),
    GTK_DIALOG_MODAL,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "Vous avez choisi :\n%s", chemin);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(file_selection);
}

void creer_file_selection()
{
    GtkWidget *selection;
     
    selection = gtk_file_selection_new( g_locale_to_utf8
("Sélectionnez un fichier", -1, NULL, NULL, NULL) );
    gtk_widget_show(selection);
     
    //On interdit l'utilisation des autres fenêtres.
    gtk_window_set_modal(GTK_WINDOW(selection), TRUE);
			 
    g_signal_connect(G_OBJECT(GTK_FILE_SELECTION(selection)->ok_button),
 "clicked", G_CALLBACK(recuperer_chemin), selection);
     
    g_signal_connect_swapped(G_OBJECT(
GTK_FILE_SELECTION(selection)->cancel_button), "clicked", 
G_CALLBACK(gtk_widget_destroy), selection);
}

void quitter(GtkWidget* widget)
{
    // destruction de win et de tout ce qu'il contient
    gtk_widget_destroy(widget);
    gtk_main_quit();
}
 
void on_quitter_button()
{
	GtkWidget *pWindow;
  GtkWidget *pQuestion;
    /* Création de la boite de message */
    /* Type : Question -> GTK_MESSAGE_QUESTION */
   /* Boutons : 1 OUI, 1 NON -> GTK_BUTTONS_YES_NO */
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);

  pQuestion = gtk_message_dialog_new(GTK_WINDOW(pWindow),
      GTK_RESPONSE_ACCEPT,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "Voulez-vous vraiment\nquitter ce programme?");
    /* Affichage et attente d une réponse */
  switch(gtk_dialog_run(GTK_DIALOG(pQuestion)))
  {
    case GTK_RESPONSE_YES:
        /* OUI -> on quitte l application */
          gtk_main_quit();
          break; 
    case GTK_RESPONSE_NO:
            /* NON -> on détruit la boite de message */
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

  pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(pWindow), 320, 200);
  gtk_window_set_title(GTK_WINDOW(pWindow), "OCR Project");
  g_signal_connect(G_OBJECT(pWindow), "destroy", 
G_CALLBACK(gtk_main_quit), NULL);
 
  /* Creation et insertion de la table 3 lignes 2 colonnes */
  pGrid = gtk_grid_new();
	for(int i = 0; i < 10; i++)
	{
		gtk_grid_insert_row(GTK_GRID(pGrid), 0);
		gtk_grid_insert_column(GTK_GRID(pGrid), 0);
	}
	for(int i = 0; i < 20; i++)
	{
  	gtk_grid_insert_row(GTK_GRID(pGrid), 0);
	}
  gtk_container_add(GTK_CONTAINER(pWindow), GTK_WIDGET(pGrid));

	gtk_grid_set_row_spacing (GTK_GRID(pGrid), 10); 
  gtk_grid_set_column_spacing (GTK_GRID(pGrid), 140);
  /* Creation des boutons */
  pButton[0]= gtk_button_new_with_mnemonic("_Quit");
  pButton[1]= gtk_button_new_with_mnemonic("_Open a file");
  pButton[2]= gtk_button_new_with_mnemonic("_Launch Program");
 
  /* Insertion des boutons */
  gtk_grid_attach(GTK_GRID(pGrid), pButton[0],
      9, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(pGrid), pButton[1],
      0, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(pGrid), pButton[2],
      4, 0, 2, 2);

  gtk_widget_show_all(pWindow);
  g_signal_connect(G_OBJECT(pButton[0]), "clicked", 
G_CALLBACK(on_quitter_button), (GtkWidget *) pWindow); 
  g_signal_connect(G_OBJECT(pButton[1]), "clicked", 
G_CALLBACK(creer_file_selection), NULL);
	gtk_main();
 
  return EXIT_SUCCESS;
}

