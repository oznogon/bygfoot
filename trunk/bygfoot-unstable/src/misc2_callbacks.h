#ifndef MISC2_CALLBACKS_H
#define MISC2_CALLBACKS_H

#include "bygfoot.h"

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
gboolean
on_graph_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_button_close_clicked                (GtkButton       *button,
                                        gpointer         user_data);

void
on_optionmenu_player_changed           (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);
#endif

void
on_optionmenu_finances_changed         (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);
