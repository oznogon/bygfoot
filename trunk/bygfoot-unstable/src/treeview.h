#ifndef TREEVIEW_H
#define TREEVIEW_H

/* return the number in the first column of the currently
   selected row of the treeview */
gint
get_index(GtkTreeView *treeview);

/* remove all columns in the treeview */
void
clear_treeview(GtkWidget *treeview);

/* create the model for the treeview in the team selection window.
   it contains the names and the divisions of all 114 english teams
   appearing in the game */
GtkTreeModel*
create_team_selection_list(gint european);

/* set up the tree view for the team selection window */
void
set_up_team_selection_treeview (GtkWidget *treeview);

/* show a list of teams in the treeview; if 'european'
   is 1, show also the european teams */
void
show_team_list(GtkWidget *treeview, gint european);

/* the sort function for the transfer list */
gint
sort_column(GtkTreeModel *model,
		 GtkTreeIter *a,
		 GtkTreeIter *b,
		 gpointer user_data);

/* create a player liststore of the players with
 team_ids and player_numbers in ids[][];
 'attrib' determines which columns are shown */
GtkTreeModel*
create_player_list(gint ids[][2], gint max,
				 gint separator,
				 gint *attrib, gint type);

void
set_up_player_list_column(GtkWidget *treeview, gint column,
			       gint attribute, gint type);

/* set up the tree view for a player list */
void
set_up_player_list (GtkWidget *player_list, gint *attrib, gint type);

/* show the players with team_ids and player_numbers stored in
   'ids' from 0 to 'max' in 'treeview'; the attributes set to 1
   in 'attrib' are shown, the rest omitted.
   type = 0: a team
   type = 10: the transferlist */
void
show_players(GtkWidget *treeview, gint ids[][2], gint max,
		  gint *attrib, gint type);

/* set up the tree view for detailed info on a player */
void
set_up_player_info (GtkWidget *player_info);

/* create the model for the detailed player info.
   in the first column we write the categories,
   in the second only the encoded team_id, player_number
   and attribute (just like in the create_player_list
   function); */
GtkTreeModel*
create_player_info(gint team_id, gint player_number);

/* show the players with team_ids and player_numbers stored in
   'ids' from 0 to 'max' in 'treeview'; the attributes set to 1
   in 'attrib' are shown, the rest omitted */
void
show_player_info(GtkWidget *treeview, gint team_id,
		      gint player_number);

void
set_attribs_from_scout(gint *attrib);

/* show the transferlist */
void
show_transfers(void);

void
set_average_skills(gint team_id);

/* show a team in 'team browse' mode so that the
   human player can try to buy a player not on
   the transferlist */
void
show_team_browse(gint team_id, GtkWidget *treeview);

/* set up the treeview for displaying fixtures */
void
set_up_fixtures(GtkWidget *treeview);

void
find_leagues(gint *leagues, gint week_number);

void
create_fixture_liststore(GtkListStore *liststore,
			      gint week_number);

/* create a new model for fixtures and fill it or append
   new data to an existing model */
GtkTreeModel*
create_fixtures(gint week_number);

/* show the fixtures for the given week and leagues */
void
show_fixtures(gint week_number);

/* fill a table with the teams ranked between the two bounds */
void
fill_table_liststore(GtkListStore *liststore, gint *bound);

/* create the model for a table */
GtkTreeModel*
create_tables(gint league);

/* set up the treeview for displaying a table */
void
set_up_table(GtkWidget *treeview);

/* show tables of an english division or CL group */
void
show_tables_league(gint league);

/* create the model for the league results */
GtkTreeModel*
create_league_results(void);

/* set up the treeview for displaying league results
   of the human player's team */
void
set_up_league_results(GtkWidget *treeview);

/* show the human player's league results of the season
   by team */
void
show_league_results(void);

/* fill a model with finances and stadium info */
GtkTreeModel*
create_fin_stad(void);

/* set up the treeview for displaying finances and stadium
   info */
void
set_up_fin_stad(GtkWidget *treeview);

/* show finances and stadium info */
void
show_fin_stad(void);

GtkTreeModel*
create_best_players(gint league,
				  gint best_players[][2]);

void
set_up_best_players(GtkWidget *treeview);

/* show the 20 players given in the int arrays */
void
show_best_players(gint league, gint best_players[][2]);

void
create_extended_fixture(GtkListStore *liststore,
			     gint fix_idx,
			     gint show_ext_info,
			     gint show_week_number);

GtkTreeModel*
create_preview(gint *fix_indices);

/* show the results of last week and the fixtures
   of next week */
void
show_preview(gint *fix_indices);

/* convert the rank of the human player into
   text */
void
stat_rank_to_text(gint rank, gchar *buf);

GtkTreeModel*
create_history(season_stat *stat);

void
set_up_history(GtkWidget *treeview);

/* write the history stored in stat into a treeview */
void
show_history(season_stat *stat);

void
get_result_minute(gint idx, gint minute,
		       gint *result);

void
get_result_penalties(gint idx, gint max_goal,
			  gint *result);

gint
get_extra_time(gint idx);

GtkTreeModel*
create_live(gint idx, gint type);

void
set_up_live(GtkWidget *treeview);

void
show_live_window(GtkWidget *live_window, gint idx, gint type);

/* get the ids of the opponents of the user's team
   this week; store the type of game (home/away/neutral); */
void
get_opponent_ids(gint *opponent_ids);

GtkTreeModel*
create_opponents(void);

void
set_up_opponents(GtkTreeView *treeview);

/* show info about the team(s); the user will play against
   the current week */
void
show_next_opponents(void);

void
create_awards_player(GtkListStore *liststore, gint prized_players[][3]);

void
create_awards_team(GtkListStore *liststore, gint prized_teams[][2]);

GtkTreeModel*
create_awards(gint prized_teams[][2], gint prized_players[][3]);

void
set_up_awards(GtkTreeView *treeview);

/* show the awards featuring the teams, players and prize
   money given in the arguments in a treeview */
void
show_season_awards(gint prized_teams[][2], gint prized_players[][3]);

#endif
