/******************************************************************
 * Support functions for the callback functions in 'callbacks.c'  *
 ******************************************************************/

#include "callback_func.h"
#include "callbacks.h"
#include "game.h"
#include "game_gui.h"
#include "graph.h"
#include "gui.h"
#include "history.h"
#include "finance.h"
#include "fixture.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "team.h"
#include "files.h"
#include "transfer.h"
#include "treeview.h"
#include "support.h"
#include "window.h"

/* get the integer value in the first column of a treeview */
gint
get_row_index(GtkTreeSelection *selection,
	      GdkEventButton *event, gint column)
{
    GtkTreeView *treeview = 
	gtk_tree_selection_get_tree_view(selection);
    GtkTreeModel *model = 
	gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    GtkTreePath *path;
    gint idx;
    
    if(!gtk_tree_view_get_path_at_pos(treeview,
				      event->x, event->y,
				      &path, NULL, NULL, NULL))
	return -1;

    gtk_tree_selection_select_path(selection, path);
    
    if(gtk_tree_path_get_indices(path) == NULL)
    {
	gtk_tree_path_free(path);
	return -1;
    }

    gtk_tree_model_get_iter(model, &iter, path);
    gtk_tree_model_get(model, &iter, column, &idx, -1);

    gtk_tree_path_free(path);

    return idx;
}


/* find out which player's been clicked on and
   show the detailed info */
void
callback_show_player_info(GtkTreeSelection *selection,
			  GdkEventButton *event)
{
    gint player_number = (selection != NULL) ?
	get_row_index(selection, event, 0) - 1 : selected_rows[0];

    if(player_number < 0)
    {
	print_message(_("You haven't selected a player"));	
	return;
    }
    
    /* set the selected_row label so that the
       next left-click doesn't swap two players */
    selected_rows[0] = -1;

    if(status == 100000 ||
       status == 110000)
    {
	if(week >= 35)
	    print_message(_("The transfer deadline has passed (week 35)."));
	else if(is_on_transferlist(my_team, player_number) == 1)
	    print_message(_("The player is already on the transferlist."));
	else
	{
	    if(players_on_transferlist() < 20)
	    {
		set_save(0);
		add_transfer(my_team, player_number, 5);
		show_transfers();
	    }
	    else
		print_message(_("The transferlist is full."));
	}
    }
    else
    {
	on_button_back_to_main_clicked(NULL, NULL);
	status = 50000;
	show_player_info(NULL, my_team, player_number);
    }
}

/* show player stats */
void
callback_show_player_stats(GtkTreeSelection *selection,
			   GdkEventButton *event)
{
    gint player_number = (selection != NULL) ?
	get_row_index(selection, event, 0) - 1 : selected_rows[0];

    if(player_number < 0)
    {
	print_message(_("You haven't selected a player"));	
	return;
    }
    
    /* set the selected_row label so that the
       next left-click doesn't swap two players */
    selected_rows[0] = -1;

    status = (100000 + 100 * my_team + player_number) * -1;

    show_graph(show_graph_window(), NULL,
	       &teams[my_team].players[player_number], PLAYER_HISTORY_SKILL);
}


/* handle a left-click on the player-list.
   swap two players, if necessary */
void
callback_select_player(GtkTreeSelection *selection,
			    GdkEventButton *event)
{
    gfloat old_average_cskill = 
	average_skill(my_team, 11, TRUE);
    gint new_number = 
	get_row_index(selection, event, 0) - 1;
    gint old_number = selected_rows[0];
    GtkTreeView *treeview = 
	gtk_tree_selection_get_tree_view(selection);

    if(new_number < 0)
    {
	selected_rows[0] = -1;
	return;
    }

    if(old_number >= 0 && old_number <= players_in_team() &&
       new_number >= 0 && 
       new_number <= players_in_team())
    {
	if(old_number != new_number)
	{
	    set_save(0);
	    swap_players(my_team, old_number,
			 my_team, new_number);
	    undo_info[0] = teams[my_team].players[old_number].age;
	    undo_info[1] = teams[my_team].players[new_number].age;
	    if( ((old_number < 11 && new_number > 10) ||
		 (old_number > 10 &&  new_number < 11)) &&
		options[OPT_SWAP] == 1)
		callback_rearrange(TRUE);
	    
	    if(fabs(average_skill(my_team, 11, TRUE) -
		    old_average_cskill) > 0.05)
		bookmaker_re_tip();

	    set_header();
	}

	show_players(GTK_WIDGET(treeview),
		     NULL, 0, NULL, 0);
	selected_rows[0] = -1;
    }
    else if(old_number == -1)
	selected_rows[0] = new_number;
}

