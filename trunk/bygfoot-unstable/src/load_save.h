#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include "bygfoot.h"
#include "variables.h"

/* try to find out which file the user wants to load
   if the input isn't correct (e.g. path omitted) */
gboolean
find_save_file(gchar *file_name);

/* check whether a file is a binary or xml
   bygfoot savegame */
gboolean
check_save_game(gchar *file_name);

/* check whether a binary file is a bygfoot savegame */
gboolean
check_save_game_binary(gchar *file_name);

/* compress or decompress a file */
void
compress_file(gchar *file_name, gint compression, gboolean decompression);

/* find out depending on the file ending whether the user wants to use
   compression and what save format he wants */
void
set_local_options(gchar *file_name,
		  gint *local_options, gchar *local_file);

/* find out whether a file ends with 'ending' */
gboolean file_has_ending(gchar *file_name, gchar *ending);

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

gboolean
load_game(gchar *file_name);

#endif
