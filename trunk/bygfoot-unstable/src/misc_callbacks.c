/************************************************************
 * callbacks for the live game window                       *
 ************************************************************/

#include "callback_func.h"
#include "callbacks.h"
#include "editor_callback_func.h"
#include "game_gui.h"
#include "generation.h"
#include "gui.h"
#include "load_save.h"
#include "maths.h"
#include "misc_callbacks.h"
#include "player.h"
#include "startup.h"
#include "support.h"
#include "team.h"
#include "text_files.h"
#include "window.h"

gboolean
on_live_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    return TRUE;
}


void
on_button_live_close_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *live_window =
	lookup_widget(GTK_WIDGET(button), "live_window");
    GtkWidget *check_live_window_show =
	lookup_widget(live_window, "check_live_window_show_live");
    GtkWidget *check_live_window_tendency =
	lookup_widget(live_window, "check_live_window_tendency");
    
    options[OPT_SHOW_LIVE] = gtk_toggle_button_get_active(
	GTK_TOGGLE_BUTTON(check_live_window_show));
    options[OPT_LIVE_TENDENCY] = gtk_toggle_button_get_active(
	GTK_TOGGLE_BUTTON(check_live_window_tendency));

    change_popups_active(-1);
    gtk_widget_destroy(live_window);

    /* if only one live game has been shown (status -100000)
       we try to show another one */
    if(status == -100000)
    {
	live_game(1);
	return;
    }

    /* status is set to -50000 by 'live_game' when
       there are no more games to show */
    if(status == -50000)
    {
	callback_new_week(FALSE);
	set_buttons();
    }
}


/************************************************************
 * callbacks for the font selection window                  *
 ************************************************************/

void
on_button_font_sel_ok_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *font_sel_window =
	lookup_widget(GTK_WIDGET(button), "font_sel_window");
    GtkWidget *font_sel =
	lookup_widget(GTK_WIDGET(button), "font_sel");

    sprintf(font_name, "%s",
	    gtk_font_selection_get_font_name(
		GTK_FONT_SELECTION(font_sel)));

    change_popups_active(-1);
    gtk_widget_destroy(font_sel_window);
}


void
on_button_font_sel_apply_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *font_sel =
	lookup_widget(GTK_WIDGET(button), "font_sel");

    sprintf(font_name, "%s",
	    gtk_font_selection_get_font_name(
		GTK_FONT_SELECTION(font_sel)));

    if(strcmp(font_name, "0") == 0)
	return;

    on_button_back_to_main_clicked(NULL, NULL);
    on_button_preview_clicked(NULL, NULL);
}


void
on_button_font_sel_cancel_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *font_sel_window =
	lookup_widget(GTK_WIDGET(button), "font_sel_window");

    change_popups_active(-1);
    gtk_widget_destroy(font_sel_window);
}


/************************************************************
 * callbacks for the file selection window                  *
 ************************************************************/

void
on_button_fsel_ok_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    gchar buf[SMALL];
    GtkWidget *entry;
    GtkWidget *fsel_window =
	lookup_widget(GTK_WIDGET(button), "fsel_window");
    gboolean good_file = TRUE;
    gchar *file_name = 
	(gchar*)gtk_file_selection_get_filename(
	    GTK_FILE_SELECTION(fsel_window));
    FILE *fil;
    
    if(status == 800001)
    {
	save_game(file_name);
	set_save(1);
    }
    else if(status == 800000)
    {
	if(load_game(file_name))
	{
	    set_save(1);
	    on_button_back_to_main_clicked(NULL, NULL);
	}
	else
	{
	    show_popup_window("Doesn't seem to be a Bygfoot save file.",
			      NULL);
	    good_file = FALSE;
	}
    }
    else if(status == 800002)
    {
	if(load_game(file_name))
	{
	    gtk_widget_show(main_window);
	    on_button_back_to_main_clicked(NULL, NULL);
	    set_save(1);
	}
	else
	{
	    show_popup_window("Doesn't seem to be a Bygfoot save file.",
			      NULL);
	    good_file = FALSE;
	}
    }
    else if(status == 800010)
    {
	if(fopen(gtk_file_selection_get_filename(
		     GTK_FILE_SELECTION(fsel_window)), "r") == NULL)
	{
	    sprintf(buf, "Could not open file: %s .",
		    gtk_file_selection_get_filename(
			GTK_FILE_SELECTION(fsel_window)));
	    show_popup_window(buf, NULL);

	    good_file = FALSE;
	}
	else
	    country_names(0, gtk_file_selection_get_filename(
			      GTK_FILE_SELECTION(fsel_window)));

	if(good_file)
	    show_team_selection();
    }
    /* editor window */
    else if(status == 800100 ||
	    status == 800110)
    {
	if(status == 800100)
	    entry = 
		lookup_widget(main_window, "entry_country_file");
	else
	    entry = 
		lookup_widget(main_window, "entry_definitions_file");

	gtk_entry_set_text(GTK_ENTRY(entry),
			   gtk_file_selection_get_filename(
			       GTK_FILE_SELECTION(fsel_window)));
    }
    /* export teams when editing */
    else if(status == 800120)
    {
	fil = fopen(gtk_file_selection_get_filename(
			GTK_FILE_SELECTION(fsel_window)), "w");
	if(fil == NULL)
	{
	    sprintf(buf, "Could not open file in write mode. ");
	    show_popup_window(buf, NULL);
	    good_file = FALSE;
	}
	else
	{
	    fclose(fil);
	    save_defs(gtk_file_selection_get_filename(
			  GTK_FILE_SELECTION(fsel_window)));
	}
    }
    /* import teams when editing */
    else if(status == 800121)
    {
	fil = fopen(gtk_file_selection_get_filename(
			GTK_FILE_SELECTION(fsel_window)), "r");
	if(fil == NULL)
	{
	    sprintf(buf, "Could not open file in read mode. ");
	    show_popup_window(buf, NULL);
	    good_file = FALSE;
	}
	else
	{
	    fclose(fil);
	    import_file(gtk_file_selection_get_filename(
			    GTK_FILE_SELECTION(fsel_window)));
	}
    }

    if(!good_file)
	return;

    change_popups_active(-1);
    
    gtk_widget_destroy(fsel_window);
}


