/******************************************************************
 * Miscellaneous functions that deal with the gui                 *
 ******************************************************************/

#include <time.h>
#include <unistd.h>
#include "callbacks.h"
#include "gui_functions.h"
#include "support.h"

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
    gchar buf[BUF_SIZE_SMALL];

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
    gchar buf[BUF_SIZE_SMALL];
    
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
    gchar buf[BUF_SIZE_SMALL];
    
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
    gchar buf[BUF_SIZE_SMALL];

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
    gchar buf[BUF_SIZE_SMALL];
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
	if(get_place(status, 5) == 1)
	    gtk_widget_show(button_league_results);

	if(get_place(status, 5) < 3)
	    gtk_widget_show(button_fixtures->parent->parent);

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

/* make new bookmaker tips in case the user's
   team has changed significantly */
void
bookmaker_re_tip(void)
{
    bookmaker_tips[0][0] =
	bookmaker_tips[1][0] = -1;

    if(status == 0)
	on_button_back_to_main_clicked(NULL, NULL);
}

/* show a popup window with an offer for a 
   player to transfer */
void
make_transfer_offer(gint idx)
{
    gchar buf[BUF_SIZE_SMALL];
    gint popup_status[3] = {1000000 + idx, -1, -1};
    gint player_number = transferlist[idx].player_number;
    gfloat value_deviance[2] =
	{0.98 - (scout % 10) * 0.03,
	 1.15 - (scout % 10) * 0.03};
    gint value = round_integer(
	rnd( teams[my_team].players[player_number].value *
	     value_deviance[0],
	     teams[my_team].players[player_number].value *
	     value_deviance[1] ), 2);
    gint team_interested = rndi(0, 174);

    while(team_interested == my_team ||
	  team_interested == 114 || 
	  team_interested == 130)
	team_interested = rndi(0, 174);
    
    popup_status[1] = team_interested;
    popup_status[2] = value;

    sprintf(buf, "%s are interested in buying %s. They offer ",
	    teams[team_interested].name,
	    teams[my_team].players[player_number].name);
    print_grouped_int(value, buf, 1);
    strcat(buf, " for him, which is ");
    print_grouped_int(abs(value - teams[my_team].
			  players[player_number].value),
		      buf, 1);
    if(value > teams[my_team].players[player_number].value)
	strcat(buf, "more ");
    else
    	strcat(buf, "less ");
    
    strcat(buf, 
	   "than the player's value according to your scout. Accept?");
    
    show_popup_window(buf, popup_status);
}

/* check for each player of the human player's team
   on the transferlist whether he'll be bought or not */
void
transfer_offers(void)
{
    gint i;
    /* bad scout: 10% probability ---> best scout: 50% */
    gfloat buy_prob =
	(gfloat)(scout % 10) * -0.13 + 0.615;

    if(week >= 35)
	return;

    for(i=0;i<20;i++)
	if(transferlist[i].time > 0 &&
	   transferlist[i].team_id == my_team &&
	   rnd(0,1) < buy_prob)
	    make_transfer_offer(i);
}

gint
get_current_rank(void)
{
    gint i;

    for(i=0;i<114;i++)
	if(rank_ids[i] == my_team)
	    return i;

    return -1;
}

/* show a popup window with either a job offer
   or a note that the human player's been fired */
