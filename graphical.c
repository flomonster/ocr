/**
 * \file graphical.c
 * \brief Create the graphical interface and call the function
 * \author decret_t
 * \date October 19th 2016
 *
 * Generate the graphical user interface (GUI) and link all the 
 * buttons with the functions to launch. 
 */

# include <gdk-pixbuf/gdk-pixbuf.h>
# include <stdio.h>
# include <stdlib.h>
# include <err.h>
# include <time.h>
# include <unistd.h>
# include <gtk/gtk.h>
# include "ocr.h"
# include "bitmap.h"
# include "detection.h"
# include "queue.h"
# include "network.h"
# include "learning.h"

/* Create a dialog that allows the user to choose a file to open.
Once the file is open, launch the ocr algorithm on it */


/**
 *\struct Zone
 *\brief Struct that contains all the info of the graphical interface
 *
*/
typedef struct
{
  GtkWidget *image; /* !< The current image */
  GtkWidget *text;  /* !< The text currently displayed */
  GtkWidget *pWindow; /* !< The parent window, the first one created */
	char *path; /* !< The path of the image to be processed */
}Zone;

/**
 *\brief Convert a constant string to an integer
 *
 *\param s The constant string to convert
*/
int a2i(const char *s)
{
 	int sign=1;
 	if(*s == '-'){
 		sign = -1;
 		s++;
	}
 	int num=0;
 	while(*s && *s != '.' && *s != '\0')
  {
   	num=((*s)-'0')+num*10;
    s++;   
  }
 	return num*sign;
}

/**
 *\brief Launch the rotation on the current image
 *
 *\param window The current parent window
 *\param data The pointer to the Zone
*/
void rotation(GtkWidget *window, gpointer data){
	Zone *zone = (Zone *)data;
	bitmap *img = loadBmp(zone->path);
	rotate(img, -5);
	char *path = "pictureRotate.bmp";
	saveBmp(path, img);
	zone->path = path;
	gtk_image_set_from_file(GTK_IMAGE(zone->image), path);
	GError *error;
	GdkPixbuf *pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(zone->image));
	gdk_pixbuf_save(pixbuf, path, "bmp", &error, NULL);
	(void)window;
}

/**
 * \brief Process the optical recognition of character algorithm on the chosen
 * picture
 *
 * \param window The parent window
 * \param data The pointer to the created zone struct
 */
void process(GtkWidget *window, gpointer data){
	network *n = loadNetwork("network.save");
	Zone *zone = (Zone *)data;
	printf("%s", zone->path);
  bitmap *img = loadBmp(zone->path);
	//draw(img);
	size_t *length = malloc(sizeof(size_t));
	size_t *useless = malloc(sizeof(size_t));
	queue *q = detectText(img, useless, length);
	int i = 0;
	char txt[*length + 1];
	txt[*length] = 0;
	char c;
	gtk_image_set_from_file(GTK_IMAGE(zone->image), "fusion.bmp");
	gtk_image_set_from_file(GTK_IMAGE(zone->image), "rlsaImage.bmp");
	gtk_image_set_from_file(GTK_IMAGE(zone->image), "segmented.bmp");
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
				c = ocr(letter, n);
				txt[i] = c;
				freeBitmap(letter);
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
		GtkTextBuffer *textBuffer = NULL;
		textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(zone->text));
		gtk_text_buffer_set_text(textBuffer, txt, i);
		free(useless);
    free(length);
    free(q);
    freeBitmap(img);
    freeNetwork(n);
		(void)window;
}

/**
 * \brief Save the file at the location chosen by the user
 * 
 * \param window The parent window
 * \param data The pointer to the created zone struct
 */
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

/**
 * \brief Return the path of a file chosen by the user
 *
 * \param widget The parent window
 * \param data The pointer to the created zone struct
 */
void fileChoose(GtkWidget *widget, gpointer data)
{
  Zone *zone = (Zone*)data;
  GtkWidget *dialog = NULL;
  gchar *fileName = NULL;
  GtkWidget *topLevel = NULL;
  topLevel = gtk_widget_get_toplevel(zone->image);
  dialog = gtk_file_chooser_dialog_new("Open file", GTK_WINDOW(topLevel),
               GTK_FILE_CHOOSER_ACTION_OPEN,
               "_Cancel", GTK_RESPONSE_CANCEL,
               "_Open", GTK_RESPONSE_ACCEPT,
               NULL);

  gint res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res ==  GTK_RESPONSE_ACCEPT)
  {
    fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
  }
	else{
		gtk_widget_destroy(dialog);
		return;
	}
	gtk_widget_destroy(dialog);
	printf("Filename : %s\n", fileName);
	char *listfiles[] = {fileName};
	learning(listfiles, 1);
	(void)widget;
  zone->path = fileName;
}

/**
 * \brief Open the file chosen by the user. Must be a image type file.
 *
 * \param widget The parent window
 * \param data The pointer to the created zone struct
 */
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
		zone->path = fileName;
  }
	gtk_widget_destroy(dialog);
	(void)widget;
}

/**
 * \brief Create a dialog that make the user confirm if he really wants to quit
 * the program
 * 
 * \param widget The parent window
 * \param data The pointer to the created zone struct
 */
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
	(void)widget;
}

/**
 * \brief Create the parent window, the menus, the zones, ...
 * 
 * \param argc The argc parameter of the main fuction
 * \param argv The argv parameter of the main fuction
 */
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
	
  menuItem = gtk_menu_item_new_with_label("Generate network");
  g_signal_connect(G_OBJECT(menuItem), "activate", 
								G_CALLBACK(generateNetwork), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  
	menuItem = gtk_menu_item_new_with_label("Learn");
  g_signal_connect(G_OBJECT(menuItem), "activate", 
								G_CALLBACK(fileChoose), zone);
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
  g_signal_connect(G_OBJECT(menuItem), "activate",
       G_CALLBACK(process), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_menu_item_new_with_label("Learn");
  g_signal_connect(G_OBJECT(menuItem), "activate",
       G_CALLBACK(fileChoose), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

	menuItem = gtk_menu_item_new_with_label("Rotate");
  g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(rotation), zone);
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
	gtk_main();
  return EXIT_SUCCESS;
}
