#ifndef HISTORY_H
#define HISTORY_H

#include <gtk/gtk.h>
#include "bygfoot.h"
#include "variables.h"

typedef struct _player_history player_history;
struct _player_history
{
    gint values[PLAYER_HISTORY_END];
};


typedef struct _team_history team_history;
struct _team_history
{
    gint values[TEAM_HISTORY_END];
};

/***************** interface functions **********************/

/* player */
void reset_player_history(player * pl);

void update_player_history(player * pl);

void
get_player_history_element(player pl, player_history *history, gint idx);

/* write the values of type 'type' of every history element into the array */
void
get_player_history_values(player pl, GArray *array, gint type);


/* team */
void reset_team_history(team *tm);

void update_team_history(team *tm);

void
get_team_history_element(team tm, team_history *history, gint idx);

/* write the values of type 'type' of every history element into the array */
void
get_team_history_values(team tm, GArray *array, gint type);

#endif
