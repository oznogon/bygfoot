#ifndef TREEVIEW_CELL_H
#define TREEVIEW_CELL_H

#include "bygfoot.h"

/* print some information about the injury of
   the player into 'buf' */
void
set_injury_info_into_cell(gint team_id, gint player_number,
			  gchar *buf);

/* print some information about why the player's banned into 'buf' */
void
set_ban_info_into_cell(gint team_id, gint player_number,
		       gchar *buf);

void
set_status_into_cell(GtkCellRenderer *renderer,
		     gint team_id, 
		     gint player_number,
		     gchar *buf,
		     gint type);

void
set_position_into_cell(GtkCellRenderer *renderer,
		       gint team_id, 
		       gint player_number,
		       gint position,
		       gchar *buf);

void
cdf_player_column(GtkTreeViewColumn *col,
		  GtkCellRenderer   *renderer,
		  GtkTreeModel      *model,
		  GtkTreeIter       *iter,
		  gpointer           user_data);

void
league_to_string(gint league, gchar *buf);

/* a cell data function (cdf); that converts the gint league value
   (of teams); into a string like "Premier Division" or "Division 2" */
void
cdf_league_to_cell (GtkTreeViewColumn *col,
		    GtkCellRenderer   *renderer,
		    GtkTreeModel      *model,
		    GtkTreeIter       *iter,
		    gpointer           user_data);

/* display an int value  */
void
cdf_int_to_cell (GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data);

/* print something like 'Champions' League Semi-finals'.
   probably the ugliest part of the whole program */
void
fixture_type_to_string(gint type, gint column, gchar *buf);

/* print the result of a game into a string */
void
fixture_result_to_string(gint fix_idx, gchar *buf);

/* render a cell in a fixture list */
void
cdf_fixture_column(GtkTreeViewColumn *col,
		   GtkCellRenderer   *renderer,
		   GtkTreeModel      *model,
		   GtkTreeIter       *iter,
		   gpointer           user_data);

void
team_id_to_table_title(gint team_id, gchar *buf);

void
team_id_to_table_string(gint team_id, gchar *buf,
			gint column, GtkCellRenderer *renderer);

void
cdf_table_column(GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data);

void
set_result_to_string(gint team_id, gchar *buf,
		     gint away);

void
cdf_league_result_column(GtkTreeViewColumn *col,
			 GtkCellRenderer   *renderer,
			 GtkTreeModel      *model,
			 GtkTreeIter       *iter,
			 gpointer           user_data);

void
cdf_best_players(GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data);

void
cdf_opponents (GtkTreeViewColumn *col,
	       GtkCellRenderer   *renderer,
	       GtkTreeModel      *model,
	       GtkTreeIter       *iter,
	       gpointer           user_data);

/* set the result of previous games against one team
   into a string */
void
my_result_to_string(gint fix_idx, gchar *buf);

/* write the previous results of the human player's team
   against a certain cpu team into a row */
void
write_my_results(GtkListStore *liststore, gint team_id);

/* write the outcomes of the last games of a team in
   'WWLLDW...' form into a row */
void
write_last_games(GtkListStore *liststore, gint team_id);

/* simulate a game and write the result as a bookmaker tip into the
   bookmaker variables */
void
write_bookmaker(GtkListStore *liststore, gint *opponent_ids, gint idx);

void
style_to_string(gint style, gchar *buf);

/* create the rows for the info on a certain team */
void
create_opponent_info(GtkListStore *liststore, gint *opponent_ids, gint idx);

#endif
