#ifndef HISTORY_H
#define HISTORY_H

#include <gtk/gtk.h>
#include "bygfoot.h"
#include "player.h"

/***************** interface functions **********************/

/* player */
void reset_player_history(player * pl);
void update_player_history(player * pl);
/* void print_player_history(struct _player * pl); */

/* finances */
void reset_team_history(gint team_id);
void update_team_history(gint team_id);
/* void print_finances_history(void); */

/* write the history of a given index into the values array */
void
get_history_element(GArray *history, gint idx, gint *values, gint max);

/* return a certain value from the history element */
gint
get_history_element_value(GArray *history, gint idx, gint type, gint max);

/* write the values of type 'type' of every history element into the array */
void
get_history_values(GArray *history, GArray *array, gint type, gint max);

#endif