/* undo the swapping of two players */
void
callback_undo(void)
{
    gint i;
    gint player_number1 = -1, player_number2 = -1;

    for(i=0;i<=players_in_team();i++)
	if(teams[my_team].players[i].age == undo_info[0])
	    player_number1 = i;
	else if(teams[my_team].players[i].age == undo_info[1])
	    player_number2 = i;
    
    if(player_number1 != -1 &&
       player_number2 != -1)
	swap_players(my_team, player_number1, my_team, player_number2);
    else
	return;

    if( ((player_number1 < 11 && player_number2 > 10) ||
	 (player_number1 > 10 &&  player_number2 < 11)) &&
	options[OPT_SWAP] == 1)
	callback_rearrange(TRUE);
       
    show_players(NULL, NULL, 0, NULL, 0);
    selected_rows[0] = -1;
}

/* show the scout's recommendation for a transfer */
void
show_recommend(gint idx)
{
    GtkWidget *spin_fee =
	lookup_widget(main_window, "spin_fee");
    GtkWidget *spin_wage =
	lookup_widget(main_window, "spin_wage");

    gtk_widget_show(spin_fee->parent->parent->parent->parent);

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_fee),
			      (gfloat)transferlist[idx].estimates[0]);
    
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_wage),
			      (gfloat)transferlist[idx].estimates[1]);
}

/* handle a click on the transfer list */
gboolean
callback_transfer_select(gint row_idx)
{
    if(row_idx < 0)
	return FALSE;

    status = status - get_place(status, 22) + row_idx;

    if(transferlist[row_idx].team_id == my_team)
    {
	remove_transfer(row_idx, TRUE);
	on_button_transfers_clicked(NULL, NULL);
	return FALSE;
    }
    else
	show_recommend(row_idx);

    return TRUE;
}

/* handle the click on the 'ok' button the human player
   makes a transfer offer with */
void
callback_make_transfer_offer(GtkWidget *widget)
{
    GtkWidget *spin_fee =
	lookup_widget(main_window, "spin_fee");
    GtkWidget *spin_wage =
	lookup_widget(main_window, "spin_wage");
    gint fee =
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_fee));
    gint wage =
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_wage));
    gint idx = get_place(status, 22);

    if(fee > BUDGET)
    {
	print_message(_("You haven't got the money."));
	return;
    }
    else if(players_in_team() == 20)
    {
	print_message(_("There's no room in your team. You can't have more than 20 players."));
	return;
    }
    
    if(fee < teams[transferlist[idx].team_id].
       players[transferlist[idx].player_number].value ||
       rnd(0,1) < 0.9)
    {
	print_message(_("The team rejects your offer and removes the player from the transferlist."));
	remove_transfer(idx, TRUE);
	on_button_transfers_clicked(NULL, NULL);
	return;
    }
    else if(wage < teams[transferlist[idx].team_id].
	    players[transferlist[idx].player_number].wage ||
	    rnd(0,1) < 0.9)
    {
	print_message(_("The player doesn't like your offer and decides to stay in his team."));
	remove_transfer(idx, TRUE);
	on_button_transfers_clicked(NULL, NULL);
	return;	
    }       

    teams[transferlist[idx].team_id].
	players[transferlist[idx].player_number].wage =
	wage;

    execute_transfer(idx);
    finances[FIN_MONEY] -= fee;
    finances[FIN_TRANSFERS] -= fee;
    print_message(_("The team accepts your offer."));
    show_players(NULL, NULL, 0, NULL, 0);
    on_button_transfers_clicked(NULL, NULL);
}

/* handle a click on the 'browse teams' button */
void
callback_transfer_team_select(gint row_idx)
{
    if(row_idx < 0)
	return;

    show_team_browse(row_idx, NULL);

    status = 130000 + row_idx;
}

