/************************************************************
 * callbacks for the options window                         *
 ************************************************************/

#include "callback_func.h"
#include "callbacks.h"
#include "gui.h"
#include "options_callbacks.h"
#include "support.h"
#include "treeview.h"
#include "window.h"

void
on_button_opt_ok_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{    
    GtkWidget *opt_window =
	lookup_widget(GTK_WIDGET(button), "opt_window");
    
    set_up_options_window(opt_window, 0);

    change_popups_active(-1);

    show_players(NULL, NULL, 0, NULL, 0);

    set_save(0);
    
    gtk_widget_destroy(opt_window);

    on_button_back_to_main_clicked(NULL, NULL);
}


void
on_button_opt_cancel_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *opt_window =
	lookup_widget(GTK_WIDGET(button), "opt_window");

    change_popups_active(-1);
    
    gtk_widget_destroy(opt_window);
}


void
on_check_show_live_toggled             (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *spin_live_duration =
	lookup_widget(GTK_WIDGET(togglebutton),
		      "spin_live_duration");

    if(gtk_toggle_button_get_active(togglebutton))
	gtk_widget_set_sensitive(spin_live_duration->parent->parent,
				 TRUE);
    else
	gtk_widget_set_sensitive(spin_live_duration->parent->parent,
				 FALSE);
}


void
on_check_notify_bound_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *spin_notify_bound =
	lookup_widget(GTK_WIDGET(togglebutton),
		      "spin_notify_bound");
    GtkWidget *check_notify_pos0 =
	lookup_widget(GTK_WIDGET(togglebutton),
		      "check_notify_pos0");
    
    if(gtk_toggle_button_get_active(togglebutton))
    {
	gtk_widget_set_sensitive(spin_notify_bound,
				 TRUE);
	gtk_widget_set_sensitive(check_notify_pos0->parent->parent,
				 TRUE);
    }
    else
    {
	gtk_widget_set_sensitive(spin_notify_bound,
				 FALSE);
	gtk_widget_set_sensitive(check_notify_pos0->parent->parent,
				 FALSE);
    }
}


void
on_button_opt_change_font_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
    show_font_sel_window();
}


void
on_spin_live_duration_activate         (GtkEntry        *entry,
                                        gpointer         user_data)
{
    GtkWidget *button_opt_ok =
	lookup_widget(GTK_WIDGET(entry), "button_opt_ok");
    gint value = (gint)strtol(gtk_entry_get_text(entry), NULL, 10);

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry),
			      (gdouble)value);
    on_button_opt_ok_clicked(GTK_BUTTON(button_opt_ok), NULL);
}

void
on_checkbutton_notify_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    gtk_widget_set_sensitive(lookup_widget(GTK_WIDGET(togglebutton), "hbox52"),
			     gtk_toggle_button_get_active(togglebutton));
}

void
on_checkbutton_autosave_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    gchar buf[SMALL];
    FILE *autosave_file;
    GtkWidget *spinbutton_autosave =
	lookup_widget(GTK_WIDGET(togglebutton),
		      "spinbutton_autosave");

    if(!gtk_toggle_button_get_active(togglebutton))
    {
	gtk_widget_set_sensitive(spinbutton_autosave, FALSE);
	return;
    }

    sprintf(buf, "%s/.bygfoot/saves/autosave", getenv("HOME"));

    autosave_file = fopen(buf, "a+");

    if(autosave_file == NULL)
    {
	gtk_toggle_button_set_active(togglebutton, FALSE);
	show_popup_window("Could not open file $HOME/.bygfoot/saves/autosave in read/write mode. Autosave is deactivated.", NULL);
	return;
    }

    fclose(autosave_file);

    gtk_widget_set_sensitive(spinbutton_autosave, TRUE);
}


void
on_checkbutton_sort_transfers_toggled  (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *radiobutton_sort_pos = 
	lookup_widget(GTK_WIDGET(togglebutton), "radiobutton_sort_pos");

    gtk_widget_set_sensitive(radiobutton_sort_pos->parent,
			     gtk_toggle_button_get_active(togglebutton));
}

