/*********************************************************************
 * Callback functions called directly from the editor                *
 *********************************************************************/

#include "defs.h"
#include "support.h"
#include "editor_defs.h"
#include "editor_callbacks.h"

void
on_button_browse_team_files_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
    show_file_selection(100);
}


void
on_button_exit_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    gtk_widget_destroy(main_window);
    gtk_main_quit();
}


void
on_button_save_defs_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_save_defs();
}


void
on_button_save_play_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_play(1);
}


void
on_button_change_structures_clicked    (GtkButton *button,
                                        gpointer         user_data)
{
    callback_change_structures();
}


void
on_button_choose_def_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    show_file_selection(110);
}

void
on_button_swap_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_swap_players();
}

gboolean
on_treeview_players_button_press_event (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    GtkTreeSelection *selection =
	gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
    
    callback_edit_player(selection, event);

    return FALSE;
}


void
on_button_up_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_move(-1);
}


void
on_button_down_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_move(1);
}


void
on_button_update_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_update();
}


void
on_button_play_ws_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_play(0);
}


void
on_button_editor_help_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    show_help(1);
}


void
on_entry_country_file_changed             (GtkEditable     *editable,
                                        gpointer         user_data)
{
    GtkWidget *entry_country_file = 
	lookup_widget(main_window, "entry_country_file");
    const gchar *filename = 
	gtk_entry_get_text(GTK_ENTRY(entry_country_file));
    FILE *fil = fopen(filename, "r");

    if(fil == NULL)
	return;

    fclose(fil);

    callback_load_team_file();
}


void
on_entry_team_changed                 (GtkEditable     *editable,
                                        gpointer         user_data)
{
    GtkWidget *entry_team = 
	lookup_widget(main_window, "entry_team");
    const gchar *team_name = gtk_entry_get_text(GTK_ENTRY(entry_team));

    if(strlen(team_name) == 0)
	return;

    callback_load_team();
}

void
on_button_prev_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_change_player(-1);
}


void
on_button_next_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_change_player(1);
}


void
on_spinbutton_average_skill_value_changed (GtkSpinButton   *spinbutton,
					   gpointer         user_data)
{
    callback_change_average_skill();
}

void
on_button_export_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    show_file_selection(120);
}


void
on_button_import_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    show_file_selection(121);
}