/* show the next or previous team in browse mode */
void
callback_transfers_browse_teams(gint direction)
{
    gint current_team_id = status % 1000;
    gint new_team_id = current_team_id + direction;
    
    if(new_team_id == my_team ||
       new_team_id == 114 ||
       new_team_id == 130)
	new_team_id += direction;

    if(new_team_id < 0)
	new_team_id = 174;
    else if(new_team_id > 174)
	new_team_id = (my_team == 0) ? 1 : 0;

    show_team_browse(new_team_id, NULL);
    status = 130000 + new_team_id;
}

/* handle a left-click in the player_info when 
   the human player browses the teams to buy players */
void
callback_transfer_buy_player(gint row_idx)
{    
    gint team_id = status % 1000;
    gint player_number = row_idx;
    gint value, wage;
    gint popup_status[3];
    gchar buf[SMALL];
    
    if(player_number == -1)
	return;

    if(players_in_team() == 20)
    {
	print_message(_("There's no room in your team. You can't have more than 20 players."));
	return;
    }

    if(week >= 35)
    {
	print_message(_("The transfer deadline has passed (week 35)."));
	return;
    }

    popup_status[0] = 130000 + team_id;
    popup_status[1] = player_number;
    popup_status[2] = 0;

    value = (finances[FIN_MONEY] < 10000000) ?
	round_integer((gint)(teams[team_id].
			     players[player_number].value * expf(1)), 2) :
	round_integer(
	    (gint)((teams[team_id].players[player_number].skill *
		    (gfloat)finances[FIN_MONEY]) /
		   (expf(1) * 8)), 2);

    wage = 
	round_integer((gint)(teams[team_id].
			     players[player_number].wage * 
			     expf(1.25)), 2);

    if(value > BUDGET)
    {
	print_message(_("After having a look at your bank account the team politely rejects your offer."));
	return;
    }

    /* here we basically throw an unfair coin whether or not the
       player is for sale */
    if(get_place((gint)(teams[team_id].players[player_number].
			skill * 10000), 1) % 3 < 2)
    {
	sprintf(buf, _("The team has unfortunately no suitable replacement for %s and has to reject your offer."),
		  teams[team_id].players[player_number].name);
	print_message(buf);
	return;
    }

    sprintf(buf, _("The team has an adequate replacement for %s and is willing to cancel his contract for "), 
	      teams[team_id].players[player_number].name);

    print_grouped_int(value, buf, 1);
    strcat(buf, _(". The player demands a wage of "));
    print_grouped_int(wage, buf, 1);
    strcat(buf, _(". Accept?"));
    
    show_popup_window(buf, popup_status);
}

/* move the player to the human player's team and deduce
   transfer fee */
void
callback_popup_buy_player(gint team_id, gint player_number)
{
    gint value = (finances[FIN_MONEY] < 10000000) ?
	round_integer((gint)(teams[team_id].
			     players[player_number].value * expf(1)), 2) :
	round_integer((gint)(finances[FIN_MONEY] * 0.3873), 2);
    
    /* adjust player's wage */
    teams[team_id].players[player_number].wage =
	round_integer((gint)(teams[team_id].
			     players[player_number].wage * 
			     expf(1.25)), 2);

    move_player(team_id, player_number, my_team);
    
    finances[FIN_MONEY] -= value;
    finances[FIN_TRANSFERS] -= value;

    show_players(NULL, NULL, 0, NULL, 0);
    show_team_browse(team_id, NULL);
}

/* try to set each of the first 11 players on his
   favoured position and sort the substitutes by position */
void
callback_rearrange(gboolean change)
{
    GtkWidget *entry_structure = 
	lookup_widget(main_window, "entry_structure");

    if(change)
    {
	change_structure(find_appropriate_structure());
	entry_set_text_from_int(GTK_ENTRY(entry_structure), teams[my_team].structure);
    }

    rearrange_team();

    selected_rows[0] = -1;

    show_players(NULL, NULL, 0, NULL, 0);    
}

void
callback_sell_player(gint trans_idx, gint fee)
{
    gint i;
    gint player_number = 
	transferlist[trans_idx].player_number;

    for(i=19;i>player_number;i--)
	if(teams[my_team].players[i].pos >= 0)
	{
	    swap_players(my_team, i, my_team, player_number); 
	    break;
	}

    teams[my_team].players[i].pos = -1;

    remove_transfer(trans_idx, TRUE);

    finances[FIN_MONEY] += fee;
    finances[FIN_TRANSFERS] += fee;
}

