/************************************************************
 * the global definitions file containing everything        *
 * that's needed in more than 1 .c file                     *
 ***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gtk/gtk.h>

/* 'define's and enums */
#include "enums.h"
/* custom structures */
#include "structures.h"
/* global variables */
#include "variables.h"

/* macros */
#define rnd(lower,upper) ((gfloat)random()/(gfloat)0x7fffffff*((upper)-(lower))+(lower))
#define rndi(lower,upper) ((gint)rint( rnd((gfloat)(lower) - 0.499, (gfloat)(upper) + 0.499) ))

/* function prototypes */

/**************************************************
 * functions from callback_func.c                 *
 **************************************************/

/* get the integer value in the first column of a treeview */
gint
get_row_index(GtkTreeSelection *selection,
	      GdkEventButton *event, gint column);

/* return either the maximum amount of money the bank
   grants a team or the human player's drawing credit;
   both values depend on the players' values
   and the stadium */
gint
calculate_max_loan_credit(gint credit);

/* return the costs for improving stadium safety or
   capacity */
gint
stadium_improve_costs(gint safety, gint build);

/* show the results of last week and the fixtures
   of next week */
void
callback_show_preview(void);


/**************************************************
 * functions from gui_functions.c                 *
 **************************************************/

/* add 'directory' both to our directory list and
   GLADE's list */
void
add_support_directory(const gchar *directory);

/* find a file in the support directories */
gchar*
find_support_file                       (const gchar     *filename);

/* set up the content of the options window: 
   if read is 1, we read the global options variables
   and set the checkbuttons etc. accordingly;
   else we read the states of the buttons and 
   set the variables accordingly */
void
set_up_options_window(GtkWidget *opt_window, gint read);

/* just like the set_up_options_window, only for the notify window */
void
set_up_notify_window(GtkWidget *notify_window, gint read);

/* change the 'popups_active' label. this determines whether
   the main window is set sensitive or not */
void
change_popups_active(gint difference);

/* adjust the widget properties in the main window
   (mainly the buttons) depending on the window status
   (i.e. in which sub-menu the human player's in) */
void
set_buttons(void);

/* show the main window with the human player's team in the
   player list */
void
initialize_main_window(void);

/* get an integer from a label */
gint
get_int_from_label(GtkLabel *label);

/* set or append text or numbers into 'label' */
void
label_set_text(GtkLabel *label, gchar *text, gint append);
void
label_set_text_from_int(GtkLabel *label, gint number, gint append);
void
label_set_text_from_float(GtkLabel *label, gfloat number,
			       gint append, gint precision);

/* print a message into the message window */
void
print_message(gchar *text);

/* get the back- and foreground colors
   of a table row depending on the rank
   it represents */
void
get_table_colors(gint team_id,
		      gchar *background_color,
		      gchar *foreground_color);

/* print a number into an entry field */
void
entry_set_text_from_int(GtkEntry *entry, gint number);

/* return the entry content as an integer */
gint
entry_get_int(GtkEntry *entry);

/* keep track whether the game is saved or not */
void
set_save(gboolean status);

void
show_results(gint page);

season_stat*
get_new_stat(void);

void
live_game(gint number);

/* start a new week: compute games etc. */
void
callback_new_week(gboolean calculate);

/* make new bookmaker tips in case the user's
   team has changed significantly */
void
bookmaker_re_tip(void);


/**************************************************
 * functions from misc.c                          *
 **************************************************/

/* check whether 'item' is in array 'list' between
   'min' and 'max' */
gint
is_in_list(gint *list, gint min, gint max, gint item);

/* check whether 'item' is in array 'list' between
   'min' and 'max'; this time we compare strings */
gint
is_in_list_char(gchar list[][50], gint min,
		     gint max, gchar *item);

/* swap two integers */
void
swap_int(gint *first, gint *second);

/* swap two floats */
void
swap_float(gfloat *first, gfloat *second);

/* swap two strings */
void
swap_char(gchar **first, gchar **second);

/* round an integer */
gint
round_integer(gint number, gint places);

/* return the index 0 or 1 for the variables like
   stadium_facts[] or goals[] */
gint
get_statistics_variable_index(fixture fix);

/* print a thousands-grouped output of 'number' into 'buf',
   like 2 234 345 instead of 2234345 */
void
print_grouped_int(gint number, gchar *buf, gint append);

/* allocate the memory needed
   for 'dest' and copy 'source' into it */
void
print_string(gchar **dest, const gchar *source);

/* free the history pointers */
void
free_history(void);

/* get a float representation of someone's age
   based on his birth year and month written as 
   an integer, e.g. 6510 for 1965, Oct. */
gfloat get_age_from_birth(gint date);


/***********************************
 * functions from text_files.c     *
 ***********************************/

/* return filename corresponding to a number defined in defs.h/enums.h */
void
text_file_number_to_char(gint number, gchar *filename, gboolean full_path);

/* try to make a .bygfoot directory in the user's home directory
   and copy the support files there if possible */
void
check_home_dir(void);

/* check for the files with team and player names
   and help file */
void
check_files(void);

/* write a number of team or player names from 'filename'
   to the array 'names' */
