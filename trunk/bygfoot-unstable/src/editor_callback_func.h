#ifndef EDITOR_CALLBACK_FUNC_H
#define EDITOR_CALLBACK_FUNC_H

void
set_spinbutton_values(void);

void
write_average_skill(void);

void
write_team_to_screen(void);

void
callback_change_structures(void);

void
set_teams_file_pulldown(void);

void
callback_load_team_file(void);

void
callback_load_team(void);

/* save the name of the edited team to
   the country file */
void
save_team_name(void);

/* save currently edited team to the
   defs file or alternatively to the argument file */
void
save_defs(const gchar *def_file);

gboolean
callback_save_defs(void);

void
callback_swap_players(void);

void
callback_play(gint save);

void
callback_edit_player(GtkTreeSelection *selection,
			  GdkEventButton *event);

void
callback_update(void);

void
callback_move(gint direction);

/* select next or previous player in the list */
void
callback_change_player(gint direction);

void
callback_change_average_skill(void);

/* extend the team definitions file by the contents
   of another team file */
void
import_file(const gchar *file_name);

#endif