void
show_job_offer(gint fire)
{
    gchar buf[JOB_OFFER_END][BUF_SIZE_SMALL];
    gchar buf2[BUF_SIZE_SMALL];
    gint new_team = my_team;
    gint new_team_bound[2];
    /* get the my_team index in the rank_ids array */
    gint current_rank = 
	get_current_rank();

    /* if he's been fired, the team to hire him should
       be worse than his old one */
    new_team_bound[0] = (current_rank - 30 + (fire % 10) * 20 < 0) ?
	0 : current_rank - 30 + (fire % 10) * 20;
    new_team_bound[1] = (current_rank + 10 + (fire % 10) * 20 > 113) ?
	113 : current_rank + 10 + (fire % 10) * 20;

    while(new_team == my_team)
	new_team = rndi(rank_ids[new_team_bound[0]],
			rank_ids[new_team_bound[1]]);

    status = 900000 + new_team + 50000 * (fire != 0);

    sprintf(buf[JOB_OFFER_ACCEPT], "Accept?");

    if(fire != 0)
    {
	if(fire == 1)
	    sprintf(buf[JOB_OFFER_TEXT], "The team owners fire you because of unsuccessfulness.");
	else if(fire == 11)
	    sprintf(buf[JOB_OFFER_TEXT], "The team owners fire you because of financial mismanagement.");

	sprintf(buf2, "\nBut the owners of %s have heard of your dismissal and would like to hire you. Here's some info on %s:",
		teams[new_team].name,
		teams[new_team].name);
	strcat(buf[JOB_OFFER_TEXT], buf2);
	strcat(buf[JOB_OFFER_ACCEPT], " (NOTE: If you don't, the game is over.)");
    }
    else
	sprintf(buf[JOB_OFFER_TEXT], "The owners of %s are impressed by your success with %s. They would like to hire you. Here's some info on %s:",
		teams[new_team].name,
		teams[my_team].name,
		teams[new_team].name);
    
    get_league_name_from_id(new_team, buf2);

    strcpy(buf[JOB_OFFER_NAME], teams[new_team].name);
    strcpy(buf[JOB_OFFER_LEAGUE], buf2);
    sprintf(buf[JOB_OFFER_RANK], "%d", rank[new_team]);
    print_grouped_int(
	round_integer((gint)rint( rnd(0.7, 0.95) *
				  (gfloat)stadiums[new_team].capacity * 113), -2),
	buf[JOB_OFFER_MONEY],
	0);
    print_grouped_int(stadiums[new_team].capacity, buf[JOB_OFFER_CAP], 0);
    sprintf(buf[JOB_OFFER_SAF], "%.0f%%", 
	    stadiums[new_team].safety * 100);

    show_job_offer_window(buf);
}

void
show_fire_warning(void)
{
    gchar buf[BUF_SIZE_SMALL];

    sprintf(buf, "The team owners are dissatisfied with the team's recent performance. There are rumours they're looking for a new coach.");

    show_popup_window(buf, NULL);
}

/* look at the counters and fire the human player 
   if his team is unsuccessful; or make a job offer
   if they're successful */
void
team_offers(void)
{    
    gint i;
    gfloat rndom;

    if(abs(counters[COUNT_SUCCESS]) < 45)
	return;

    if(counters[COUNT_SUCCESS] < -45 &&
       counters[COUNT_SUCCESS] > -60 &&
       counters[COUNT_WARNING] == 0)
    {
	show_fire_warning();
	counters[COUNT_WARNING] = 1;	
	return;
    }
    
    /* fire */
    for(i=0;i<3;i++)
    {
	rndom = rnd(0,1);
	if(counters[COUNT_SUCCESS] <= -95 + i * 10 &&
	   rndom < 0.8 - i * 0.25)
	{
	    show_job_offer(1);
	    return;
	}
    }

    if(options[OPT_JOBS] == 0)
	return;

    /* hire */
    rndom = rnd(0,1);
    for(i=0;i<3;i++)
    {
	if(counters[COUNT_SUCCESS] >= 95 - i * 10)
	{
	    if(rndom < 0.3 - i * 0.1)
		show_job_offer(0);
	    
	    return;
	}
    }
}

/* autosave the game if necessary */
void
update_autosave(void)
{
    gchar buf[BUF_SIZE_SMALL];
    
    if(options[OPT_AUTOSAVE] < 0)
	return;

    counters[COUNT_AUTOSAVE]--;

    if(counters[COUNT_AUTOSAVE] > 0)
	return;

    counters[COUNT_AUTOSAVE] = options[OPT_AUTOSAVE];

    sprintf(buf, "%s/.bygfoot/saves/autosave", getenv("HOME"));

    save_game(buf);
}

