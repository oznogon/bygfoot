/******************************************************************
 * Miscellaneous functions that deal with the gui                 *
 ******************************************************************/

#include <time.h>
#include <unistd.h>

#include "callbacks.h"
#include "gui.h"
#include "maths.h"
#include "misc.h"
#include "support.h"
#include "team.h"
#include "treeview.h"

/* list of pixmap directories */
static GList *support_directories = NULL;

/* add 'directory' plus subdirectories recursively to the list
   we browse when looking for files (e.g. pixmaps) */
void
add_support_directory_recursive                   (const gchar     *directory)
{
    GDir *newdir =
	g_dir_open(directory, 0, NULL);
    const gchar *file;
    gchar *fullpath;

    if(newdir == NULL)
	return;

    support_directories = g_list_prepend (support_directories,
					     g_strdup (directory));

    while(TRUE)
    {
	file = g_dir_read_name(newdir);

	if(file == NULL)
	    break;

	fullpath = g_strdup_printf ("%s%s%s", directory,
				    G_DIR_SEPARATOR_S, file);

	if(g_file_test(fullpath, G_FILE_TEST_IS_DIR))
	    add_support_directory_recursive(fullpath);
	
	g_free(fullpath);
    }

    g_dir_close(newdir);
}

/* add 'directory' both to our directory list and
   GLADE's list */
void
add_support_directory(const gchar *directory)
{
    add_pixmap_directory(directory);
    add_support_directory_recursive(directory);
}

gchar*
find_support_file                       (const gchar     *filename)
{
  GList *elem;

  /* We step through each of the pixmaps directory to find it. */
  elem = support_directories;
  while (elem)
    {
      gchar *pathname = g_strdup_printf ("%s%s%s", (gchar*)elem->data,
                                         G_DIR_SEPARATOR_S, filename);
      if (g_file_test (pathname, G_FILE_TEST_EXISTS))
        return pathname;

      g_free (pathname);
      elem = elem->next;
    }
  return NULL;
}

/* keep track whether the game is saved or not */
void
set_save(gboolean status)
{
    GtkWidget *button_save =
	lookup_widget(main_window, "button_save");

    save_status = status;
    
    gtk_widget_set_sensitive(button_save,
			     (status != 1));
}

/* get an integer from a label */
gint
get_int_from_label(GtkLabel *label)
{
    const gchar *string;
    gint number;
    
    string = gtk_label_get_text(label);
    
    number = (gint)strtol(string, NULL, 10);
    
    return number;
}

/* set or append 'text' into 'label' */
void
label_set_text(GtkLabel *label, gchar *text, gint append)
{
    const gchar *current_text = gtk_label_get_text(label);
    gchar buf[SMALL];

    strcpy(buf, "");

    if(append == 0)
	strcpy(buf, text);
    else
	sprintf(buf, "%s%s", current_text, text);

    gtk_label_set_text(label, buf);
}

/* set or append 'number' as text into 'label' */
void
label_set_text_from_int(GtkLabel *label, gint number, gint append)
{
    const gchar *current_text = gtk_label_get_text(label);
    gchar buf[SMALL];
    
    strcpy(buf, "");

    if(append == 0)
	sprintf(buf, "%d", number);
    else
	sprintf(buf, "%s%d", current_text, number);

    gtk_label_set_text(label, buf);
}

/* set or append 'number' as text into 'label' */
void
label_set_text_from_float(GtkLabel *label, gfloat number,
			       gint append, gint precision)
{
    const gchar *current_text = gtk_label_get_text(label);
    gchar buf[SMALL];
    
    strcpy(buf, "");

    if(append == 0)
	sprintf(buf, "%.*f", precision, number);
    else
	sprintf(buf, "%s%.*f", current_text, precision, number);

    gtk_label_set_text(label, buf);
}

/* change the 'popups_active' variable. this determines whether
   the main window is set sensitive or not */
void
change_popups_active(gint difference)
{
    if(main_window == NULL)
	return;

    popups_active += difference;

    if(popups_active == 0)
	gtk_widget_set_sensitive(main_window, TRUE);
    else
	gtk_widget_set_sensitive(main_window, FALSE);
}

/* get the back- and foreground colors
   of a table row depending on the rank
   it represents */
void
get_table_colors(gint team_id,
		      gchar *background_color,
		      gchar *foreground_color)
{
    gchar foreground[50],
	background[50];

    strcpy(foreground, "Black");
    strcpy(background, "White");

    /* the league leader */
    if(team_id < 114 && rank[team_id] == 1)
	strcpy(background, "tomato");
    /* teams that get promoted or cl teams that
       are qualified for the quarter final */
    else if( (team_id > 113 && rank[team_id] < 3 && rank[team_id] > 0) ||
	     (team_id < 20 && rank[team_id] < 5) ||
	     (team_id < 114 && rank[team_id] < 4) )
	strcpy(background, "Lightblue");
    /* the teams that would play promotion games */
    else if(rank[team_id] < 8 && team_id > 19 && team_id < 114)
	strcpy(background, "khaki");
    /* thet teams that get relegated */
    else if(rank[team_id] > 
	    teams_in_league(get_league_from_id(team_id)) - 4 &&
	    team_id < 92)
	strcpy(background, "Lightgreen");

    if(foreground_color != NULL)
	strcpy(foreground_color, foreground);
    if(background_color != NULL)
	strcpy(background_color, background);
}

