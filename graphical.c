#include <stdio.h>
#include <stdlib.h>
# include <err.h>
# include <time.h>
#include <gtk/gtk.h>
#include "ocr.h"
#include "bitmap.h"
#include "detection.h"
#include "queue.h"
#include "network.h"
#include "main_functions.h"

/* Create a dialog that allows the user to choose a file to open.
Once the file is open, launch the ocr algorithm on it */

typedef struct
{
  GtkWidget *image;
  GtkWidget *text;
  GtkWidget *pWindow;
}Zone;


void saveFile(GtkWidget *window, gpointer data){
	Zone *zone = (Zone*)data;
  GtkWidget *topLevel = NULL;
  topLevel = gtk_widget_get_toplevel(zone->text);
  GtkWidget *dialog = NULL;
  dialog = gtk_file_chooser_dialog_new("Save file", GTK_WINDOW(topLevel),
               GTK_FILE_CHOOSER_ACTION_SAVE,
               "_Cancel", GTK_RESPONSE_CANCEL,
               "_Ok", GTK_RESPONSE_ACCEPT,
               NULL);

  GtkFileFilter *filter = NULL;
  filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "Text File");
  gtk_file_filter_add_pattern(filter, "*tex)");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  GtkFileFilter *all = NULL;
  all = gtk_file_filter_new();
  gtk_file_filter_set_name(all, "All Files");
  gtk_file_filter_add_pattern(all, "*");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), all);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
             TRUE);
  gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), "New Document");
  
  switch(gtk_dialog_run(GTK_DIALOG(dialog)))
  { 
    case GTK_RESPONSE_ACCEPT:
    {
      gchar *fileName = NULL;
      fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      FILE *file = NULL;
      file = fopen(fileName, "w");
      if (file)
      { 
  			gchar *chars = NULL;
			  GtkTextBuffer *textBuffer = NULL;
			  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(zone->text));
			  GtkTextIter start;
			  GtkTextIter end;
			  gtk_text_buffer_get_bounds(textBuffer, &start, &end);
			  chars = gtk_text_buffer_get_text(textBuffer, &start, &end, FALSE);
			  fprintf (file, "%s", chars);
			  fclose(file);
			  g_free(chars);
      }
      else
			  warnx("Impossible to save file");
      break;
    }
    default:
      break;
  }
 	gtk_widget_destroy(dialog);
	(void)window;
}

void cbOpen(GtkWidget *widget, gpointer data)
{
  Zone *zone = (Zone*)data;
  GtkWidget *topLevel = NULL;
  topLevel = gtk_widget_get_toplevel(zone->image);
  GtkFileFilter *filter = NULL;
  filter = gtk_file_filter_new();
  GtkWidget *dialog = NULL;
  dialog = gtk_file_chooser_dialog_new("Open file", GTK_WINDOW(topLevel),
               GTK_FILE_CHOOSER_ACTION_OPEN,
               "_Cancel", GTK_RESPONSE_CANCEL,
               "_Open", GTK_RESPONSE_ACCEPT,
               NULL);

  gtk_file_filter_add_pixbuf_formats(filter);
  gtk_file_filter_set_name(filter, "Image File");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res ==  GTK_RESPONSE_ACCEPT)
  {
    gchar *fileName = NULL;
    fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    gtk_image_set_from_file(GTK_IMAGE(zone->image), fileName);
    g_free(fileName);
  }
	gtk_widget_destroy(dialog);
	(void)widget;
}

/* Create a dialog that make the user confirm if he really 
wants to quit the program*/
void leaveDialog(GtkWidget *widget, gpointer data)
{
	Zone *zone = (Zone*)data;
  GtkWidget *pQuestion;
	GtkWidget *pWindow;
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
					free(zone);
          break; 
    case GTK_RESPONSE_NO:
          /* No -> dialog destroyed*/
          break;
    }
	gtk_widget_destroy(pQuestion);
}

