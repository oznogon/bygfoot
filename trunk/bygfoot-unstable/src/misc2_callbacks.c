#include "callback_func.h"
#include "callbacks.h"
#include "graph.h"
#include "gui.h"
#include "misc2_callbacks.h"
#include "maths.h"
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

gboolean
on_graph_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    on_button_close_clicked(GTK_BUTTON(lookup_widget(widget, "button_close")),
			    NULL);
    return FALSE;
}


void
on_button_close_clicked                (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *graph_window =
	lookup_widget(GTK_WIDGET(button), "graph_window");

    gtk_widget_destroy(graph_window);

    change_popups_active(-1);

    on_button_back_to_main_clicked(NULL, NULL);
}

void
on_optionmenu_player_changed           (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint i;
    gint value = gtk_option_menu_get_history(optionmenu);
    gint team_id = get_place(get_place(-1 * status, 14), 23);
    gint player_number = get_place(-1 * status, 22);

    for(i=HISTORY_SKILL;i<HISTORY_MONEY;i++)
	if(value == i - HISTORY_SKILL + 1)
	    show_graph(lookup_widget(GTK_WIDGET(optionmenu), "graph_window"),
		       team_id, player_number, i);
}

void
on_optionmenu_finances_changed         (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{

}

