/*********************************************************
 * Functions called directly from the GUI                *
 *********************************************************/

#include "callbacks.h"
#include "callback_func.h"
#include "fixture.h"
#include "free.h"
#include "gui.h"
#include "game_gui.h"
#include "load_save.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "team.h"
#include "treeview.h"
#include "startup.h"
#include "support.h"
#include "window.h"

/************************************************************
 * callbacks for the main window                            *
 ************************************************************/

void
on_menu_new_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gint popup_status[3] = {40, -1, -1};

    if(options[OPT_CONF_QUIT] == 0 ||
       save_status ||
       GPOINTER_TO_INT(user_data) == 1)
    {
	free_memory();
	start(1);
    }
    else
	show_popup_window(
	    _("Are you sure you want to start a new game and discard your current game?"),
	    popup_status);    
}


void
on_menu_open_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_load_clicked(NULL, NULL);
}


void
on_menu_save_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_save_clicked(NULL, NULL);
}


void
on_menu_save_as_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    show_file_selection(1);
}


void
on_menu_options_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_options_clicked(NULL, NULL);
}


void
on_menu_quit_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_quit_clicked(NULL, NULL);
}

void
on_menu_about_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_help_clicked(NULL, NULL);
}


void
on_menu_team_editor_help_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    show_help(1);
}

/* change playing style of the team */
void
on_style_optionmenu_changed            (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint old_style = teams[my_team].style;

    set_save(FALSE);
    teams[my_team].style =
	gtk_option_menu_get_history(optionmenu) - 2;

    if(old_style != teams[my_team].style)
	bookmaker_re_tip();
}

void
on_structure_entry_activate            (GtkEntry        *entry,
                                        gpointer         user_data)
{
    gint i;
    gint new_structure = entry_get_int(entry);
    gchar buf[SMALL];

    if(new_structure == teams[my_team].structure)
	return;

    if(get_place(new_structure, 1) + 
       get_place(new_structure, 2) + 
       get_place(new_structure, 3) == 10)
    {
	set_save(FALSE);
	change_structure(new_structure);	
	show_players(NULL, NULL, 0, NULL, 0);
	callback_rearrange(FALSE);
    }
    else
	print_message(_("Your team structure must have three non-zero digits which sum up to 10."));

    entry_set_text_from_int(entry, teams[my_team].structure);    

    /* debug */
    if(new_structure > -9 && new_structure < 0)
	print_average_skill(new_structure * -1);
    else if(new_structure == -9)
    {
	sprintf(buf, "Cntsuc: %d", counters[COUNT_SUCCESS]);
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure == -10)
    {
	options[OPT_ATT_TALENT] = (options[OPT_ATT_TALENT] + 1) % 2;	
	sprintf(buf, "Show talent %d", options[OPT_ATT_TALENT]);
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure == -11)
    {
	finances[FIN_DEBTS] = 0;
	counters[COUNT_LOAN] = -2;
	sprintf(buf, "Debts reset; loan counter reset");
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure == -12)
    {
	for(i=0;i<20;i++)
	    teams[my_team].players[i].age =
		teams[my_team].players[i].peak_age;

	sprintf(buf, "Player ages set to peak age");
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure < -114 && new_structure > -175)
	my_team = new_structure * -1;
    else if(new_structure < -70 && new_structure > -99)
	my_team = league_rank_id(get_league_from_id(my_team), 
				 rank[my_team] + new_structure + 85);
    else if(new_structure == -200)
    {
	for(i=0;i<20;i++)
	    g_print("i %d time %d player %s %s\n",
		    i, transferlist[i].time,
		    teams[transferlist[i].team_id].players[transferlist[i].player_number].name,
		    teams[transferlist[i].team_id].name);
    }
    else if(new_structure < -250 && new_structure > -300)
    {
	stadiums[my_team].capacity = (new_structure * -1) % 100 * 1000;
	sprintf(buf, "New stadium capacity: %d", stadiums[my_team].capacity);
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure >= 50 && new_structure <= 99)
    {
	options[OPT_DUMMY1] = new_structure;
	sprintf(buf, "Safety warning set to %d%%",
		options[OPT_DUMMY1]);
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure == 100)
    {
	options[OPT_DUMMY1] *= -1;
	sprintf(buf, "Safety warning %d",
		(options[OPT_DUMMY1] > 0));
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
    else if(new_structure == 999)
    {
	debug = (debug + 1) % 2;
	sprintf(buf, "Debug: %d", debug);
	g_print("\n *** %s ***\n\n", buf);
	print_message(buf);
    }
}

void
on_scout_optionmenu_changed            (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint new_scout = gtk_option_menu_get_history(optionmenu) + 1;

    if(scout % 10 != new_scout)
    {
	set_save(FALSE);
	scout = scout % 10 + (new_scout * 10);
	print_message(
	    _("Next week you'll fire your scout and hire a new one."));
    }
    else
	scout = scout % 10;
}


void
on_optionmenu_physio_changed        (GtkOptionMenu   *optionmenu,
				     gpointer         user_data)
{
    if(gtk_option_menu_get_history(optionmenu) == physio - 1)
	return;

    physio = gtk_option_menu_get_history(optionmenu) + 1;
    set_save(FALSE);
}

/* change the team structure */
void
on_structure_button_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *entry_structure =
	lookup_widget(main_window, "entry_structure");
    gint structure = 
	entry_get_int(GTK_ENTRY(entry_structure));
    
    /* if the human player has entered something
       new into the entry field, try to change the
       structure correspondingly */
    if(structure != teams[my_team].structure)
	on_structure_entry_activate(GTK_ENTRY(entry_structure), NULL);
    else
    {
	set_save(FALSE);
	cycle_through_structures();
	entry_set_text_from_int(GTK_ENTRY(entry_structure),
				teams[my_team].structure);
	callback_rearrange(FALSE);
	show_players(NULL, NULL, 0, NULL, 0);
    }
}