/* improve stadium capacity or safety */
void
callback_stadium_improve(void)
{
    gint cost =
	stadium_improve_costs(1, -1);

    if(BUDGET + cost < 0)
    {
	print_message(_("You haven't got the money."));
	return;
    }
    /* the player can't increase a value more often than once
       a week */
    else if(counters[COUNT_INC_SAF] == 2)
    {
	print_message(_("You may increase only twice a week."));
	return;
    }
    
    if(stadiums[my_team].safety >= 0.996)
    {
	print_message(_("You can't improve your stadium safety, it's already 100% safe."));
	return;
    }
    
    stadiums[my_team].safety += 0.05;
    counters[COUNT_INC_SAF]++;
    if(stadiums[my_team].safety > 1)
	stadiums[my_team].safety = 1;

    finances[FIN_MONEY] += cost;
    finances[FIN_STADIUM] += cost;

    set_save(0);
}

gint
callback_get_loan(void)
{
    gint maximum_loan = 
	calculate_max_loan_credit(0) - finances[FIN_DEBTS];
    GtkWidget *spin_fee =
	lookup_widget(main_window, "spin_fee");

    if(maximum_loan <= 0)
    {
	print_message(_("Your bank grants you no more money."));
	return 0;
    }

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_fee),
			      (gdouble)maximum_loan);    

    return 1;
}

gint
callback_pay_loan(void)
{
    gint max_pay = 
	(BUDGET > finances[FIN_DEBTS]) ?
	finances[FIN_DEBTS] : BUDGET;
    GtkWidget *spin_fee =
	lookup_widget(main_window, "spin_fee");

    if(finances[FIN_DEBTS] == 0)
    {
	print_message(_("You are free from debt."));
	return 0;
    }
    else if(BUDGET <= 0)
    {
	print_message(_("You're below your drawing credit limit. You can't pay back money."));
	return 0;
    }

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_fee),
			      (gdouble)max_pay);    

    return 1;
}

gint
callback_get_the_loan(void)
{
    gint maximum_loan = 
	calculate_max_loan_credit(0) - finances[FIN_DEBTS];
    GtkWidget *spin_fee =
	lookup_widget(main_window, "spin_fee");
    gint loan = 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_fee));

    if(loan > maximum_loan)
    {
	print_message(_("You can't borrow this much."));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_fee),
				  (gdouble)maximum_loan);
	return 0;
    }

    finances[FIN_MONEY] += loan;
    finances[FIN_DEBTS] += loan;
    
    counters[COUNT_LOAN] = (counters[COUNT_LOAN] >= -1) ?
	counters[COUNT_LOAN] : 15;

    return 1;
}

gint
callback_pay_the_loan(void)
{
    gint i;
    gint maximum_loan = calculate_max_loan_credit(0);
    gint max_pay = 
	(BUDGET > finances[FIN_DEBTS]) ?
	finances[FIN_DEBTS] : finances[FIN_MONEY] + calculate_max_loan_credit(1);
    GtkWidget *spin_fee =
	lookup_widget(main_window, "spin_fee");
    gint pay = 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_fee));

    if(pay > BUDGET)
    {
	print_message(_("You haven't got the money to pay back this much"));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_fee),
				  (gdouble)max_pay);
	return 0;
    }

    pay = (pay > finances[FIN_DEBTS]) ?
	finances[FIN_DEBTS] : pay;

    finances[FIN_MONEY] -= pay;
    finances[FIN_DEBTS] -= pay;

    if(finances[FIN_DEBTS] == 0)
    {
	print_message(_("You are free from debt."));
	counters[COUNT_LOAN] = -2;
	return 1;
    }
    else
    {
	print_message(_("You have paid back a part of your loan."));
	for(i=6;i>1;i--)
	    if((gfloat)pay / (gfloat) maximum_loan >= 1 / (gfloat)i)
		counters[COUNT_LOAN]++;

	if(counters[COUNT_LOAN] > 15)
	    counters[COUNT_LOAN] = 15;
    }

    return 1;
}

/* sort the field players and goalies and
   show the best 10 */
