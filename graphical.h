/**
 * \file graphical.h
 * \brief Create the graphical interface and call the function
 * \author decret_t
 * \date October 19th 2016
 *
 * Generate the graphical user interface (GUI) and link all the 
 * buttons with the functions to launch. 
 */

# ifndef GRAPHICAL_H_INCLUDED
# define GRAPHICAL_H_INCLUDED

# include "ocr.h"
# include <gtk/gtk.h>
# include "bitmap.h"
# include "detection.h"
# include "queue.h"
# include "network.h"

/**
 *\struct Zone
 *\brief Struct that contains all the info of the graphical interface
 *
*/
typedef struct {
	GtkWidget *pWindow;
	GtkWidget *image;
	GtkWidget *text;
}Zone;

/**
 *\brief Convert a constant string to a double
 *
 *\param s The constant string to convert
*/
double a2i(const char *s);

/**
 *\brief Launch the rotation on the current image
 *
 *\param window The current parent window
 *\param data The pointer to the Zone
*/
void rotation(GtkWidget *window, gpointer data);

/**
 * \brief Process the optical recognition of character algorithm on the chosen
 * picture
 *
 * \param window The parent window
 * \param data The pointer to the created zone struct
 */
void process(GtkWidget *window, gpointer data);

/**
 * \brief Save the file at the location chosen by the user
 * 
 * \param window The parent window
 * \param data The pointer to the created zone struct
 */
void saveFile(GtkWidget *widget, gpointer data);

/**
 * \brief Return the path of a file chosen by the user
 *
 * \param widget The parent window
 * \param data The pointer to the created zone struct
 */
void fileChoose(GtkWidget *widget, gpointer data);

/**
 * \brief Open the file chosen by the user. Must be a image type file.
 *
 * \param widget The parent window
 * \param data The pointer to the created zone struct
 */
void cbOpen(GtkWidget *widget, gpointer data);

/**
 * \brief Create a dialog that make the user confirm if he really wants to quit
 * the program
 * 
 * \param widget The parent window
 * \param data The pointer to the created zone struct
 */
void leaveDialog(GtkWidget *widget, gpointer data);

/**
 * \brief Create the parent window, the menus, the zones, ...
 * 
 * \param argc The argc parameter of the main fuction
 * \param argv The argv parameter of the main fuction
 */
int start(int argc, char **argv);

# endif