void
get_best_players(gint league, gint best_players[][2])
{
    gint i, j;
    gint bound[2];
    gfloat to_order[480];
    gint order[480];

    for(i=0;i<480;i++)
	to_order[i] = -1;
    
    get_league_bounds(league, bound);

    for(i=bound[0];i<bound[1];i++)
	for(j=0;j<20;j++)
	    if(teams[i].players[j].pos >= 0)
	    {
		to_order[(i - bound[0]) * 20 + j] = 
		    (gfloat)teams[i].players[j].goals;

		if(teams[i].players[j].games > 0)
		    to_order[(i - bound[0]) * 20 + j] +=
			((gfloat)teams[i].players[j].goals /
			 ((gfloat)teams[i].players[j].games * 10));
	    }
    
    sort_float_array(to_order, order, 0, 479);

    j = 0;
    for(i=0;i<480;i++)
	if(teams[bound[0] + (order[i] - order[i] % 20) / 20].
	   players[order[i] % 20].pos > 0 && j < 10 &&
	   to_order[order[i]] != -1)
	{
	    best_players[j][0] = 
		bound[0] + (order[i] - order[i] % 20) / 20;
	    best_players[j][1] = order[i] % 20;
	    j++;
	}

    j = 0;
    for(i=479;i>=0;i--)
	if(teams[bound[0] + (order[i] - order[i] % 20) / 20].
	   players[order[i] % 20].pos == 0 &&
	   teams[bound[0] + (order[i] - order[i] % 20) / 20].
	   players[order[i] % 20].games >=
	   (gfloat)teams[bound[0] + (order[i] - order[i] % 20) / 20].
	   results[RES_GAM] / 2 && j < 10 &&
	   to_order[order[i]] != -1)
	{
	    best_players[10 + j][0] = 
		bound[0] + (order[i] - order[i] % 20) / 20;
	    best_players[10 + j][1] = order[i] % 20;
	    j++;
	}
}

void
show_results(gint page)
{
    if( (page == 3 && options[OPT_SHOW_MY_GAMES] == 0)
	|| page == 0 )
    {
	show_fixtures(week - 1);
	status = 600011;
    }
    else if( (page == 3 && options[OPT_SHOW_MY_GAMES] == 1)
	     || page == 1 )
    {
	callback_show_preview();
	status = 600010;
    }
}

/* update some variables each week */
void
update_variables(void)
{
    gint i, j;

    for(i=0;i<2;i++)
	for(j=0;j<2;j++)
	    bookmaker_tips[i][j] = -1;
}

void
finance_events(void)
{
    gint i;
    gchar buf[BUF_SIZE_SMALL];
    gchar buf2[BUF_SIZE_SMALL];

    if(counters[COUNT_LOAN] == 0)
	show_popup_window("You've got to pay back your loan NOW!!!",
			  NULL);

    if(counters[COUNT_LOAN] == -1 ||
       counters[COUNT_POSITIVE] == -1 ||
       counters[COUNT_OVERDRAWN] == 4)
    {
	show_job_offer(11);
	return;
    }

    if(counters[COUNT_POSITIVE] == 0)
	show_popup_window(
	    "Your bank account has to exceed your drawing credit next week!!!",
	    NULL);

    for(i=0;i<3;i++)
	if(counters[COUNT_OVERDRAWN] == i + 1 &&
	   counters[COUNT_POSITIVE] == 6 - i * 2)
	{
	    if(counters[COUNT_OVERDRAWN] == 1)
		sprintf(buf, "You have overdrawn your bank account. ");
	    else
		sprintf(buf, "You have overdrawn your bank account once again. ");
	    
	    sprintf(buf2, "The team owners give you %d weeks to get positive.",
		    counters[COUNT_POSITIVE]);

	    strcat(buf, buf2);
	    show_popup_window(buf, NULL);
	}    
}

gboolean
my_team_will_play(void)
{
    gint i;

    for(i = FIX_END - 1; i >= 0; i--)
	if(fixtures[i].type != -1 &&
	   my_team_involved(fixtures[i]) &&
	   fixtures[i].week_number > week)
	    return TRUE;

    return FALSE;
}

/* find out the winners of the promotion games */
void
get_promotion_winners(gint *promotion_winners)
{
    gint i, k;

    k = 0;

    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type > 55000 &&
	   fixtures[i].type < 60000 &&
	   fixtures[i].type % 10 == 5)
	{
	    promotion_winners[k] = winner_of(fixtures[i]);
	    k++;
	}
}

