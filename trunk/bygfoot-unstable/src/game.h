#ifndef GAMES_H
#define GAMES_H

#include "bygfoot.h"

typedef struct _goal goal;
struct _goal
{
    gint minute;    
    gint team_id;
    gint scorer;
    gint time;
    gint type;
};

#include "variables.h"

/* get the number of goals for 'team' in the first leg */
gint
get_first_leg_result(fixture fix, gint team);

/* determine whether a game is a draw. this is simple if we have a
   single game but slightly more complicated if we have the second leg
   of a home-away pair of games.
   type indicates whether we have a result after extra-time or
   penalties:
   type = 0: regulation
   type = 1: extra-time
   type = 2: penalties */
gboolean
is_draw(fixture fix, gint type);

/* the contribution of a player to the attack or defend value of
   his team */
gfloat
prg_player_contribution(gint team_id, gint player_number,
			   gfloat home_advantage, gint attack);

/* calculate the values that decide a game */
void
prg_calculate_att_def(fixture fix, gfloat *attack_value,
			   gfloat *defend_value, gfloat *goalie_value,
			   gfloat *home_advantage);

/* calculate the scoring probabilites of the field players */
void
prg_calculate_player_probs(fixture fix,
				gfloat player_score_probs[][9]);

/* calculate the values that decide a game */
void
prg_calculate_values(fixture fix, gfloat *attack_value,
			  gfloat *defend_value, gfloat *goalie_value,
			  gfloat *scoring_probability,
			  gfloat player_score_probs[][9],
			  gfloat *home_advantage);

/* store information about a goal */
void
prg_write_goal(fixture fix, gint team, gint scorer,
		    gint time, gint type);

void
sort_best_players(gint team_id, gint *order);

void
get_scorer_pen(gint team_id, gint *scorer);

void
get_scorer(gfloat *player_score_probs, gint *scorer, gboolean own_goal);

/* simulate one goal chance */
void
prg_simulate_chance(fixture *fix,
		    gfloat *scoring_probability,
		    gfloat player_score_probs[][9],
		    gfloat goalie_value,
		    gint extra_time, gint team,
		    gboolean bookmaker);

/* simulate a number of goal chances and store the results
   in appropriate places */
void
prg_simulate_chances(fixture *fix, gfloat *attack_value,
		     gfloat *defend_value, gfloat *goalie_value,
		     gfloat *scoring_probability,
		     gfloat player_score_probs[][9],
		     gint extra_time, gboolean bookmaker);

/* if bookmaker is TRUE we don't touch other variables
   than the fixture to write the result because we only want
   to have a bookmaker's tip */
void
process_result_game(fixture *fix, gfloat *home_advantage,
			 gint extra_time, gboolean bookmaker);

/* simulate a penalty shot */
void
simulate_penalty(fixture *fix, gfloat goalie_value,
		 gint player_number, gint team, gint time,
		 gboolean bookmaker);

/* calculate penalties. here we have no home advantage */
void
process_result_penalties(fixture *fix);

void
calculate_booking_player(fixture fix, gint team,
			      gint player_number);

void
calculate_injury_player(fixture fix, gint team,
			     gint player_number);

/* find out if there were injuries or bookings */
void
process_calculate_inj_book(fixture fix);

gfloat
get_stadium_percentage(fixture fix);

gint
stadium_events(fixture fix);

void
process_stadium(fixture fix);

void
process_game_general(fixture fix);

void
process_game_sort_goals(fixture fix);

/* calculate the result of the game stored in 'fix' */
void
process_game(fixture *fix);

/* calculate the results of a week's games */
void
process_week_games(gint week_number);

#endif