void
on_button_fsel_cancel_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *fsel_window =
	lookup_widget(GTK_WIDGET(button), "fsel_window");
    
    change_popups_active(-1);
    gtk_widget_destroy(fsel_window);
    
    if(status == 800010 || status == 800002)
	show_team_selection();
}


gboolean
on_fsel_window_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GtkWidget *button_fsel_cancel =
	lookup_widget(widget, "button_fsel_cancel");

    on_button_fsel_cancel_clicked(GTK_BUTTON(button_fsel_cancel),
				  NULL);

  return FALSE;
}


/************************************************************
 * callbacks for the team selection window                  *
 ************************************************************/

void
on_team_selection_cancel_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *team_selection = 
	lookup_widget(GTK_WIDGET(button), "team_selection");

    gtk_widget_destroy(team_selection);
    gtk_widget_destroy(main_window);

    gtk_main_quit();
}


void
on_team_selection_tv_row_activated     (GtkTreeView     *treeview,
                                        GtkTreePath     *path,
                                        GtkTreeViewColumn *column,
                                        gpointer         user_data)
{
    GtkWidget *button = lookup_widget(GTK_WIDGET(treeview),
				      "team_selection_ok");

    on_team_selection_ok_clicked(GTK_BUTTON(button), NULL);
}

void
on_team_selection_ok_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *team_selection = 
	lookup_widget(GTK_WIDGET(button), "team_selection");
    GtkWidget *checkbutton_only_names = 
	lookup_widget(GTK_WIDGET(button), "checkbutton_only_names");
    GtkWidget *checkbutton_no_def = 
	lookup_widget(GTK_WIDGET(button), "checkbutton_no_def");
    GtkWidget *checkbutton_load_my_team = 
	lookup_widget(GTK_WIDGET(button), "checkbutton_load_my_team");
    gint read = 2;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_no_def)))
	read = 0;
    else if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_only_names)))
	read = 1;

    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_load_my_team)))
	read += 10;

    /* get the id of the human player's team according
       to the list */
    get_my_team(GTK_WIDGET(button));    
    
    /* read the teams file which specifies (perhaps)
       some data about the players of a team */
    read_teams_file(read, NULL, NULL, NULL);

    /* remove some players from the team. the human player's
       team has initially 17 players in the team (so that he
       has some room for new players), the cpu-teams have 20 */    
    set_up_my_team();

    gtk_widget_destroy(team_selection);

    /* show the main window with the human player's team in the
       player list */
    initialize_main_window();

    gtk_widget_show(main_window);        

    set_save(0);

    set_buttons();
}


void
on_team_selection_load_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *team_selection = 
	lookup_widget(GTK_WIDGET(button), "team_selection");

    show_file_selection(2);

    gtk_widget_destroy(team_selection);
}


void
on_radiobutton_country_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    change_country_team_selection(GTK_WIDGET(togglebutton));
}


void
on_button_select_country_file_clicked  (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *team_selection =
	lookup_widget(GTK_WIDGET(button), "team_selection");

    show_file_selection(10);
    
    gtk_widget_destroy(team_selection);
}


void
on_checkbutton_load_randomly_toggled   (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *checkbutton_only_names = 
	lookup_widget(GTK_WIDGET(togglebutton), "checkbutton_only_names");
    GtkWidget *checkbutton_load_my_team = 
	lookup_widget(GTK_WIDGET(togglebutton), "checkbutton_load_my_team");

    if(gtk_toggle_button_get_active(togglebutton))
    {
	gtk_widget_set_sensitive(checkbutton_only_names, FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_load_my_team), FALSE);
    }
    else
    {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_load_my_team), TRUE);
	gtk_widget_set_sensitive(checkbutton_only_names, TRUE);
    }
}

void
on_checkbutton_only_names_toggled      (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    GtkWidget *checkbutton_load_my_team = 
	lookup_widget(GTK_WIDGET(togglebutton), "checkbutton_load_my_team");

    if(gtk_toggle_button_get_active(togglebutton))
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_load_my_team), FALSE);
    else
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_load_my_team), TRUE);
}

