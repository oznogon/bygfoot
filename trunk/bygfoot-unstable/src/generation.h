#ifndef GENERATION_H
#define GENERATION_H

#include "bygfoot.h"
#include "variables.h"

/* name all players in each team between first_team and last_team */
void
give_player_names(gint first_team, gint last_team);

/* choose names of uefa cup participants */
void
choose_european_names_uefa(gchar all_names[][50],
				gchar european_names[][50]);

/* choose a team name from a given country which isn't
   yet on the list */
void
choose_european_name(gchar all_names[][50],
			  gint country, gint name_idx,
			  gchar european_names[][50]);

/* choose the names for the european
   teams from the names array; this ensures
   that each season there are different teams
   participating in the european cups */
void
choose_european_names(gchar european_names[][50], gchar all_names[][50]);

/* generate the twenty players of a team */
void
generate_players(gint team_id);

/* fill in data for a team stadium */
void
fill_in_stadium(gint team_id);

/* read team names from a file */
void
give_teams_names(gboolean only_euro);

/* fill in all the data for a team: name, players,
   stadium etc. */
void
fill_in_team(gint team_id);

/* generate the european teams participating in
   champions' league etc. */
void
fill_in_euro_teams(void);

/* fill in non-euro-teams */
void
fill_in_teams(void);

void
give_leagues_names(void);

/* select team and cup names according to the country given as
   an integer or a string */
void
country_names(gint country_number, const gchar *country_filename);

/* generate teams */
void
generate_teams(void);

/**
 * Generate wekk index for refresh objective
 */
void generate_objective_refresh_week();

#endif
