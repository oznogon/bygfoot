#include "defs.h"
#include "callback_func.h"
#include "callbacks.h"
#include "gui.h"
#include "misc2_callbacks.h"
#include "support.h"
#include "team.h"

void
on_button_cancel_clicked                   (GtkButton       *button,
					    gpointer         user_data)
{
    GtkWidget *stadium_window = 
	lookup_widget(GTK_WIDGET(button), "stadium_window");
    
    change_popups_active(-1);
    gtk_widget_destroy(stadium_window);
}

void
on_button_ok_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_improve_stadium(GTK_WIDGET(button));
}

void
on_button_offer_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *job_offer_window =
	lookup_widget(GTK_WIDGET(button), "job_offer_window");

    set_save(0);
    change_my_team(status % 1000);
    on_button_back_to_main_clicked(NULL, NULL);

    change_popups_active(-1);
    gtk_widget_destroy(job_offer_window);
}


void
on_button_offer_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *job_offer_window =
	lookup_widget(GTK_WIDGET(button), "job_offer_window");

    change_popups_active(-1);
    gtk_widget_destroy(job_offer_window);

    if(status >= 950000)
	on_button_quit_clicked(NULL, GINT_TO_POINTER(1));
}