void
callback_show_best_players(gint league)
{
    gint best_players[20][2];

    get_best_players(league, best_players, 10, 0, 9,
		     player_compare_goalgetters);
    get_best_players(league, best_players, 0, 10, 19,
		     player_compare_goalies);

    show_best_players(league, best_players);
}

/* show the history of the given season */
void
callback_show_history(gint season_number)
{
    gint i;
    season_stat *stat = history;

    if(history == NULL)
    {
	print_message(_("This is your first season."));
	on_button_back_to_main_clicked(NULL, NULL);
	return;
    }

    if(season_number == season)
	season_number = 1;
    else if(season_number == 0)
	season_number = season - 1;

    status = 240000 + season_number;

    for(i=0;i<season_number - 1;i++)
	if(stat->next != NULL)
	    stat = stat->next;
	else
	    g_print("callback_show_history: too few histories available. season_number : %d\n", season_number);
	
    show_history(stat);
}

void
get_fix_indices(gint *fix_indices, gint week_number)
{
    gint i, j, k;

    j = 0;
    for(i = week_number; i < week_number + 4; i++)
    {
	for(k=FIX_END - 1;k>=0;k--)
	    if(fixtures[k].type != -1 &&
	       my_team_involved(fixtures[k])&&
	       fixtures[k].week_number == i)
	    {
		fix_indices[j] = k;
		j++;
	    }
	
	while(j < (i - week_number + 1) * 2)
	    j++;
    }
}

/* show the results of last week and the fixtures
   of next week */
void
callback_show_preview(void)
{
    gint i;
    gint fix_indices[8];

    for(i=0;i<8;i++)
	fix_indices[i] = -10;

    get_fix_indices(fix_indices, week - 1);

    show_preview(fix_indices);
}

/* handle a click on the OK button of the stadium capacity increase window */
void
callback_improve_stadium(GtkWidget *widget)
{
    gint i;
    GtkWidget *radiobuttons[4];
    GtkWidget *stadium_window =
	lookup_widget(widget, "stadium_window");

    radiobuttons[0] = 
	lookup_widget(widget, "radiobutton_seat0");
    radiobuttons[1] = 
	lookup_widget(widget, "radiobutton_seat1");
    radiobuttons[2] = 
	lookup_widget(widget, "radiobutton_seat2");
    radiobuttons[3] = 
	lookup_widget(widget, "radiobutton_seat3");

    for(i=0;i<4;i++)
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobuttons[i])))
	{
	    if(stadium_improve_costs(0, i) + BUDGET < 0)
	    {
		show_popup_window(_("You haven't got the money."), NULL);
		return;
	    }
	    
	    counters[COUNT_INC_CAP] = -(i + 1) * 500;
	    
	    if(i > 0)
		counters[COUNT_INC_CAP] -= rndi(i * 2 - 1, i * 2);
	}    

    change_popups_active(-1);
    gtk_widget_destroy(stadium_window);

    set_save(0);
    show_fin_stad();
    set_buttons();
}

/* remove the selected player from the transfer list */
void
callback_rm_pl_transfer(void)
{
    gint i;
    gint player_number = 
	selected_rows[0];

    if(player_number < 0) 
    {
	print_message(_("You haven't selected a player"));
	return;
    }

    for(i=0;i<20;i++)
	if(transferlist[i].time > 0 &&
	   transferlist[i].team_id == my_team &&
	   transferlist[i].player_number == player_number)
	{
	    set_save(0);
	    remove_transfer(i, TRUE);
	    on_button_transfers_clicked(NULL, NULL);
	    return;
	}

    print_message(("The selected player's not on the list."));
}

/* fire the selected player */
void
callback_fire_player(GtkTreeSelection *selection,
		     GdkEventButton *event)
{
    gint i;
    gint player_number = (selection != NULL) ?
	get_row_index(selection, event, 0) - 1 : selected_rows[0];
    gint popup_status[3] = {61, my_team, player_number};

    if(player_number < 0) 
    {
	print_message(_("You haven't selected a player"));
	return;
    }

    selected_rows[0] = player_number;

    for(i = COUNT_OLD_WAGE1; i <= COUNT_OLD_WAGE5; i++)
	if(counters[i] == 0)
    {
	popup_status[0] = 60;
	break;	
    }

    show_popup_window("", popup_status);
}

/* show a popup telling the user that some players' careers
   had to stop because of a severe injury */
