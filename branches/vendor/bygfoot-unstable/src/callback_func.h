/* try to set each of the first 11 players on his
   favoured position and sort the substitutes by position */
void rearrange_team(void);

/* return the structure that fits the positions of
   the first 11 players */
gint find_appropriate_structure(void);

/* show the players with team_ids and player_numbers stored in
   'ids' from 0 to 'max' in 'treeview'; the attributes set to 1
   in 'attrib' are shown, the rest omitted */
void show_player_info(GtkWidget *treeview, gint team_id,
		      gint player_number);

/* write the history stored in stat into a treeview */
void show_history(season_stat *stat);

/* show the 20 players given in the int arrays */
void show_best_players(gint league, gint best_players[][2]);

/* show the results of last week and the fixtures
   of next week */
void show_preview(gint *fix_indices);


/* set the week number, season number, team name, team league
   and team rank labels */
void
set_header(void);

void
get_best_players(gint league, gint best_players[][2]);
