#ifndef GAME_GUI_H
#define GAME_GUI_H

#include "bygfoot.h"
#include "variables.h"

/* make new bookmaker tips in case the user's
   team has changed significantly */
void
bookmaker_re_tip(void);

/* show a popup window with an offer for a 
   player to transfer */
void
make_transfer_offer(gint idx);

/* check for each player of the human player's team
   on the transferlist whether he'll be bought or not */
void
transfer_offers(void);

gint
get_current_rank(void);

/* show a popup window with either a job offer
   or a note that the human player's been fired */
void
show_job_offer(gint fire);

void
show_fire_warning(void);

/* look at the counters and fire the human player 
   if his team is unsuccessful; or make a job offer
   if they're successful */
void
team_offers(void);

/* autosave the game if necessary */
void
update_autosave(void);

void
show_results(gint page);

/* update some variables each week */
void
update_variables(void);

void
finance_events(void);

/* find out the winners of the promotion games */
void
get_promotion_winners(gint *promotion_winners);

/* promotions and relegations */
void
season_end_prom_rel(void);

/* generate new european teams
   and get the right engl. teams */
void
season_end_euro(void);

/* nullify some counters and
   number of games and goals etc. */
void
season_end_nullify(void);

/* change each team slightly to have
   something new in the new season */
void
season_end_change_teams(void);

season_stat*
get_new_stat(void);

void
history_get_champions(season_stat *stat);

void
history_get_best_players(season_stat *stat);

/* return the rank of the human player if
   he's played in a regular championship
   or his result if he's played in a cup only */
gint
history_get_rank(void);

/* fill in a new history element at
   the end of the season with cup winners
   and champions etc. */
void
write_new_history(void);

/* end a season and begin a new one by promoting /
   relegating the appropriate teams; find out who's
   participating in the european cups, write new fixtures
   etc. */
void
season_end(void);

/* give some awards and prize money to teams and players
   at the end of a season */
void
season_awards(void);

/* change the country in the team selection window
   when the user clicks on a flag */
void
change_country_team_selection(GtkWidget *button);

gboolean
my_team_will_play(void);

gint
get_game_end(gint idx);

/* get the left/right probabilitiy for the tendency bar */
gfloat
get_tendency_prob(gint idx, gdouble value);

/* get a new value for the tendency bar in the live game window */
gdouble
update_tendency(GtkWidget *hscale_tendency,
			gint minute, gint idx, gdouble *old_values,
			gfloat stick_prob);

void
show_live_game(gint idx);

/* show a live game */
void
live_game(gint number);

#endif