/* promotions and relegations */
void
season_end_prom_rel(void)
{
    gint i, j;
    gint limits[4] = {20, 44, 68, 92};
    gint promotion_winners[4];
    gint my_old_league = 
	get_league_from_id(my_team);

    /* swap the first and last three
       in the tables */
    for(i=0;i<4;i++)
	for(j=0;j<3;j++)
	    swap_teams(rank_ids[limits[i] + j], 
		       rank_ids[limits[i] - j - 1]);

    get_promotion_winners(promotion_winners);

    for(i=0;i<4;i++)
	swap_teams(promotion_winners[i], rank_ids[limits[i] - 4]);

    /* reward or punish the human player if he's
       promoted or relegated */
    if(my_old_league < get_league_from_id(my_team))
	counters[COUNT_SUCCESS] -= 30;
    else if(my_old_league > get_league_from_id(my_team))
	counters[COUNT_SUCCESS] += 30;
}

/* generate new european teams
   and get the right engl. teams */
void
season_end_euro(void)
{
    gint i;

    /* set english participants */
    teams[114].id = rank_ids[0];
    /* if there's a double winner the runner-up takes part
       in the CWC */
    teams[130].id = (get_winner_runner_up(9000, 1) == rank_ids[0]) ?
	get_winner_runner_up(9000, 0) : 
	get_winner_runner_up(9000, 1);
    
    for(i=0;i<3;i++)
	if(rank_ids[i + 1] != teams[130].id)
	    teams[175 + i].id = rank_ids[i + 1];
	else
	    teams[175 + i].id = rank_ids[4];

    fill_in_euro_teams();
}

/* nullify some counters and
   number of games and goals etc. */
void
season_end_nullify(void)
{
    gint i, j;

    counters[COUNT_OVERDRAWN] =
	counters[COUNT_WARNING] = 0;

    if(counters[COUNT_SUCCESS] > 100)
	counters[COUNT_SUCCESS] = 100;

    for(i=0;i<130;i++)
    {
	for(j=0;j<RES_END;j++)
	    teams[i].results[j] = 0;

	for(j=0;j<20;j++)
	{
	    teams[i].players[j].games =
		teams[i].players[j].goals = 
		teams[i].players[j].booked = 0;
	}
    }

    for(i=0;i<178;i++)
	stadiums[i].games =
	    stadiums[i].average_attendance = 0;
    for(i=0;i<FIX_END;i++)
	fixtures[i].type = -1;
    for(i=0;i<50;i++)
    {
	goals[0][i].minute = goals[1][i].minute = -1;

	if(i<20)
	    transferlist[i].time = -1;
	if(i<11)
	    injuries[i] = booked[i] = -1;
	if(i<2)
	    stadium_facts[i][0] =
		stadium_facts[i][1] = 0;
	if(i > FIN_MONEY && i < FIN_DEBTS)
	    finances[i] = 0;
    }
}

void
change_player(player *pl, gfloat factor)
{
    pl->skill =
	pl->cskill =
	pl->skill * factor;
    if(pl->skill > 9.9)
	pl->skill =
	    pl->cskill = rnd(9.7, 9.9);
		
    pl->etal =
	estimate_talent(*pl);
    
    pl->value = assign_value(*pl);
    pl->wage = assign_wage(*pl);
}

/* change each team slightly to have
   something new in the new season */
void
season_end_change_teams(void)
{
    gint i, j;
    gfloat factor;

    for(i=0;i<114;i++)
	if(i != my_team)
	{
	    factor = rnd(0.92, 1.08);
	    /* if the human player's the
	       champion, make the other teams rather
	       better than worse */
	    if(rank_ids[0] == my_team && i < 20)
		factor += 0.04;
	    
	    for(j=0;j<20;j++)
		change_player(&(teams[i].players[j]), factor);
	}
}