/* catch a click on the player list in the left panel */
gboolean
on_player_list_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    GtkTreeSelection *selection =
	gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));

    if(event->type == GDK_BUTTON_PRESS &&
       event->button == 3)
	callback_show_player_info(selection, event);
    else if(event->type == GDK_BUTTON_PRESS &&
	    event->button == 1)
	callback_select_player(selection, event);
    else if(event->type == GDK_BUTTON_PRESS &&
	    event->button == 2)
	callback_fire_player(selection, event);

    return FALSE;
}


void
on_button_rearrange_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
    set_save(FALSE);
    callback_rearrange(options[OPT_REARRANGE]);
}

void
on_button_transfers_clicked           (GtkButton       *button,
				       gpointer         user_data)
{
    show_transfers();
    status = 100000;
    notify_status[NOTIFY_TRANSFERS] = FALSE;

    gtk_notebook_set_current_page(
	GTK_NOTEBOOK(lookup_widget(main_window, "notebook_player")), 1);

    set_buttons();
}


gboolean
on_player_info_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    GtkTreeSelection *selection = 
	gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
    gint row_idx = 
	get_row_index(selection, event, (get_place(status, 12) == 22));
    gint id;
    
    if(event->type == GDK_BUTTON_PRESS)
    {
	if(status == 0 && row_idx >= 0)
	{
	    status = 130000 + row_idx;
	    show_team_browse(row_idx, NULL);
	}
	else if(get_place(status, 12) == 10 ||
	   get_place(status, 12) == 11)
	{
	    if(callback_transfer_select(row_idx - 1))
		status = 110000 + get_place(status, 22);
	}
	else if(status == 120000)
	    callback_transfer_team_select(row_idx - 1);
	else if(get_place(status, 12) == 13)
	    callback_transfer_buy_player(row_idx - 1);
	else if(get_place(status, 12) == 22 && row_idx != my_team &&
		row_idx >= 0 && row_idx < 130)
	{
	    id = teams[row_idx].id;
	    status = 130000 + id;
	    show_team_browse(id, NULL);
	}
	else if(get_place(status, 12) == 21 &&
		row_idx >= 0 && row_idx < FIX_END)
	{
	    id = fixtures[row_idx].team_id[(event->button == 3)];
	    if(id == my_team)
		id = fixtures[row_idx].team_id[(event->button != 3)];
	    status = 130000 + id;
	    show_team_browse(id, NULL);
	}
    }

    set_buttons();

    return FALSE;
}


