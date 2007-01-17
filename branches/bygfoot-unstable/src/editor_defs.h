/* in editor mode we don't need some globale variables
   but need others so we rename them */
#define structure2 save_status
#define selected_player notify_status
#define birth_dates rank

void callback_load_team_file(void);

/* load a team from the team def file */
void callback_load_team(void);

void callback_change_structures(void);

void set_teams_file_pulldown(void);

void callback_swap_players(void);

void callback_play(gint save);

void callback_edit_player(GtkTreeSelection *selection,
			  GdkEventButton *event);

void callback_update(void);

void callback_move(gint direction);

/* select next or previous player in the list */
void callback_change_player(gint direction);

void callback_change_average_skill(void);
