/******************************************************************
 * Functions involving the gui AND some visible game events       *
 ******************************************************************/
#include <unistd.h>

#include "callback_func.h"
#include "callbacks.h"
#include "game_gui.h"
#include "game.h"
#include "generation.h"
#include "gui.h"
#include "finance.h"
#include "fixture.h"
#include "load_save.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "support.h"
#include "team.h"
#include "transfer.h"
#include "treeview.h"
#include "treeview_cell.h"
#include "window.h"

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
    gchar buf[SMALL];
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
    gchar buf[JOB_OFFER_END][SMALL];
    gchar buf2[SMALL];
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
    gchar buf[SMALL];

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
    gchar buf[SMALL];
    
    if(options[OPT_AUTOSAVE] < 0)
	return;

    counters[COUNT_AUTOSAVE]--;

    if(counters[COUNT_AUTOSAVE] > 0)
	return;

    counters[COUNT_AUTOSAVE] = options[OPT_AUTOSAVE];

    sprintf(buf, "%s/.bygfoot/saves/autosave", g_get_home_dir());

    save_game(buf);
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
    gchar buf[SMALL];
    gchar buf2[SMALL];

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
	if(i<12)
	    for(j=0;j<2;j++)
		injuries[j][i] = booked[j][i] = -1;
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
  gint i;
  gint best_players[6][2];
    
  get_best_players(get_league_from_id(my_team),
		   best_players, 10, 0, 2,
		   player_compare_goalgetters);
  get_best_players(get_league_from_id(my_team),
		   best_players, 0, 3, 5,
		   player_compare_goalies);
  for(i=0;i<6;i++)
    {
      stat->best_players[i].games =
	teams[best_players[i][0]].
	players[best_players[i][1]].games;
      stat->best_players[i].goals =
	teams[best_players[i][0]].
	players[best_players[i][1]].goals;
      strcpy(stat->best_players[i].name,
	     teams[best_players[i][0]].
	     players[best_players[i][1]].name);
      strcpy(stat->best_players[i].team_name,
	     teams[best_players[i][0]].name);
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

/* give some awards and prize money to teams and players
   at the end of a season */
void
season_awards(void)
{
  /* ids and prize money for teams and players */
  gint prized_teams[PRIZE_TEAM_END][2];
  gint prized_players[PRIZE_PLAYER_END][3];

  season_awards_team(prized_teams);
  season_awards_player(prized_players);

  show_season_awards(prized_teams, prized_players);
}

/* change the country in the team selection window
   when the user clicks on a flag */
void
change_country_team_selection(GtkWidget *button)
{
    gint i;
    GtkWidget *radiobutton_country[FILES_PLAYER_NAMES];
    GtkWidget *team_selection_treeview =
	lookup_widget(button, "team_selection_treeview");    

    radiobutton_country[FILES_COUNTRY_ENG] = 
	lookup_widget(button, "radiobutton_country0");
    radiobutton_country[FILES_COUNTRY_DE] = 
	lookup_widget(button, "radiobutton_country1");
    radiobutton_country[FILES_COUNTRY_IT] = 
	lookup_widget(button, "radiobutton_country2");
    radiobutton_country[FILES_COUNTRY_FR] = 
	lookup_widget(button, "radiobutton_country3");
    radiobutton_country[FILES_COUNTRY_ES] = 
	lookup_widget(button, "radiobutton_country4");
    radiobutton_country[FILES_COUNTRY_RO] = 
	lookup_widget(button, "radiobutton_country5");
    radiobutton_country[FILES_COUNTRY_BR] = 
	lookup_widget(button, "radiobutton_country6");
    radiobutton_country[FILES_COUNTRY_PL] = 
	lookup_widget(button, "radiobutton_country7");
    radiobutton_country[FILES_COUNTRY_MX] = 
	lookup_widget(button, "radiobutton_country8");

    for(i=0;i<FILES_PLAYER_NAMES;i++)	
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
    gchar buf[SMALL], 
	buf2[SMALL], buf3[SMALL];
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
	if(goals[goal_idx][i].time == GOAL_TIME_PENALTY)
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
	status = -60000;

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
	callback_new_week(FALSE);
	return;
    }
    
    show_live_game(idx[number]);

    /* we have already shown the second game of the week
       or we only had one game this week */
    if(number == 1 || idx[1] == -1)
	status = -50000;

    return;
}