season_stat*
get_new_stat(void)
{
    season_stat *stat = history;

    if(history == NULL)
    {
	stat = (season_stat*)g_malloc(sizeof(season_stat));
	stat->next = NULL;
	history = stat;
	return stat;
    }

    while(stat->next != NULL)
	stat = stat->next;

    stat->next = (season_stat*)g_malloc(sizeof(season_stat));
    stat->next->next = NULL;

    return stat->next;
}

void
history_get_champions(season_stat *stat)
{
    gint i;

   /* english champions */
    strcpy(stat->team_names[STAT_PREM],
	   teams[rank_ids[0]].name);
    strcpy(stat->team_names[STAT_NAT_CONF],
	   teams[rank_ids[92]].name);    
    for(i=0;i<3;i++)
	strcpy(stat->team_names[STAT_DIV1 + i],
	       teams[rank_ids[20 + i * 24]].name);
    
    /* fa cup, league cup, charity shield */
    strcpy(stat->team_names[STAT_FA], 
	   teams[get_winner_runner_up(9000, 1)].name);
    strcpy(stat->team_names[STAT_LEAGUE], 
	   teams[get_winner_runner_up(11000, 1)].name);
    strcpy(stat->team_names[STAT_CHARITY], 
	   teams[get_winner_runner_up(25000, 1)].name);

    /* european champions */
    strcpy(stat->team_names[STAT_CL],
	   teams[get_winner_runner_up(6000, 1)].name);
    strcpy(stat->team_names[STAT_CWC],
	   teams[get_winner_runner_up(7000, 1)].name);
    strcpy(stat->team_names[STAT_UEFA],
	   teams[get_winner_runner_up(8815, 1)].name);
    strcpy(stat->team_names[STAT_SUPERCUP],
	   teams[get_winner_runner_up(35000, 1)].name);
}

void
history_get_best_players(season_stat *stat)
{
    gint i, j, k;
    gint best_players[20][2];
    
    for(k=0;k<2;k++)    
	for(i=0;i<5;i++)
	{
	    get_best_players(i + 1, best_players);
	    for(j=0;j<3;j++)
	    {
		stat->best_players[k * 15 + i * 3 + j].games =
		    teams[best_players[k * 10 + j][0]].
		    players[best_players[k * 10 + j][1]].games;
		stat->best_players[k * 15 + i * 3 + j].goals =
		    teams[best_players[k * 10 + j][0]].
		    players[best_players[k * 10 + j][1]].goals;
		strcpy(stat->best_players[k * 15 + i * 3 + j].name,
		       teams[best_players[k * 10 + j][0]].
		       players[best_players[k * 10 + j][1]].name);
		strcpy(stat->best_players[k * 15 + i * 3 + j].team_name,
		       teams[best_players[k * 10 + j][0]].name);
	    }
	}
}

/* return the rank of the human player if
   he's played in a regular championship
   or his result if he's played in a cup only */
gint
history_get_rank(void)
{
    gint i;
    gint int_cup;

    if(my_team < 114)
	return rank[my_team];

    if(my_team < 130)
	int_cup = 6;
    else if(my_team < 145)
	int_cup = 7;
    else
	int_cup = 8;

    for(i = FIX_END - 1; i >= 0; i--)
	if(my_team_involved(fixtures[i]) &&
	   get_place(fixtures[i].type, 11) == int_cup)
	    break;

    if(i == -1)
	return 0;

    if(get_place(fixtures[i].type, 12) == 65)
	return 65;

    return get_place(fixtures[i].type, 12) * 
	(1 - 2 * (winner_of(fixtures[i]) == my_team));
}

/* fill in a new history element at
   the end of the season with cup winners
   and champions etc. */
void
write_new_history(void)
{
    season_stat *stat =
	stat = get_new_stat();
    
    stat->season_number = season;
    stat->my_league = get_league_from_id(my_team);
    stat->my_rank = history_get_rank();

    strcpy(stat->team_names[STAT_MY_TEAM], teams[my_team].name);
    
    history_get_champions(stat);

    history_get_best_players(stat);
}

/* end a season and begin a new one by promoting /
   relegating the appropriate teams; find out who's
   participating in the european cups, write new fixtures
   etc. */