gboolean
on_player_list2_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    on_player_list_button_press_event(
	lookup_widget(main_window, "player_list"),
	event, NULL);

  return FALSE;
}


void
on_button_transfer_ok_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    if(get_place(status, 12) == 11)
	callback_make_transfer_offer(GTK_WIDGET(button));
    else if(get_place(status, 12) == 31)
    {
	if(callback_get_the_loan() == 1)
	{
	    on_button_fin_stad_clicked(NULL, NULL);
	    set_save(FALSE);
	}
    }
    else if(get_place(status, 12) == 32)
    {
	if(callback_pay_the_loan() == 1)
	{
	    on_button_fin_stad_clicked(NULL, NULL);
	    set_save(FALSE);
	}
    }
    
    set_buttons();
}


void
on_spin_fee_activate                   (GtkEntry        *entry,
                                        gpointer         user_data)
{
    gint value = (gint)strtol(gtk_entry_get_text(entry), NULL, 10);    
    GtkWidget *spin_fee = lookup_widget(main_window, "spin_fee");

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_fee), 
			      (gdouble)value);

    on_button_transfer_ok_clicked(NULL, NULL);    
}


void
on_button_browse_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *player_info = 
	lookup_widget(main_window, "player_info");

    show_team_list(player_info, 1);
    status = 120000;
    
    set_buttons();
}


void
on_button_browse_back_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    if(!GTK_WIDGET_MAPPED(button))
	on_button_quit_clicked(button, NULL);
    else if(get_place(status, 12) == 13)
	callback_transfers_browse_teams(-1);
    else if(get_place(status, 12) == 21)
    {	
	status = status - get_place(status, 22) +
		   get_my_fixture_week(get_place(status, 22), -1);
	show_fixtures(get_place(status, 22));
    }
    else if(get_place(status, 12) == 22)
    {
	show_tables_league(
	    (get_place(status, 1) + 4) % 6 + 1);
	status = 220000 + (get_place(status, 1) + 4) % 6 + 1;
    }
    else if(get_place(status, 12) == 23)
    {
	callback_show_best_players(
	    (get_place(status, 1) + 3) % 5 + 1);
	status = 230000 + (get_place(status, 1) + 3) % 5 + 1;
    }
    else if(get_place(status, 12) == 24)
	callback_show_history(status - 240001);
    else if(get_place(status, 12) == 60)
	show_results(status % 2);

    set_buttons();
}


void
on_button_browse_forward_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    if(get_place(status, 12) == 13)
	callback_transfers_browse_teams(1);
    else if(get_place(status, 12) == 21)
    {
	status = status - get_place(status, 22) + 
		   get_my_fixture_week(get_place(status, 22), 1);
	show_fixtures(get_place(status, 22));
    }
    else if(get_place(status, 12) == 22)
    {
	show_tables_league(
	    get_place(status, 1) % 6 + 1);
	status = 220000 + get_place(status, 1) % 6 + 1;
    }
    else if(get_place(status, 12) == 23)
    {
	callback_show_best_players(
	    get_place(status, 1) % 5 + 1);
	status = 230000 + get_place(status, 1) % 5 + 1;
    }
    else if(get_place(status, 12) == 24)
	callback_show_history(status - 240000 + 1);
    else if(get_place(status, 12) == 60)
	show_results(status % 2);

    set_buttons();
}

