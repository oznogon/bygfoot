void
cdf_player_column(GtkTreeViewColumn *col,
		  GtkCellRenderer   *renderer,
		  GtkTreeModel      *model,
		  GtkTreeIter       *iter,
		  gpointer           user_data);

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

/* render a cell in a fixture list */
void
cdf_fixture_column(GtkTreeViewColumn *col,
		   GtkCellRenderer   *renderer,
		   GtkTreeModel      *model,
		   GtkTreeIter       *iter,
		   gpointer           user_data);

void
cdf_table_column(GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data);

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

/* create the rows for the info on a certain team */
void
create_opponent_info(GtkListStore *liststore, gint *opponent_ids, gint idx);