void
season_end(void)
{
    write_new_history();
    
    /* promotions and relegations */
    season_end_prom_rel();

    /* change each team slightly to have
       something new in the new season */
    season_end_change_teams();
    
    /* generate new european teams
       and get the right engl. teams */
    season_end_euro();

    /* nullify some counters and
       number of games and goals etc. */
    season_end_nullify();

    write_season_fixtures();
}

/* change the country in the team selection window
   when the user clicks on a flag */
void
change_country_team_selection(GtkWidget *button)
{
    gint i;
    GtkWidget *radiobutton_country[TEXT_FILES_PLAYER_NAMES];
    GtkWidget *team_selection_treeview =
	lookup_widget(button, "team_selection_treeview");    

    radiobutton_country[TEXT_FILES_COUNTRY_ENG] = 
	lookup_widget(button, "radiobutton_country0");
    radiobutton_country[TEXT_FILES_COUNTRY_DE] = 
	lookup_widget(button, "radiobutton_country1");
    radiobutton_country[TEXT_FILES_COUNTRY_IT] = 
	lookup_widget(button, "radiobutton_country2");
    radiobutton_country[TEXT_FILES_COUNTRY_FR] = 
	lookup_widget(button, "radiobutton_country3");
    radiobutton_country[TEXT_FILES_COUNTRY_ES] = 
	lookup_widget(button, "radiobutton_country4");
    radiobutton_country[TEXT_FILES_COUNTRY_RO] = 
	lookup_widget(button, "radiobutton_country5");
    radiobutton_country[TEXT_FILES_COUNTRY_BR] = 
	lookup_widget(button, "radiobutton_country6");
    radiobutton_country[TEXT_FILES_COUNTRY_PL] = 
	lookup_widget(button, "radiobutton_country7");

    for(i=0;i<TEXT_FILES_PLAYER_NAMES;i++)	
      if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_country[i])))
	  country_names(i, "");
    
    show_team_list(team_selection_treeview, 0);
}

gint
get_game_end(gint idx)
{
    gint i;
    gint goal_idx = 
	get_statistics_variable_index(fixtures[idx]);

    if(is_draw(fixtures[idx], 0))
	return 120;

    for(i=0;i<50;i++)
	if(goals[goal_idx][i].minute < 0)
	    break;

    if(i == 0)
	return rndi(90, 95);
    
    return (goals[goal_idx][i - 1].minute >= 90) ?
	goals[goal_idx][i - 1].minute + rndi(1, 2) :
	rndi(90, 95);
}

/* get the left/right probabilitiy for the tendency bar */
gfloat
get_tendency_prob(gint idx, gdouble value)
{
    gfloat attack_value[2];
    gfloat defend_value[2];
    gfloat goalie_value[2];
    gfloat home_advantage[2] = {1, 1};

    if(fixtures[idx].type % 1000 != 0 || 
       fixtures[idx].type < 6000)
	home_advantage[0] = 
	    gauss_dist(1.05, 1.05, 1.15, 1.15);

    prg_calculate_att_def(fixtures[idx], attack_value,
			  defend_value, goalie_value,
			  home_advantage);
    if(value == 0.5)
	return (attack_value[1] + defend_value[1]) /
	    (attack_value[0] + defend_value[0] +
	     attack_value[1] + defend_value[1]);

    if(value < 0.5)
	return attack_value[1] / (attack_value[1] + defend_value[0]);

    return attack_value[0] / (attack_value[0] + defend_value[1]);
}

