#ifndef STARTUP_H
#define STARTUP_H

/* write default options and other
   global variables.
   meanings of the options see non_gui_defs.h */
void
write_global_variables(gint new_game);

/* start a new game: generate teams etc. */
void
start_new_game(gint new_game);

/* show the initial team selection window */
void
start(gint new_game);

void
set_my_team(gint team_id, gint league);

/* get the id of the human player's team according
   to the list in the team selection window */
void
get_my_team(GtkWidget *widget);

#endif
