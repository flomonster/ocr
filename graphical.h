# ifndef GRAPHICAL_H_INCLUDED
# define GRAPHICAL_H_INCLUDED

int start(int argc, char **argv);

void leaveButton(GtkWidget *pWindow);

void on_quitter_btn(GtkWidget *data);

void recuperer_chemin(GtkWidget *parent_window);

void quitter(GtkWidget* widget);

# endif
