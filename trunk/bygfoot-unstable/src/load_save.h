#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include "bygfoot.h"

/* check whether a file is a bygfoot savegame */
gboolean
check_save_game(gchar *file_name);

void
save_history_best_player(player_stat pl_stat, FILE *fil);

void
load_history_best_player(player_stat *pl_stat, FILE *fil);

void
save_history_element(season_stat *stat, FILE *fil);

void
load_history_element(season_stat *stat, FILE *fil);

void
save_history(FILE *fil);

void
load_history(FILE *fil);

void
save_fixtures(FILE *fil);

void
load_fixtures(FILE *fil);

void
save_player(gint team_id, gint player_number, FILE *fil);

void
load_player(gint team_id, gint player_number, FILE *fil);

void
save_teams(FILE *fil);

void
load_teams(FILE *fil);

void
save_stadiums(FILE *fil);

void
load_stadiums(FILE *fil);

void
save_transfers(FILE *fil);

void
load_transfers(FILE *fil);

void
save_options(FILE *fil);

void
load_options(FILE *fil);

void
save_game(gchar *file_name);

void
load_game(gchar *file_name);

#endif
