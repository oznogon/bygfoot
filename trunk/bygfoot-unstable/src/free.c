#include "free.h"

/* free everything we allocated */
void
free_memory(void)
{
    free_history();
    
    if(save_file != NULL)
	g_string_free(save_file, TRUE);

    free_histories();
}

/* free the history pointers */
void
free_history(void)
{
    season_stat *stat = history;
    season_stat *stat2;

    if(history == NULL)
	return;

    stat2 = stat->next;
    while(stat2 != NULL)
    {
	g_free(stat);
	stat = stat2;
	stat2 = stat->next;
    }
    
    g_free(stat);

    history = NULL;
}

/* free the team and player histories */
void
free_histories(void)
{
    gint i, j;
    
    for(i=0;i<175;i++)
	if(i != 114 && i != 130)
	{
	    if(teams[i].history != NULL)
	    {
		g_array_free(teams[i].history, TRUE);
		teams[i].history = NULL;
	    }
	    for(j=0;j<20;j++)
		if(teams[i].players[j].history != NULL)
		{
		    g_array_free(teams[i].players[j].history, TRUE);
		    teams[i].players[j].history = NULL;
		}
	}
}
