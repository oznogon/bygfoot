#ifndef TEAM_H
#define TEAM_H

#include "bygfoot.h"

typedef struct _team team;
struct _team
{
    gchar name[50];

    /* id: team id
       structure: structure of the team (number of defenders,
                  midfielders and forwards) */
    gint id, structure, results[RES_END];

    /* playing style from -2 to 2: (all out) defense,
       normal, (all out) attack */
    gint style;    
    
    player players[20];

    GArray *history;
};

#include "variables.h"

/* try to set each of the first 11 players on his
   favoured position and sort the substitutes by position */
void
rearrange_team(void);

/* return the structure that fits the positions of
   the first 11 players */
gint
find_appropriate_structure(void);

/* finance and other settings for a new human player team.
   remove some players from the team. the human player's
   team has initially 17 players in the team (so that he
   has some room for new players);, the cpu-teams have 20 */    
void
set_up_my_team(void);

void
get_league_bounds(gint league, gint *bound);

/* return the number of teams in league */
gint
teams_in_league(gint league);

/* return the league (0 to 8); of a team based on its id */
gint
get_league_from_id(gint team_id);

/* return the name of the league the team plays in */
void
get_league_name_from_id(gint team_id, gchar *name);

/* return random playing style from -2 to 2 */
gint
assign_playing_style(void);

/* return a playing structure like 442 or 433 */
gint
assign_playing_structure(void);

/* compare the one or two matches the teams
   have played against each other */
gint
direct_comparison(gint team_id1, gint team_id2);

/* compare two teams regarding points etc. */
gint
team_compare_tables(const void *value1, const void *value2);

/* compare two teams regarding defensive */
gint
team_compare_defensive(const void *value1, const void *value2);

/* compare two teams regarding offensive */
gint
team_compare_offensive(const void *value1, const void *value2);

/* compare two teams regarding player skill. */
gint
team_compare_skills(const void *value1, const void *value2);



/* sort the teams between the bounds according to 'compare' and
   write the result to 'sorted' */
void
sort_teams(gint *bound, gint *sorted, 
	   gint start_idx, gint end_idx,
	   GCompareFunc compare);

/* sort the teams by points and goals and write
   the result into the global ranks array. */
void
sort_teams_ranks(gint league);

/* return the id of the team ranked 'rank' in 'league' */
gint
league_rank_id(gint league, gint rank);

/* update the player values of a team: make them
   older, better/worse etc. */
void
update_teams_players(gint team_id);

/* replace injured players by healthy substitutes in the
   cpu-teams */
void
update_teams_injuries(gint team_id);

/* substitute a couple of players in a cpu-team */
void
update_teams_substitutes(gint team_id);

/* replace some players rndomly so that there's some
   change in the cpu-teams */
void
update_teams_new_players(gint team_id);

/* change the playing structure of a cpu-team */
void
update_teams_new_structure(gint team_id);

/* weekly update of the teams.
   make players older (and maybe better /worse);.
   for cpu-teams: change the team a bit, replace injured players
   by substitutes, replace old players by newly
   generated ones etc. */
void
update_teams(void);

/* update counters like number of weeks the human player's bank account
   is negative etc. */
void
update_counters(void);

/* change the playing structure of the human player's team */
void
change_structure(gint new_structure);

/* pick a new structure from the list of common structures
   for the human player's team */
void
cycle_through_structures(void);

/* sort the tables */
void
update_ranks(void);

/* change the scout if the player has chosen so */
void
update_scout(void);

/* enlarge the stadium if necessary */
void
update_stadium(void);

void
change_my_team(gint new_team);

void
swap_stadiums(gint id1, gint id2);

/* swap two teams */
void
swap_teams(gint team_id1, gint team_id2);

/* return average skill of a team */
gfloat
average_skill(gint team_id, gint bound, gboolean cskill);

void
print_average_skill(gint league);

/* award the best teams in a season with some money */
void
season_awards_team(gint prized_teams[][2]);

/* generate new objective and display */
void change_objective();
#endif