int start(int argc, char **argv)
{
 
  gtk_init(&argc, &argv);
	GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(mainWindow), "destroy",
														  G_CALLBACK(leaveDialog), NULL);

  gtk_window_maximize(GTK_WINDOW(mainWindow));
  gtk_window_set_title(GTK_WINDOW(mainWindow), "Group's Name");

  //Image and Text
  Zone *zone = malloc(sizeof(struct Zone*));
  zone->image = gtk_image_new();
  zone->text = gtk_text_view_new();
	
	GtkWidget *mainBox = NULL;	
	mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_container_add(GTK_CONTAINER(mainWindow), mainBox);

	/*Menu Bar*/
  GtkWidget *menuBar = NULL;
  menuBar = gtk_menu_bar_new();

  /*Menu*/
  GtkWidget *menu = NULL;
  menu = gtk_menu_new();
  /*Menu items*/
  GtkWidget *menuItem = NULL;
  menuItem = gtk_menu_item_new_with_label("Open");
  g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(cbOpen), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_menu_item_new_with_label("Save");
  g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(saveFile), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_menu_item_new_with_label("Quit");
  g_signal_connect(G_OBJECT(menuItem), "activate", 
												G_CALLBACK(leaveDialog), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  /*Submenu*/
  menuItem = gtk_menu_item_new_with_label("File");
  /*Link menu items*/
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  /*Add to menu bar*/
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);


  /*Menu*/
  menu = gtk_menu_new();
  /*Menu items*/
  menuItem = gtk_menu_item_new_with_label("Process");
  //g_signal_connect(G_OBJECT(menuItem), "activate",
  //     G_CALLBACK(cb_process), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_menu_item_new_with_label("Learn");
  g_signal_connect(G_OBJECT(menuItem), "activate",
       G_CALLBACK(launchLearning), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);


  /*Submenu*/
  menuItem = gtk_menu_item_new_with_label("Edit");
  /*Link menu items*/
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  /*Add to menuBar*/
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);


  /*Menu*/
  menu = gtk_menu_new();
  /*Menu items*/
  menuItem = gtk_menu_item_new_with_label("Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_menu_item_new_with_label("About");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  /*S u*/
  menuItem = gtk_menu_item_new_with_label("?");
  /*Link menu items*/
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  /*Add to menuBar*/
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  /*Add bar to window*/
  gtk_box_pack_start(GTK_BOX(mainBox), menuBar, FALSE, FALSE, 0);

	GtkWidget *toolBar = gtk_toolbar_new();
  gtk_box_pack_start(GTK_BOX(mainBox), toolBar, FALSE, FALSE, 0);
	
	GtkButton *op = GTK_BUTTON(gtk_button_new_with_mnemonic("_Open"));
	g_signal_connect(op, "clicked", G_CALLBACK(cbOpen), zone);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), GTK_TOOL_ITEM(op), -1);
	GtkButton *sf = GTK_BUTTON(gtk_button_new_with_mnemonic("_Save"));
  g_signal_connect(op, "clicked", G_CALLBACK(cbOpen), zone);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), GTK_TOOL_ITEM(sf), -1);
	GtkButton *ex = GTK_BUTTON(gtk_button_new_with_mnemonic("_Execute"));
  g_signal_connect(op, "clicked", G_CALLBACK(cbOpen), zone);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), GTK_TOOL_ITEM(ex), -1);
	GtkButton *qu = GTK_BUTTON(gtk_button_new_with_mnemonic("_Quit"));
  g_signal_connect(op, "clicked", G_CALLBACK(cbOpen), zone);
  gtk_toolbar_insert(GTK_TOOLBAR(toolBar), GTK_TOOL_ITEM(qu), -1);
  gtk_toolbar_set_style(GTK_TOOLBAR(toolBar), GTK_TOOLBAR_ICONS);
	
	GtkWidget *mainZone = NULL;
  mainZone = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  GtkWidget *separator = NULL;
  separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_box_pack_start(GTK_BOX(mainBox), separator, FALSE, FALSE, 2);
  separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);

  GtkWidget *scroll = NULL;
  scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scroll), zone->text);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
         GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
         GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_box_pack_start(GTK_BOX(mainZone), separator, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX(mainZone), zone->image, FALSE, TRUE, 0);
  separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(mainZone), separator, FALSE, FALSE, 2);
  gtk_box_pack_start(GTK_BOX(mainZone), scroll, TRUE, TRUE, 0);
  separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
  gtk_box_pack_start(GTK_BOX(mainZone), separator, FALSE, FALSE, 2);

  gtk_box_pack_end(GTK_BOX(mainBox), mainZone, TRUE, TRUE, 0);

	gtk_widget_show_all(mainWindow);
	//g_signal_connect(G_OBJECT(pButton[3]), "clicked",
    //               G_CALLBACK(learning), zone);
	gtk_main();
  return EXIT_SUCCESS;
}


