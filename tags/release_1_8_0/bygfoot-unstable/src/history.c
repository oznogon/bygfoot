#include "history.h"
#include "player.h"
#include "team.h"
#include <math.h>

/*********************************** PLAYER *************************************/

void reset_player_history(player * pl)
{
    if(pl->history != NULL)
	g_array_free(pl->history, TRUE);
    pl->history = g_array_new(FALSE, FALSE, sizeof(player_history));
}

void update_player_history(player * pl)
{
    /* Creates a new player history (corresponding to the current situation)
       and adds it to the existing history list */	
    player_history new;

    new.values[PLAYER_HISTORY_SEASON] = season;
    new.values[PLAYER_HISTORY_WEEK] = week;
    new.values[PLAYER_HISTORY_SKILL] = (gint)rint(pl->cskill * 10);
    new.values[PLAYER_HISTORY_GOALS] = pl->goals;
    new.values[PLAYER_HISTORY_WAGE] = pl->wage;
    new.values[PLAYER_HISTORY_VALUE] = pl->value;
    
    if(pl->history->len == options[OPT_HISTORY_PLAYER_MAX])
	g_array_remove_index(pl->history, 0);

    g_array_append_val(pl->history, new);
}

/***************************** FINANCES *****************************************/

void reset_team_history(team *tm)
{
    if(tm->history != NULL)
	g_array_free(tm->history, TRUE);
    tm->history = g_array_new(FALSE, FALSE, sizeof(team_history));
}

void update_team_history(team *tm)
{
    gint bound[2];
    team_history new;

    get_league_bounds(get_league_from_id(tm->id), bound);

    new.values[TEAM_HISTORY_SEASON] = season;
    new.values[TEAM_HISTORY_WEEK] = week;
    new.values[TEAM_HISTORY_RANK] = bound[1] - bound[0] - rank[tm->id];
    new.values[TEAM_HISTORY_PTS] = tm->results[RES_PTS];
    new.values[TEAM_HISTORY_GD] = tm->results[RES_GF] - tm->results[RES_GA];
    new.values[TEAM_HISTORY_GF] = tm->results[RES_GF];
    new.values[TEAM_HISTORY_GA] = tm->results[RES_GA];
    new.values[TEAM_HISTORY_MONEY] = (tm->id == my_team) ? finances[FIN_MONEY] : 0;
    new.values[TEAM_HISTORY_AV_ATTENDANCE] = stadiums[tm->id].average_attendance;
	
    if(tm->history->len == options[OPT_HISTORY_TEAM_MAX])
	g_array_remove_index(tm->history, 0);

    g_array_append_val(tm->history, new);
}

/* write the history of a given index into the values array */
void
get_player_history_element(player pl, player_history *history, gint idx)
{
    gint i;

    for(i=0;i<PLAYER_HISTORY_END;i++)
	history->values[i] = 
	    g_array_index(pl.history, player_history, idx).values[i];
}

/* write the values of type 'type' of every history element into the array */
void
get_player_history_values(player pl, GArray *array, gint type)
{
    gint i;
    gint value;

    for(i=0;i<pl.history->len;i++)
    {	
	value = g_array_index(pl.history, player_history, i).values[type];
	g_array_append_val(array, value);
    }
}

/* write the history of a given index into the values array */
void
get_team_history_element(team tm, team_history *history, gint idx)
{
    gint i;

    for(i=0;i<TEAM_HISTORY_END;i++)
	history->values[i] = 
	    g_array_index(tm.history, team_history, idx).values[i];
}

/* write the values of type 'type' of every history element into the array */
void
get_team_history_values(team tm, GArray *array, gint type)
{
    gint i;
    gint value;

    for(i=0;i<tm.history->len;i++)
    {	
	value = g_array_index(tm.history, team_history, i).values[type];
	g_array_append_val(array, value);
    }
}