void
on_button_start_editor_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *team_selection =
	lookup_widget(GTK_WIDGET(button), "team_selection");

    gtk_widget_destroy(team_selection);
    gtk_widget_destroy(main_window);
    
    start_editor();
}


/************************************************************
 * callbacks for the popup window                           *
 ************************************************************/



void
on_button_popup_ok_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{    
    gint i;
    gint status[3];
    GtkWidget *popup_window =
	lookup_widget(GTK_WIDGET(button), "popup_window");
    GtkWidget *label_popup_status[3];
    GtkWidget *radiobutton =
	lookup_widget(GTK_WIDGET(button), "radiobutton0");
    gboolean one_time =
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton));
    
    label_popup_status[0] = 
	lookup_widget(popup_window, "label_popup_status");
    label_popup_status[1] = 
	lookup_widget(popup_window, "label_popup_status2");
    label_popup_status[2] = 
	lookup_widget(popup_window, "label_popup_status3");
    
    for(i=0;i<3;i++)
	status[i] = 
	    get_int_from_label(GTK_LABEL(label_popup_status[i]));

    change_popups_active(-1);
    gtk_widget_destroy(popup_window);

    if(status[0] == 50)	
	on_button_quit_clicked(NULL, GINT_TO_POINTER(1));
    else if(status[0] == 40)
	on_menu_new_activate(NULL, GINT_TO_POINTER(1));
    else if(status[0] >= 100 && status[0] < 200)
    {
	if(status[0] == 100 + OPT_CONF_NEW_WEEK)
	    on_button_new_week_clicked(NULL, GINT_TO_POINTER(1));
	else if(status[0] == 100 + OPT_CONF_UNFIT)
	    on_button_new_week_clicked(NULL, GINT_TO_POINTER(2));
    }
    else if(get_place(status[0], 12) == 13)
    {
	set_save(0);
	callback_popup_buy_player(status[0] % 1000,
				  status[1]);
    }
    else if(status[0] >= 1000000)
    {
	set_save(0);
	callback_sell_player(status[0] - 1000000, 
			     status[2]);
    }
    else if(status[0] == 92)
	on_start_editor_activate(NULL, GINT_TO_POINTER(1));
    else if(status[0] == 93)
	on_start_update_activate(NULL, GINT_TO_POINTER(1));
    else if(status[0] == 60)
    {
	set_save(0);
	fire_player(one_time);
    }
}


void
on_button_popup_cancel_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    gint i;
    gint status[3];
    GtkWidget *popup_window = 
	lookup_widget(GTK_WIDGET(button), "popup_window");
    GtkWidget *label_popup_status[3];
    
    label_popup_status[0] = 
	lookup_widget(popup_window, "label_popup_status");
    label_popup_status[1] = 
	lookup_widget(popup_window, "label_popup_status2");
    label_popup_status[2] = 
	lookup_widget(popup_window, "label_popup_status3");

    for(i=0;i<3;i++)
	status[i] = 
	    get_int_from_label(GTK_LABEL(label_popup_status[i]));

    change_popups_active(-1);
    gtk_widget_destroy(popup_window);

    if(status[0] == 500001 ||
       status[0] == 500011)
	on_button_quit_clicked(NULL, GINT_TO_POINTER(1));
}


gboolean
on_popup_window_delete_event           (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
    GtkWidget *button_popup_cancel =
	lookup_widget(widget, "button_popup_cancel");

    on_button_popup_cancel_clicked(GTK_BUTTON(button_popup_cancel),
				   NULL);

  return FALSE;
}


void
on_popup_check_toggled                 (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    gint i;
    gint status[3];
    GtkWidget *popup_window =
	lookup_widget(GTK_WIDGET(togglebutton), "popup_window");
    GtkWidget *label_popup_status[3];
    
    label_popup_status[0] = 
	lookup_widget(popup_window, "label_popup_status");
    label_popup_status[1] = 
	lookup_widget(popup_window, "label_popup_status2");
    label_popup_status[2] = 
	lookup_widget(popup_window, "label_popup_status3");
    
    for(i=0;i<3;i++)
	status[i] = 
	    get_int_from_label(GTK_LABEL(label_popup_status[i]));

    if(status[0] >= 100 && status[0] < 200)
	options[status[0] - 100] = 
	    (options[status[0] - 100] + 1 ) % 2;
    else if(status[0] == 500000)
	options[OPT_JOBS] = 
	    (options[OPT_JOBS] + 1) % 2;
    else if(status[0] == 0)
	options[OPT_NOTIFY] = 
	    gtk_toggle_button_get_active(togglebutton);
    
    set_save(0);
}



void
on_button_popup_close_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    on_button_popup_cancel_clicked(button, NULL);
}


/************************************************************
 * callbacks for the help window                            *
 ************************************************************/

void
on_button_help_close_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *help_window =
	lookup_widget(GTK_WIDGET(button), "help_window");

    gtk_widget_destroy(help_window);
}