void
on_button_back_to_main_clicked         (GtkButton       *button,
                                        gpointer         user_data)
{
    gint popup_status[3] = {0, -1, -1};

    if(notify_status[NOTIFY_TRANSFERS])
    {	
	notify_status[NOTIFY_TRANSFERS] = FALSE;
	show_popup_window(_("You might want to have a look at the transfer list. "),
			  popup_status);
    }
    
    if(notify_status[NOTIFY_INJURY] == 1)
    {
	notify_status[NOTIFY_INJURY] = FALSE;
	callback_notify_injury();
    }

    initialize_main_window();
    status = 0;
    
    set_buttons();
}


void
on_optionmenu_figures_changed          (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint item = 
	gtk_option_menu_get_history(optionmenu);
    gint show_week = week - (week != 1);

    if(item == 1)
    {
	status = 210000 + show_week;
	show_fixtures(show_week);
    }
    else if(item == 2)
    {
	show_tables_league(get_league_from_id(my_team));
	status = 220000 + get_league_from_id(my_team);
    }
    else if(item == 3)
    {
	callback_show_best_players(get_league_from_id(my_team));
	status = 230000 + get_league_from_id(my_team);
    }
    else if(item == 4)
    {
	callback_show_history(season - 1);
	status = 240000 + season - 1;
    }
    else if(item == 5)
    {
	callback_show_graph();
	status = -100000 - my_team * 100;
    }
    
    set_buttons();
}


void
on_button_league_results_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    show_league_results();

    set_buttons();
}


void
on_button_new_week_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    gint popup_status[3] =
	{100 + OPT_CONF_NEW_WEEK, -1, -1};

    if(options[OPT_CONF_NEW_WEEK] == 0 ||
       GPOINTER_TO_INT(user_data) >= 1)
    {
	if(options[OPT_CONF_UNFIT] == 0 ||
	   GPOINTER_TO_INT(user_data) == 2 ||
	   unfit_players() == 0 ||
	   !my_team_played(week))
	{
	    print_message("");
	    status = 600000 + options[OPT_SHOW_MY_GAMES];
	    callback_new_week(TRUE);
	    set_save(FALSE);
	}
	else
	{
	    popup_status[0] = 100 + OPT_CONF_UNFIT;
	    show_popup_window(_("There are injured or banned players in your team. Proceed?"),
			      popup_status);
	}
    }
    else
	show_popup_window(_("Are you sure you want to end this week's activities and begin a new week?"),
			  popup_status);

    set_buttons();
}


void
on_button_fin_stad_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    show_fin_stad();
    status = 300000;

    set_buttons();
}


void
on_button_stad_capacity_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
    if(stadiums[my_team].capacity >= 100000)
	print_message(_("You can't have a bigger stadium; it's not safe."));
    else if(counters[COUNT_INC_CAP] < 1)
	show_stadium_window();
    else
	print_message(_("Your stadium is being enlarged. You can't increase its capacity until work's finished."));

    set_buttons();
}


void
on_button_stad_safety_clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_stadium_improve();
    show_fin_stad();
    
    set_buttons();
}


void
on_button_get_loan_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    if(callback_get_loan() == 1)
	status = 310000;

    set_buttons();
}


void
on_button_pay_loan_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    if(callback_pay_loan() == 1)
	status = 320000;
    
    set_buttons();
}

void
on_button_preview_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_show_preview();

    status = 400000;

    set_buttons();
}

void
on_button_options_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
    show_opt_window();
}

void
on_button_load_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    show_file_selection(0);
    
    set_buttons();
}


void
on_button_save_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    if(save_file->str[save_file->len - 1] != '/' &&
       options[OPT_OVERWRITE] == 1)
    {
	save_game(save_file->str);
	set_save(TRUE);
    }
    else
	show_file_selection(1);

    set_buttons();
}

void
on_button_quit_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    gint popup_status[3] = {50,
			    -1, -1};

    if(GPOINTER_TO_INT(user_data) == 1 ||
       options[OPT_CONF_QUIT] == 0 || save_status == 1)
	gtk_main_quit();
    else
	show_popup_window(_("Do you really want to quit without saving?"), popup_status);
}

void
on_button_help_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    show_help(0);
}