void
get_names(gchar *filename, gchar names[][50]);

/* write the game options to the file
   $HOME/.bygfoot/bygfoot.conf */
void
save_conf_file(void);

/* read the bygfoot configurations file */
void
read_conf_file(void);


/***********************************
 * functions from math_functions.c *
 ***********************************/

/* generate a uniformly distributed random number
   between a and b */
/* gfloat */
/* rnd(gfloat a, gfloat b); */

/* generate a random integer between a and b */
/* gint */
/* rndi(gint a, gint b); */

/* generate a random number from the gauss distribution */
gfloat
gaussrand(void);

/* generate a random number from the gauss distribution N((b+c)/2, s^2)
   that's guaranteed to be between a and d and with probability 99,7%
   is between b and c */
gfloat
gauss_dist(gfloat a, gfloat b, gfloat c, gfloat d);

/* write a permutation of the integers between 'min' and 'max'
   into the array 'perm' */
gint
write_permutation(gint *perm, gint min, gint max);

/* sort a gfloat array between indices 'first_element'
   and 'last_element';
   the function assumes that 'order' can hold
   'last_element' - 'first_element' + 1 entries */
gint
sort_float_array(gfloat *array, gint *order, 
		      gint first_element, gint last_element);

/* get the 'place'th digit of the integer beginning from
   the left; if place is greather than 10, return
   the same number save that the last few digits are turned
   to 0.
   examples: value = 1345, place = 3 --> return value 3;
   value = 1345, place = 2 --> return value 1300 */
gint
get_place(gint value, gint place);

/* sum up the elements of an integer array between indices
   'min' and 'max' */
gint
sum(gint *array, gint min, gint max);


/***********************************
 * functions from player_func.c *
 ***********************************/

/* return a player's talent depending on his league and skill */
gfloat
calculate_talent(gint league, player pl);


/* determine on which position a given player plays
   depending on his number in the team and on the structure
   the team plays; if 'generation' is 1, this is position
   assignment, else just a query */
gint
get_position_from_structure(gint team_id, gint structure,
				 gint player_number,
				 gint generation);

/* estimate a player's talent; depends on the quality
   of the human player's scout */
gfloat
estimate_talent(player pl);

/* assign value and wage for a player, depending
   on skill, talent, league and age */
gint
assign_value(player pl);
gint
assign_wage(player pl);

/* fill in a player's data depending on league and
   the player's number in his team */
void
generate_player(gint team_id, gfloat team_factor,
		     gint player_number);

/* return the number of players in the human player's team */
gint
players_in_team(void);

/* swap two players */
void
swap_players(gint team_id1, gint team_id2,
		  gint player_number1, gint player_number2);

/* remove a player from a team */
void
remove_player(gint team_id, gint player_number);

/* move the player from team1 to team2 and replace
   him by a new one */
void
move_player(gint team1, gint player_number, gint team2);

/***********************************
 * functions from team_func.c *
 ***********************************/

/* finance and other settings for a new human player team.
   remove some players from the team. the human player's
   team has initially 17 players in the team (so that he
   has some room for new players), the cpu-teams have 20 */    
void
set_up_my_team(void);

void
get_league_bounds(gint league, gint *bound);

/* return the number of teams in league */
gint
teams_in_league(gint league);

/* return the league (0 to 8) of a team based on its id */
gint
get_league_from_id(gint team_id);

/* return the name of the league the team plays in */
void
get_league_name_from_id(gint team_id, gchar *name);

/* return a playing structure like 442 or 433 */
gint
assign_playing_structure(void);

/* return random playing style from -2 to 2 */
gint
assign_playing_style(void);

/* change the playing structure of the human player's team */
void
change_structure(gint new_structure);

/* sort the transfer list */
void
sort_transferlist(gint sort_attrib);

/* how many players there are on the transferlist */
gint
players_on_transferlist(void);

/* whether the player is on the transferlist */
gint
is_on_transferlist(gint team_id, gint player_number);

/* add the specified player to the transferlist; he'll stay there
   for 'time' weeks */
void
add_transfer(gint team_id, gint player_number, gint time);

/* remove a player from the transfer list and resort the list (sometimes) */
void
remove_transfer(gint number, gboolean resort);

/* move a player from the transferlist to the human player's team
   and replace him */
void
execute_transfer(gint idx);

/* swap two teams */
void
swap_teams(gint team_id1, gint team_id2);

/* return interest rate depending on user's debt */
gfloat
interest_rate(void);

/* find out at the end of a season whether the human player's team
   was ranked among the first 3. if so, he gets some prize money */
void
prize_money(void);

/* return the id of the team ranked 'rank' in 'league' */
gint
league_rank_id(gint league, gint rank);

/* read the teams file which specifies (perhaps)
   some data about the players of a team;
   'read' tells us which parts to read: either
   names and values (read=2), only names (1) or
   nothing(0); if 'team_name' isn't NULL, read that
   team to team number 114; if birth_dates isn' NULL,
   birth dates are stored within */
void
read_teams_file(gint read, const gchar *team_name, gint *structure2,
		     gint *birth_dates);