/* get a new value for the tendency bar in the live game window */
gdouble
update_tendency(GtkWidget *hscale_tendency,
			gint minute, gint idx, gdouble *old_values,
			gfloat stick_prob)
{
    gint i;
    gint goal_idx =
	get_statistics_variable_index(fixtures[idx]);
    gint next_goal = 150;
    gint previous_goal = -100;
    gint team_id = 0;
    gint steps_to_goal;
    gint minutes_to_goal;    
    gfloat prob;
    gfloat rndom;
    gdouble current_value =
	gtk_range_get_value(GTK_RANGE(hscale_tendency));
    gdouble new_value;
    GdkColor color;

    prob = get_tendency_prob(idx, current_value);
    
    /* get the time of the next and previous goals */
    for(i=0;i<50;i++)
	if(goals[goal_idx][i].minute >= minute)
	{
	    next_goal = goals[goal_idx][i].minute;
	    team_id = (goals[goal_idx][i].team_id == 
		       fixtures[idx].team_id[0]) ?
		1 : 0;
	    break;	    
	}

    for(i=49;i>=0;i--)
	if(goals[goal_idx][i].minute < minute &&
	   goals[goal_idx][i].minute > 0)
	{
	    previous_goal = goals[goal_idx][i].minute;
	    break;	    
	}

    if(current_value == 0 || current_value == 1 ||
       next_goal == minute)
    {
	gdk_color_parse("red", &color);
	gtk_widget_modify_bg(hscale_tendency,
			     GTK_STATE_NORMAL, &color);
    }
    else
    {
	gdk_color_parse("Lightgrey", &color);
	gtk_widget_modify_bg(hscale_tendency,
			     GTK_STATE_NORMAL, &color);
    }

    if(next_goal == minute)
	return team_id;
    else if(minute - previous_goal < 4)
	return current_value;
    else if(minute - previous_goal < 6)
	return 0.5;

    steps_to_goal = 
	(gint)fabs((current_value - (gdouble)team_id) * 20);
    minutes_to_goal = next_goal - minute;

    if(minutes_to_goal <= steps_to_goal)
	return fabs(team_id - (gdouble)minutes_to_goal / 20);

    if(steps_to_goal - minutes_to_goal < 6)
	prob += 0.15 * (1 - 2 * team_id);
    else if(steps_to_goal - minutes_to_goal < 11)
	prob += 0.1 * (1 - 2 * team_id);

    if(old_values[0] != old_values[1])
	prob += (stick_prob * (old_values[0] - old_values[1]) * 20);

    rndom = rnd(0,1);
    if(rndom < prob - 0.05)
	new_value = current_value - 0.05;
    else if(rndom > prob + 0.05)
	new_value = current_value + 0.05;
    else
	new_value = current_value;

    if(new_value > 0.95)
	new_value = 0.98;
    if(new_value < 0.05)
	new_value = 0.02;

    return new_value;
}

void
show_live_game(gint idx)
{
    gint i, j, k;
    gint game_end;
    gint length;
    gint goal_idx =
	get_statistics_variable_index(fixtures[idx]);
    gdouble old_values[2] = {0, 0};
    gdouble new_value = 0;
    gfloat stick_prob = (gfloat)rndi(1, 3) / 10 + 0.05;
    gchar buf[BUF_SIZE_SMALL], 
	buf2[BUF_SIZE_SMALL], buf3[BUF_SIZE_SMALL];
    GtkWidget *live_window =
	return_live_window();
    GtkWidget *ruler =
	lookup_widget(live_window, "hruler_live");
    GtkWidget *progressbar_live =
	lookup_widget(live_window, "progressbar_live");
    GtkWidget *hscale_tendency =
	lookup_widget(live_window, "hscale_tendency");
    GtkWidget *button_live_close =
	lookup_widget(live_window, "button_live_close");
    GtkWidget *check_live_window_tendency =
	lookup_widget(live_window, "check_live_window_tendency");
    
    game_end = get_game_end(idx);

    if(fixtures[idx].type < 6000 ||
       get_place(fixtures[idx].type, 12) == 65)
	length = 97;
    else
	length = 121;

    if(options[OPT_LIVE_TENDENCY] == 0)
    {
	gtk_toggle_button_set_active(
	    GTK_TOGGLE_BUTTON(check_live_window_tendency),
	    FALSE);
	gtk_widget_hide(hscale_tendency);
    }
    else
	gtk_toggle_button_set_active(
	    GTK_TOGGLE_BUTTON(check_live_window_tendency),
	    TRUE);
    
    gtk_ruler_set_range(GTK_RULER(ruler),
			0, length, 0, length);

    strcpy(buf, "");
    strcpy(buf2, "");
    fixture_type_to_string(fixtures[idx].type,
			   0, buf);
    fixture_type_to_string(fixtures[idx].type,
			   1, buf2);
    sprintf(buf3, "%s  %s", buf, buf2);
    gtk_window_set_title(GTK_WINDOW(live_window), buf3);
    
    change_popups_active(1);
    gtk_widget_show(live_window);

    for(i=0;i<game_end * 4;i++)
    {
	gtk_progress_bar_set_fraction(
	    GTK_PROGRESS_BAR(progressbar_live),
	    (gdouble)i / (length * 4));

	if(i % 4 == 0)
	{
	    new_value = update_tendency(hscale_tendency,
					i / 4, idx, old_values,
					stick_prob);
	    gtk_range_set_value(GTK_RANGE(hscale_tendency), new_value);
	    old_values[0] = old_values[1];
	    old_values[1] = new_value;

	    show_live_window(live_window, idx, i / 4);
	}

	while(gtk_events_pending())
	    gtk_main_iteration();

	usleep(25000 + options[OPT_LIVE_DURATION] * 240);
    }

    /* no penalties*/
    if(fixtures[idx].result[0][2] + 
       fixtures[idx].result[1][2] == 0)
    {
	gtk_widget_show(button_live_close->parent);
	return;
    }

    for(i=0;i<50;i++)
	if(goals[goal_idx][i].type > 1)
	    break;

    for(k=i;k<50;k++)
	if(goals[goal_idx][k].minute > 0)
	{
	    for(j=1;j<3;j++)
	    {
		show_live_window(live_window, idx, j * 1000 + k + 1);

		while(gtk_events_pending())
		    gtk_main_iteration();	    
		usleep(1300000);	    
	    }
	}

    gtk_widget_show(button_live_close->parent);
}