/* print a message into the message window */
void
print_message(gchar *text)
{
    gint i, j;
    gchar buf[strlen(text) + 200];
    GtkWidget *message_window =
	lookup_widget(main_window, "message_window");

    for(i = 0; i <= 100 - options[OPT_MESS] * 50; i++)
    {
	strcpy(buf, "");
	for(j=0;j<100 - options[OPT_MESS] * 50 - i;j++)
	    strcat(buf, " ");
	
	strcat(buf, text);
	gtk_entry_set_text(GTK_ENTRY(message_window), buf);
	
	while(gtk_events_pending())
	    gtk_main_iteration();
	
	usleep(10000);	
    }
}

/* print a number into an entry field */
void
entry_set_text_from_int(GtkEntry *entry, gint number)
{
    gchar buf[SMALL];

    sprintf(buf, "%d", number);

    gtk_entry_set_text(entry, buf);
}

/* return the entry content as an integer */
gint
entry_get_int(GtkEntry *entry)
{
    const gchar *text = gtk_entry_get_text(entry);

    gint number = (gint)strtol(text, NULL, 10);

    return number;
}

/* set the week number, season number, team name, team league
   and team rank labels */
void
set_header(void)
{
    gchar buf[SMALL];
    GtkWidget *label_season = 
	lookup_widget(main_window, "label_season");
    GtkWidget *label_week =
	lookup_widget(main_window, "label_week");
    GtkWidget *label_team =
	lookup_widget(main_window, "label_team");
    GtkWidget *label_league =
	lookup_widget(main_window, "label_league");
    GtkWidget *label_rank =
	lookup_widget(main_window, "label_rank");
    GtkWidget *label_money =
	lookup_widget(main_window, "label_money");
    GtkWidget *eventbox_rank =
	lookup_widget(main_window, "eventbox_rank");
    GtkWidget *label_av_skill = 
	lookup_widget(main_window, "label_av_skill");

    gchar background_color[50];
    GdkColor color;

    label_set_text_from_int(GTK_LABEL(label_season), season, 0);
    label_set_text_from_int(GTK_LABEL(label_week), week, 0);
    label_set_text(GTK_LABEL(label_team), teams[my_team].name, 0);
    if(my_team < 130)
	label_set_text_from_int(GTK_LABEL(label_rank), rank[my_team], 0);
    else
	label_set_text(GTK_LABEL(label_rank), "none", 0);

    print_grouped_int(finances[FIN_MONEY], buf, 0);
    label_set_text(GTK_LABEL(label_money), buf, 0);

    if(finances[FIN_MONEY] >= 0)
	gdk_color_parse ("black", &color);
    else
	gdk_color_parse ("Orangered", &color);    
    gtk_widget_modify_fg (label_money, GTK_STATE_NORMAL, &color);

    get_table_colors(my_team, background_color, NULL);
    gdk_color_parse(background_color, &color);
    gtk_widget_modify_bg(eventbox_rank, GTK_STATE_NORMAL, &color);

    get_league_name_from_id(my_team, buf);
    label_set_text(GTK_LABEL(label_league), buf, 0);

    sprintf(buf, "%.2f  %.2f", average_skill(my_team, 11, TRUE),
	    average_skill(my_team, 20, FALSE));
    if(get_place(status, 12) != 13 &&
       get_place(status, 12) != 50)
	gtk_label_set_text(GTK_LABEL(label_av_skill), buf);
}

/* adjust the widget properties in the main window
   (mainly the buttons) depending on the window status
   (i.e. in which sub-menu the human player's in) */
