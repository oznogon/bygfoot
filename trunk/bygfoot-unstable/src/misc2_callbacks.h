#include <gtk/gtk.h>

/* change the human player's team */
void change_my_team(gint new_team);

/* handle a click on the OK button of the stadium capacity increase window */
void
callback_improve_stadium(GtkWidget *widget);

void
on_button_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data);
void
on_button_cancel_clicked                   (GtkButton       *button,
					    gpointer         user_data);

void
on_button_offer_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_offer_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data);