/* return average skill of a team */
gfloat
average_skill(gint team_id, gint bound, gboolean cskill);

void
print_average_skill(gint league);

/* try to set each of the first 11 players on his
   favoured position and sort the substitutes by position */
void
callback_rearrange(gboolean change);

/***********************************
 * functions from fixtures.c       *
 ***********************************/

/* return the index of the first fixture with type 'type' */
gint
get_first_fixture(gint type);

/* find out if the human player's team is involved
   in the fixture */
gboolean
my_team_involved(fixture fix);

/* find out whether the human player's team
   plays (or played) the given week */
gboolean
my_team_played(gint week_number);

/* return winner or runner-up of the game
   with fixture type 'type' */
gint
get_winner_runner_up(gint type, gint winner);

/* return the id of the winner of fix */
gint
winner_of(fixture fix);

/* write the fixtures for the round robin games of
   a season and the first rounds of the cups */
void
write_season_fixtures(void);

/* return the id of a team that reached
   every round of a cup that currently exists */
gint
get_best_team(void);


/***********************************
 * functions from treeview_func.c  *
 ***********************************/

/* remove all columns in the treeview */
void
clear_treeview(GtkWidget *treeview);

/* return the number in the first column of the currently
   selected row of the treeview */
gint
get_index(GtkTreeView *treeview);

/* show a list of teams in the treeview; if 'european'
   is 1, show also the european teams */
void
show_team_list(GtkWidget *treeview, gint european);

/* show the players with team_ids and player_numbers stored in
   'ids' from 0 to 'max' in 'treeview'; the attributes set to 1
   in 'attributes' are shown, the rest omitted */
void
show_players(GtkWidget *treeview, gint ids[][2], gint max,
		  gint *attrib, gint type);

/* show the transferlist */
void
show_transfers(void);

/* show a team in 'team browse' mode so that the
   human player can try to buy a player not on
   the transferlist */
void show_team_browse(gint team_id, GtkWidget *treeview);



/***********************************
 * functions from generation.c     *
 ***********************************/

/* generate the european teams participating in
   champions' league etc. */
void
fill_in_euro_teams(void);

/* select team and cup names according to country */
void
country_names(gint country_number, const gchar *country_filename);


/***********************************
 * functions from games.c          *
 ***********************************/

/* get the number of goals for 'team' in the first leg */
gint
get_first_leg_result(fixture fix, gint team);

/* determine whether a game is a draw. this is simple if we have a
   single game but slightly more complicated if we have the second leg
   of a home-away pair of games */
gboolean
is_draw(fixture fix, gint type);

/* calculate the values that decide a game */
void
prg_calculate_att_def(fixture fix, gfloat *attack_value,
			   gfloat *defend_value, gfloat *goalie_value,
			   gfloat *home_advantage);

/* if bookmaker is TRUE we don't touch other variables
   than the fixture to write the result because we only want
   to have a bookmaker's tip */
void
process_result_game(fixture *fix, gfloat *home_advantage,
			 gint extra_time, gboolean bookmaker);



/****************************
 * functions from startup.c *
 ****************************/

/* start a new game: generate teams etc. */
void
start(gint new_game);



/**************************************************
 * functions from editor_gui_functions.c          *
 **************************************************/

/* start the team editor */
void
start_editor(void);

/* create an empty bygfoot def file */
gint
create_new_def_file(void);

/* save currently edited team to file */
gint
callback_save_defs(void);

/* save currently edited team to the
   defs file or alternatively to the argument file */
void
save_defs(const gchar *save_file);

/**************************************************
 * functions from windows.c                       *
 **************************************************/

/* show the file selection window */
void
show_file_selection(gint save);

/* show the team selection window at the beginning of the game */
void
show_team_selection(void);

/* show a popup window with 'text' and adjust the
   window buttons according to the 'status' integers */
void
show_popup_window(gchar *text, gint *status);

void
show_font_sel_window(void);

/* start the team editor */
void
start_editor(void);

/* create the live window and return the pointer */
GtkWidget*
return_live_window(void);

/* create the main window and return the pointer */
GtkWidget*
return_main_window(void);

/* pop up the help window */
void
show_help(gint customization);

/* create and show notification options window */
void
show_notify_window(void);

/* show a window with a job offer and some info on the possible new team */
void
show_job_offer_window(gchar buf[][BUF_SIZE_SMALL]);



/**************************************************
 * functions from callback.c                      *
 **************************************************/

void
on_button_back_to_main_clicked         (GtkButton       *button,
                                        gpointer         user_data);
void
on_button_preview_clicked              (GtkButton       *button,
                                        gpointer         user_data);
void
on_button_quit_clicked                 (GtkButton       *button,
                                        gpointer         user_data);
void
on_menu_new_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_button_new_week_clicked             (GtkButton       *button,
                                        gpointer         user_data);
void
on_start_editor_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);




/**************************************************
 * functions from load_save.c                     *
 **************************************************/

/* check whether a file is a bygfoot savegame */
gboolean
check_save_game(gchar *file_name);

/* load or save a game */
void
load_game(gchar *file_name);
void
save_game(gchar *file_name);