void
on_button_fixtures_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *optionmenu =
	lookup_widget(main_window, "optionmenu_figures");

    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu),
				1);
}


void
on_button_tables_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
    GtkWidget *optionmenu =
	lookup_widget(main_window, "optionmenu_figures");

    gtk_option_menu_set_history(GTK_OPTION_MENU(optionmenu),
				2);
}


void
on_optionmenu_quick_opt_changed        (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint i;
    gint item = 
	gtk_option_menu_get_history(optionmenu);
    gint *quick_opt_options[4] =
	{&options[OPT_NOTIFY],
	 &options[OPT_JOBS],
	 &options[OPT_SHOW_LIVE],
	 &options[OPT_OVERWRITE]};

    for(i=1;i<5;i++)
	if(item == i)
	    *quick_opt_options[i - 1] =
		(*quick_opt_options[i - 1] + 1) % 2;

    gtk_option_menu_set_history(optionmenu, 0);

    set_save(FALSE);
}

void
on_button_undo_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
    callback_undo();
}

void
on_start_editor_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gint popup_status[3] = {92, 0, 0};

    if(save_status || GPOINTER_TO_INT(user_data) == 1)
    {	
	free_memory();
	gtk_widget_destroy(main_window);
	start_editor();
	return;
    }

    show_popup_window(_("Your current game is not saved and will be lost. Continue?"),
		      popup_status);
}


void
on_optionmenu_fixtures_changed         (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint i;
    gint item = 
	gtk_option_menu_get_history(optionmenu);
    gint show_week = week - (week != 1);
   
    if(item == 0)
	return;

    for(i=2;i<7;i++)
	if(item == i)
	    status = 210000 + show_week + (i - 1) * 1000;
    
    for(i=11;i<14;i++)
	if(item == i)
	    status = 210000 + show_week + (i - 5) * 1000;

    if(item == 8)
	status = 210000 + show_week + 9000;
    else if(item == 9)
	status = 210000 + show_week + 1100;
	
    clear_treeview(NULL);
    show_fixtures(show_week);
}

void
on_put_on_transfer_list_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_transfers_clicked(NULL, NULL);
    callback_show_player_info(NULL, NULL);
}


void
on_remove_from_transfer_list_activate (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    callback_rm_pl_transfer();
}


void
on_fire_activate                      (GtkMenuItem     *menuitem,
				       gpointer         user_data)
{
    callback_fire_player(NULL, NULL);
}

void
on_show_info_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    on_button_back_to_main_clicked(NULL, NULL);
    callback_show_player_info(NULL, NULL);
}

void
on_show_statistics_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    callback_show_player_stats(NULL, NULL);
}

void
on_shoots_penalties_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    callback_penalty_shooter();
}


void
on_start_update_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data)
{
    gint popup_status[3] = {93, 0, 0};

    if(save_status || GPOINTER_TO_INT(user_data) == 1)
    {	
	free_memory();
	start_update();
	return;
    }

    show_popup_window(_("Your current game is not saved and will be lost. Continue?"),
		      popup_status);
}

void
on_optionmenu_finstad_changed          (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data)
{
    gint value = gtk_option_menu_get_history(optionmenu);

    if(value == 1)
    {
	if(stadiums[my_team].capacity >= 100000)
	    print_message(_("You can't have a bigger stadium; it's not safe."));
	else if(counters[COUNT_INC_CAP] < 1)
	    show_stadium_window();
	else
	    print_message(_("Your stadium is being enlarged. You can't increase its capacity until work's finished."));
    }
    else if(value == 2)
    {
	callback_stadium_improve();
	show_fin_stad();
    }
    else if(value == 3)
    {
	if(callback_get_loan() == 1)
	    status = 310000;
    }
    else if(value == 4)
    {
	if(callback_pay_loan() == 1)
	    status = 320000;
    }

    gtk_option_menu_set_history(optionmenu, 0);

    set_buttons();
}

void
on_checkbutton_boost_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
    options[OPT_BOOST] = 
	gtk_toggle_button_get_active(togglebutton);

    bookmaker_re_tip();
}

