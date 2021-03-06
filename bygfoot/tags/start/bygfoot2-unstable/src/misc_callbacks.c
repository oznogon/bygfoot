#include "misc_callbacks.h"

void
on_team_selection_cancel_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    free_memory();
    gtk_main_quit();
}


void
on_team_selection_tv_row_activated     (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{

}


void
on_radiobutton_country_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button_select_country_file_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_checkbutton_load_randomly_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_checkbutton_only_names_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button_start_editor_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_team_selection_ok_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_team_selection_load_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


gboolean
on_popup_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_popup_check_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{

}


void
on_button_popup_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_popup_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_popup_close_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


gboolean
on_fsel_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_button_fsel_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_fsel_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


gboolean
on_button_font_sel_cancel_clicked      (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_button_font_sel_ok_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_font_sel_apply_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{

}


gboolean
on_live_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{

  return FALSE;
}


void
on_button_live_close_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_button_help_close_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{

}


void
on_combo_country_entry_changed         (GtkEditable     *editable,
                                        gpointer         user_data)
{
    GtkWidget *combo_country_entry =
	lookup_widget(GTK_WIDGET(editable), "combo_country_entry");
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(combo_country_entry));

    if(strlen(entry_text) == 0)
	return;

    misc_callback_show_team_list(GTK_WIDGET(editable), entry_text);
}