void
callback_notify_injury(void)
{
    gint i;
    gchar names[11][19];
    gchar buf[SMALL];

    for(i=0;i<11;i++)
    {
	strcpy(names[i], "");
	if(teams[my_team].players[i].health > 12000)
	{
	    strcpy(names[i], teams[my_team].players[i].name);
	    remove_player(my_team, i);
	}
    }

    sprintf(buf, _("I'm sorry to tell you that one or more of your players have suffered severe injuries. They won't be able to play football on a professional level anymore and had to retire:\n   "));
    
    for(i=1;i<11;i++)
	if(strlen(names[i]) > 0)
	{
	    strcat(buf, names[i]);
	    strcat(buf, "\n   ");
	}

    show_popup_window(buf, NULL);
}

/* make the selected player the one responsible for penalties */
void
callback_penalty_shooter(void)
{
    gint player_number = 
	selected_rows[0];
    gchar buf[SMALL];

    if(player_number < 0) 
    {
	print_message(_("You haven't selected a player"));
	return;
    }

    if(player_number == options[OPT_PENALTY_SHOOTER])
    {
	options[OPT_PENALTY_SHOOTER] = -1;
	print_message(_("Penalty shooter deselected. Player with highest cskill will shoot penalties."));
	show_players(NULL, NULL, 0, NULL, 0);    
	return;
    }

    options[OPT_PENALTY_SHOOTER] = player_number;

    if(player_number > 10)
	sprintf(buf, _("%s is a substitute. He will only shoot the penalties if you move him into the team; otherwise the field player with highest skill will shoot."), teams[my_team].players[player_number].name);
    else
	sprintf(buf, _("%s will shoot the penalties in regulation and extra time."),
		teams[my_team].players[player_number].name);

    print_message(buf);
    show_players(NULL, NULL, 0, NULL, 0);
}

void
callback_show_graph(void)
{
    GtkWidget *graph_window = 
	show_graph_window();
    
    gtk_widget_hide(lookup_widget(graph_window,
				  "optionmenu_player"));    
    show_graph(graph_window, &teams[my_team], NULL, TEAM_HISTORY_RANK);
}


/* start a new week: compute games etc. */
void
callback_new_week(gboolean calculate)
{
    GtkWidget *player_list =
	lookup_widget(main_window, "player_list");
    GtkWidget *button_new_week =
	lookup_widget(main_window, "button_new_week");
    
    if(calculate)
    {
	update_autosave();

	if(my_team_played(week))
	    gtk_tree_view_set_model(GTK_TREE_VIEW(player_list),
				    NULL);

	update_stadium();
	process_week_games(week);

	if(my_team_played(week) || my_team > 114)
	    update_finances();

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

    if(options[OPT_SKIP_WEEKS] == 1 ||
       !my_team_will_play())
	while(!my_team_played(week) &&
	      !my_team_played(week - 1) && week < 49)
	{
	    update_stadium();
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
    
    if(my_team < 114 && week == 49)
      season_awards();

	if(week == 50) {//check objective
		int i=0;		
		while(i<MAX_OBJECTIVE && objective_is_success(my_team,seasonObjective+i))
			i++;
		if(i!=MAX_OBJECTIVE)
			show_job_offer(JOB_OBJECTIVE);				
	}
    if(week == 50)
    {
		season_end();
		season++;
		week = 1;
    }

    if(stadiums[my_team].safety < (gfloat)options[OPT_DUMMY1] / 100)
	show_popup_window("Your stadium safety's low. ", NULL);


    
	if(week == OBJECTIVE_REFRESH){
		int i=0;
		gchar tmp[BIG];
		//Refresh objective
		objective_generate(my_team,seasonObjective);
		//show new objective;	  
		strcpy(tmp,_("Objective from Team Manager :"));
		for(i=0;i<MAX_OBJECTIVE;i++) {			
			gchar * obj_string=objective_get_message(seasonObjective+i);	
			if(obj_string) {
				gchar * tmpDup=strdup(tmp);
				sprintf(tmp,"%s\n- %s",tmpDup,obj_string);
				g_free(obj_string);
				g_free(tmpDup);
			}
		}
		show_popup_window(tmp,NULL);	  
    }
	
    gtk_widget_set_sensitive(button_new_week, TRUE);
}
