/*********************************************************
 * Functions called directly from the GUI                *
 *********************************************************/

#include "callbacks.h"
#include "support.h"


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
	free_history();
	start(1);
    }
    else
	show_popup_window("Are you sure you want to start a new game and discard your current game?", popup_status);    
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
	print_message("Your team structure must have three non-zero digits which sum up to 10.");

    entry_set_text_from_int(entry, teams[my_team].structure);    

    /* debug */
    if(new_structure > -9 && new_structure < 0)
	print_average_skill(new_structure * -1);
    else if(new_structure == -9)
	g_print("\n *** Cntsuc: %d ***\n\n", 
		counters[COUNT_SUCCESS]);
    else if(new_structure == -10)
	options[OPT_ATT_TALENT] = (options[OPT_ATT_TALENT] + 1) % 2;
    else if(new_structure == -11)
    {
	finances[FIN_DEBTS] = 0;
	counters[COUNT_LOAN] = -2;
    }
    else if(new_structure == -12)
	for(i=0;i<20;i++)
	    teams[my_team].players[i].age =
		teams[my_team].players[i].peak_age;
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
	stadiums[my_team].capacity = (new_structure * -1) % 100 * 1000;
    else if(new_structure >= 50 && new_structure <= 99)
	options[OPT_DUMMY1] = new_structure;
    else if(new_structure == 100)
	options[OPT_DUMMY1] *= -1;
    else if(new_structure == 999)
    {
	debug = (debug + 1) % 2;
	g_print("\n *** Debug: %d ***\n\n", debug);
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
	    "Next week you'll fire your scout and hire a new one.");
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

    set_buttons();
}


gboolean
on_player_info_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    GtkTreeSelection *selection = 
	gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));

    if(event->type == GDK_BUTTON_PRESS)
    {
	if(status == 0)
	    callback_show_opponent_team(selection, event);
	else if(get_place(status, 12) == 10 ||
	   get_place(status, 12) == 11)
	{
	    if(callback_transfer_select(selection, event) == 1)
		status = 110000 + get_place(status, 22);
	}
	else if(status == 120000)
	    callback_transfer_team_select(selection, event);   
	else if(get_place(status, 12) == 13)
	    callback_transfer_buy_player(selection, event);
    }

    set_buttons();

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

    initialize_main_window();
    status = 0;
    
    set_buttons();

    if(notify_status)
    {	
	notify_status = FALSE;
	show_popup_window("You might want to have a look at the transfer list. ",
			  popup_status);
    }
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
	    show_popup_window("There are injured or banned players in your team. Proceed?",
			      popup_status);
	}
    }
    else
	show_popup_window("Are you sure you want to end this week's activities and begin a new week?",
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
	print_message("You can't have a bigger stadium; it's not safe.");
    else if(counters[COUNT_INC_CAP] < 1)
	show_stadium_window();
    else
	print_message("Your stadium is being enlarged. You can't increase its capacity until work's finished.");

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
    {
	free_history();
	
	g_string_free(save_file, TRUE);

	gtk_main_quit();
    }
    else
	show_popup_window("Do you really want to quit without saving?", popup_status);
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
	free_history();	
	g_string_free(save_file, TRUE);
	gtk_widget_destroy(main_window);
	start_editor();
	return;
    }

    show_popup_window("Your current game is not saved and will be lost. Continue?",
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
