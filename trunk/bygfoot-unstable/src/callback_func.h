#ifndef CALLBACK_FUNC_H
#define CALLBACK_FUNC_H

#include "bygfoot.h"

/* get the integer value in the first column of a treeview */
gint
get_row_index(GtkTreeSelection *selection,
	      GdkEventButton *event, gint column);

/* find out which player's been clicked on and
   show the detailed info */
void
callback_show_player_info(GtkTreeSelection *selection,
			  GdkEventButton *event);

/* handle a left-click on the player-list.
   swap two players, if necessary */
void
callback_select_player(GtkTreeSelection *selection,
			    GdkEventButton *event);

/* undo the swapping of two players */
void
callback_undo(void);

/* show the scout's recommendation for a transfer */
void
show_recommend(gint idx);

/* handle a click on the transfer list */
gint
callback_transfer_select(gint row_idx);

/* handle the click on the 'ok' button the human player
   makes a transfer offer with */
void
callback_make_transfer_offer(GtkWidget *widget);

/* handle a click on the 'browse teams' button */
void
callback_transfer_team_select(gint row_idx);

/* show the next or previous team in browse mode */
void
callback_transfers_browse_teams(gint direction);

/* handle a left-click in the player_info when 
   the human player browses the teams to buy players */
void
callback_transfer_buy_player(gint row_idx);

/* move the player to the human player's team and deduce
   transfer fee */
void
callback_popup_buy_player(gint team_id, gint player_number);

/* try to set each of the first 11 players on his
   favoured position and sort the substitutes by position */
void
callback_rearrange(gboolean change);

void
callback_sell_player(gint trans_idx, gint fee);

/* improve stadium capacity or safety */
void
callback_stadium_improve(void);

/* return either the maximum amount of money the bank
   grants a team or the human player's drawing credit;
   both values depend on the players' values
   and the stadium */
gint
calculate_max_loan_credit(gint credit);

gint
callback_get_loan(void);

gint
callback_pay_loan(void);

gint
callback_get_the_loan(void);

gint
callback_pay_the_loan(void);

/* sort the field players and goalies and
   show the best 10 */
void
callback_show_best_players(gint league);

/* show the history of the given season */
void
callback_show_history(gint season_number);

void
get_fix_indices(gint *fix_indices, gint week_number);

/* show the results of last week and the fixtures
   of next week */
void
callback_show_preview(void);

void
get_option_widgets(GtkWidget *opt_window,
			gint **zero_one_options,
			GtkWidget **zero_one_widgets,
			GtkWidget **radio_buttons,
			GtkWidget **radiobutton_sort);

/* set up the content of the options window: 
   if read is 1, we read the global options variables
   and set the checkbuttons etc. accordingly;
   else we read the states of the buttons and 
   write the variables accordingly */
void
set_up_options_window(GtkWidget *opt_window, gint read);

void
get_check_widgets(GtkWidget *notify_window, GtkWidget **notify_check);

void
get_spin_widgets(GtkWidget *notify_window, 
		      GtkWidget **notify_spin);

/* just like the set_up_options_window, only for the notify window */
void
set_up_notify_window(GtkWidget *notify_window, gint read);

/* handle a click on the OK button of the stadium capacity increase window */
void
callback_improve_stadium(GtkWidget *widget);

/* remove the selected player from the transfer list */
void
callback_rm_pl_transfer(void);

/* fire the selected player */
void
callback_fire_player(GtkTreeSelection *selection,
		     GdkEventButton *event);

/* show a popup telling the user that some players' careers
   had to stop because of a severe injury */
void
callback_notify_injury(void);

/* make the selected player the one responsible for penalties */
void
callback_penalty_shooter(void);

/* start a new week: compute games etc. */
void
callback_new_week(gboolean calculate);

#endif
