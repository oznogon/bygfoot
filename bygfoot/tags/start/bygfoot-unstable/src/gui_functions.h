/* enlarge the stadium if necessary */
void
update_stadium(void);

/* show info about the team(s) the user will play against
   the current week */
void show_next_opponents(void);

/* print something like 'Champions' League Semi-finals'.
   probably the ugliest part of the whole program */
void fixture_type_to_string(gint type, gint column, gchar *buf);

void show_live_window(GtkWidget *live_window, gint idx, gint type);

/* weekly update of the fixtures, i.e. new fixtures are
   generated when a round of a cup is over */
void update_fixtures(void);

/* change the scout if the player has chosen so */
void update_scout(void);

/* sort the tables */
void update_ranks(void);

/* weekly update of the transferlist */
void update_transferlist(void);

/* update the finances of the human player's team */
void update_finances(void);

/* update counters like number of weeks the human player's bank account
   is negative etc. */
void update_counters(void);

/* weekly update of the teams.
   make players older (and maybe better /worse).
   for cpu-teams: change the team a bit, replace injured players
   by substitutes, replace old players by newly
   generated ones etc. */
void update_teams(void);

/* calculate the results of a week's games */
void process_week_games(gint week_number);