/* show a live game */
void
live_game(gint number)
{
    gint i, k;
    gint idx[2] = {-1, -1};    

    if(number == 0)
	status = -100000;

    k = 0;
    for(i=FIX_END - 1;i>=0;i--)
	if(my_team_involved(fixtures[i]) &&
	   fixtures[i].week_number == week)
	{
	    idx[k] = i;
	    k++;
	}        

    /* no more games to show,
       so we set the status appropriately */
    if(idx[number] == -1)
    {
	status = -50000;
	return;
    }
    
    show_live_game(idx[number]);

    /* we have already shown the second game of the week
       or we only had one game this week */
    if(number == 1 || idx[1] == -1)
	status = -50000;

    return;
}

/* start a new week: compute games etc. */
void
callback_new_week(gboolean calculate)
{
    GtkWidget *player_list =
	lookup_widget(main_window, "player_list");
    
    if(calculate)
    {
	if(my_team_played(week))
	    gtk_tree_view_set_model(GTK_TREE_VIEW(player_list),
				    NULL);

	if(my_team_played(week) || my_team > 114)
	    update_finances();

	update_stadium();
	process_week_games(week);

	if(options[OPT_SHOW_LIVE] == 1)
	{
	    live_game(0);
	    return;
	}
    }

    update_ranks();
    update_fixtures();
    update_teams();

    week++;

    update_transferlist();

    prize_money();

    if(options[OPT_SKIP_WEEKS] == 1 ||
       !my_team_will_play())
	while(!my_team_played(week) &&
	      !my_team_played(week - 1) && week < 49)
	{
	    process_week_games(week);
	    update_ranks();
	    update_fixtures();
	    update_teams();
	    
	    week++;
	    
	    update_transferlist();
	}
    
    transfer_offers();
    update_counters();
    team_offers();
    finance_events();
    update_scout();
    update_autosave();    
    update_variables();
    
    /* player wasn't fired */
    if( status < 900000 && 
	(my_team_played(week - 1) ||
	 my_team_played(week)) )
    {
	if(my_team_played(week - 1))
	    show_results(3);
	else if(week < 49)
	    callback_show_preview();
    }
    
    if(week == 50)
    {
	season_end();
	season++;
	week = 1;
    }

    if(stadiums[my_team].safety < (gfloat)options[OPT_DUMMY1] / 100)
	show_popup_window("Your stadium safety's low. ", NULL);
}
