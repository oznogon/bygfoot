#ifndef MISC2_CALLBACKS_H
#define MISC2_CALLBACKS_H

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
#endif
