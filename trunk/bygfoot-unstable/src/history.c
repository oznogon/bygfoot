#include "history.h"
#include "player.h"
#include "team.h"
#include <math.h>

/*********************************** PLAYER *************************************/

void reset_player_history(player * pl)
{
    g_array_free(pl->history, TRUE);
    pl->history = g_array_new(FALSE, FALSE, PLAYER_HISTORY_END * sizeof(gint));
}

void update_player_history(player * pl)
{
    /* Creates a new player history (corresponding to the current situation)
       and adds it to the existing history list */	
    gint values[PLAYER_HISTORY_END];

    values[PLAYER_HISTORY_SEASON] = season;
    values[PLAYER_HISTORY_WEEK] = week;
    values[PLAYER_HISTORY_SKILL] = (gint)rint(pl->cskill * 10);
    values[PLAYER_HISTORY_GOALS] = pl->goals;
    values[PLAYER_HISTORY_WAGE] = pl->wage;
    values[PLAYER_HISTORY_VALUE] = pl->value;
    
    g_array_append_val(pl->history, values);
}

/***************************** FINANCES *****************************************/

void reset_team_history(gint team_id)
{
    g_array_free(teams[team_id].history, TRUE);
    teams[team_id].history = g_array_new(FALSE, FALSE, TEAM_HISTORY_END * sizeof(gint));
}

void update_team_history(gint team_id)
{
    gint values[TEAM_HISTORY_END];

    values[TEAM_HISTORY_RANK] = rank[team_id];
    values[TEAM_HISTORY_PTS] = teams[team_id].results[RES_PTS];
    values[TEAM_HISTORY_GD] = teams[team_id].results[RES_GF] - teams[team_id].results[RES_GA];
    values[TEAM_HISTORY_GF] = teams[team_id].results[RES_GF];
    values[TEAM_HISTORY_GA] = teams[team_id].results[RES_GA];
    values[TEAM_HISTORY_MONEY] = (team_id == my_team) ? finances[FIN_MONEY] : 0;
    values[TEAM_HISTORY_AV_ATTENDANCE] = stadiums[team_id].average_attendance;
	
    g_array_append_val(teams[team_id].history, values);
}

/* get either a player or a team history;
   either a whole element, or a value from an element,
   or values of a certain type from each element */
gint
get_history(team *team, player *pl, gint idx, gint *values,
	    GArray *array, gint hist_type, gint type)
{
    GArray *history;
    gint max;

    if(team == NULL)
    {
	history = pl->history;
	max = PLAYER_HISTORY_END;
    }
    else
    {
	history = team->history;
	max = TEAM_HISTORY_END;
    }

    if(type == GET_HISTORY_ELEMENT)
	get_history_element(history, idx, values, max);
    else if(type == GET_HISTORY_ELEMENT_VALUE)
	return get_history_element_value(history, idx, hist_type, max);
    else if(type == GET_HISTORY_VALUES)
	get_history_values(history, array, hist_type, max);

    return 0;
}

/* write the history of a given index into the values array */
void
get_history_element(GArray *history, gint idx, gint *values, gint max)
{
    gint i;

    if(history->len <= idx)
    {
	g_print("get_history_element: index %d too high, array not long enough\n", idx);
	return;
    }

    for(i=0;i<max;i++)
	values[i] = 
	    g_array_index(history, gint, idx * max + i);
}

/* return a certain value from the history element */
gint
get_history_element_value(GArray *history, gint idx, gint type, gint max)
{
    if(history->len <= idx)
    {
	g_print("get_history_player: index %d too high, array not long enough;\n", idx);
	return 1;
    }

    return g_array_index(history,
			 gint, idx * max + type);
}

/* write the values of type 'type' of every history element into the array */
void
get_history_values(GArray *history, GArray *array, gint type, gint max)
{
    gint i;
    gint value;

    for(i=0;i<history->len;i++)
    {	
	value = g_array_index(history, gint, i * max + type);
	g_array_append_val(array, value);
    }
}