void
set_buttons(void)
{
    GtkWidget *optionmenu_figures =
	lookup_widget(main_window, "optionmenu_figures");
    GtkWidget *button_transfer_ok =
	lookup_widget(main_window, "button_transfer_ok");
    GtkWidget *spin_fee = 
	lookup_widget(main_window, "spin_fee");
    GtkWidget *spin_wage =
	lookup_widget(main_window, "spin_wage");
    GtkWidget *label_scout_recommends =
	lookup_widget(main_window, "label_scout_recommends");
    GtkWidget *label_spin =
	lookup_widget(main_window, "label_spin");
    GtkWidget *button_browse =
	lookup_widget(main_window, "button_browse");
    GtkWidget *button_browse_back =
	lookup_widget(main_window, "button_browse_back");
    GtkWidget *player_info_separator =
	lookup_widget(main_window, "player_info_separator");
    GtkWidget *button_league_results =
	lookup_widget(main_window, "button_league_results");
    GtkWidget *button_get_loan =
	lookup_widget(main_window, "button_get_loan");
    GtkWidget *button_fixtures = 
	lookup_widget(main_window, "button_fixtures");
              
    gtk_widget_hide(button_browse_back->parent->parent);
    gtk_widget_hide(button_browse->parent);
    gtk_widget_hide(button_transfer_ok->parent);
    gtk_widget_hide(spin_wage->parent);
    gtk_widget_hide(spin_fee->parent);
    gtk_widget_hide(label_scout_recommends);
    gtk_widget_hide(player_info_separator);
    gtk_widget_hide(button_league_results);
    gtk_widget_hide(button_get_loan->parent->parent);
    gtk_widget_hide(button_fixtures->parent->parent);

    gtk_option_menu_set_history(
	GTK_OPTION_MENU(optionmenu_figures), 0);
    
    if(status == 0 || get_place(status, 12) == 50)
	gtk_widget_show(player_info_separator);
    else if(get_place(status, 6) == 1)
    {
	gtk_widget_show(button_browse->parent);
	if(get_place(status, 5) == 1)
	{
	    gtk_widget_show(button_transfer_ok->parent);
	    gtk_widget_show(label_scout_recommends);
	    gtk_widget_show(spin_fee->parent);
	    gtk_widget_show(spin_wage->parent);
	}
	else if(get_place(status, 5) == 3)
	    gtk_widget_show(button_browse_back->parent->parent);
    }
    else if(get_place(status, 6) == 2)
    {
	if(get_place(status, 5) < 3)
	{
	    gtk_widget_show(button_league_results);
	    gtk_widget_show(button_fixtures->parent->parent);
	}

	if(get_place(status, 5) != 4 ||
	   season > 2)
	    gtk_widget_show(button_browse_back->parent->parent);
	else
	    gtk_widget_show(player_info_separator);
    }
    else if(get_place(status, 6) == 3)
    {
	gtk_widget_show(button_get_loan->parent->parent);
	if(get_place(status, 5) == 1 ||
	   get_place(status, 5) == 2)
	{
	    gtk_widget_show(button_transfer_ok->parent);
	    gtk_widget_show(spin_fee->parent);
	    label_set_text(GTK_LABEL(label_spin), "Max.", 0);
	}
    }
    else if(get_place(status, 6) == 4)
    {
	gtk_widget_show(button_fixtures->parent->parent);
    }
    else if(get_place(status, 6) == 6)
    {
	gtk_widget_show(button_fixtures->parent->parent);

	gtk_widget_show(button_browse_back->parent->parent);
    }
    
    set_header();
}

/* show the main window with the human player's team in the
   player list */
void
initialize_main_window(void)
{
    gint i;
    GtkWidget *optionmenu_quick_opt =
	lookup_widget(main_window, "optionmenu_quick_opt");
    GtkWidget *optionmenu_scout = 
	lookup_widget(main_window, "optionmenu_scout");
    GtkWidget *optionmenu_physio = 
	lookup_widget(main_window, "optionmenu_physio");
    GtkWidget *optionmenu_style = 
	lookup_widget(main_window, "optionmenu_style");
    GtkWidget *optionmenu_fixtures = 
	lookup_widget(main_window, "optionmenu_fixtures");
    GtkWidget *entry_structure =
	lookup_widget(main_window, "entry_structure");
    GtkWidget *quick_opt_items[4];
    gint quick_opt_options[4] =
	{options[OPT_NOTIFY],
	 options[OPT_JOBS],
	 options[OPT_SHOW_LIVE],
	 options[OPT_OVERWRITE]};
    
    quick_opt_items[0] = 
	lookup_widget(optionmenu_quick_opt, "menu_item_notify");
    quick_opt_items[1] = 
	lookup_widget(optionmenu_quick_opt, "menu_item_jobs");
    quick_opt_items[2] = 
	lookup_widget(optionmenu_quick_opt, "menu_item_live");
    quick_opt_items[3] = 
	lookup_widget(optionmenu_quick_opt, "menu_item_overwrite");

    show_next_opponents();

    show_players(NULL, NULL, 0, NULL, 0);

    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu_style),
				teams[my_team].style + 2);
    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu_scout),
				scout - 1);
    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu_physio),
				physio - 1);
    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu_quick_opt),
				0);
    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu_fixtures), 0);

    entry_set_text_from_int(GTK_ENTRY(entry_structure),
			    teams[my_team].structure);

    gtk_check_menu_item_set_active(
	GTK_CHECK_MENU_ITEM(quick_opt_items[0]), 
	(quick_opt_options[0] < 100));

    for(i=0;i<4;i++)
	gtk_check_menu_item_set_active(
	    GTK_CHECK_MENU_ITEM(quick_opt_items[i]),
	    quick_opt_options[i]);

    set_header();
}
