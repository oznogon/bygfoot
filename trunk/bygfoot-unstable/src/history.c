/* #include "history.h" */
#include "bygfoot.h"
#include <math.h>

/*********************************** PLAYER *************************************/

void reset_player_history(player * pl)
{
	reset_player_history_list(&pl->history);
}

void print_player_history(player * pl)
{
	print_player_history_list(&pl->history);
}

void reset_player_history_list(struct player_history_list * plist)
{
	plist->size = 0;
	if (plist->list != NULL)
		free(plist->list);
	plist->list = NULL;
}

void update_player_history(player * pl)
{
	/* Creates a new player history (corresponding to the current situation)
	   and adds it to the existing history list */
	
	struct player_history now;
	now.week = week;
	now.season = season;
	now.fitness = (gint)(100.0*pl->fitness);   
	now.cskill = (gint)(10.0*pl->cskill);
	now.wage = pl->wage;
	
	/* not necessary, but it is more secure */         
	if (pl->history.list == NULL)
		reset_player_history(pl);
		
	pl->history.size++;
	pl->history.list = realloc(pl->history.list, pl->history.size * sizeof(struct player_history));
	/* The history list size is extended each week. It can be a little slow. 
	
	   There are several possibilities : 
	   0 : extend the history size each week (current solution)
	   1 : extend the history size regularly (e.g. every 10 weeks) 
	      - week 1 : extend the size and update
	      - week 2 to 10 : update
	      - week 11 : extend the size and update...
	   2 : assing a finite size for history (e.g. 100). After 100 weeks, 
	      the history [1] becomes [0], etc.
	*/
	
	pl->history.list[pl->history.size - 1] = now;
}

void print_player_history_list(struct player_history_list * plist)
{
	gint i;
	printf("season\tweek\tfitness\tcskill\twage\n");
	for (i = 0; i < plist->size; i++)
	{
		printf("%d\t", plist->list[i].season);
		printf("%d\t", plist->list[i].week);
		printf("%.2f\t", plist->list[i].fitness / 100.0);
		printf("%.1f\t", plist->list[i].cskill / 10.0);
		printf("%d\n", plist->list[i].wage);
	}
} 	
			
/***************************** FINANCES *****************************************/

void reset_finances_history(void)
{
	reset_finances_history_list(&financial_history);
}

void update_finances_history(void)
{
	update_finances_history_list(&financial_history);
}

void print_finances_history(void)
{
	print_finances_history_list(&financial_history);
}

void reset_finances_history_list(struct finances_history_list * flist)
{
	flist->size = 0;
	if (flist->list != NULL)
		free(flist->list);
	flist->list = NULL;
}

void update_finances_history_list(struct finances_history_list * flist)
{
	struct finances_history now;
	now.week = week;
	now.season = season;
	now.money = finances[FIN_MONEY];
	
	if (flist == NULL)
		reset_finances_history_list(flist);
	
	flist->size++;
	flist->list = realloc(flist->list, flist->size * sizeof(struct finances_history));
	flist->list[flist->size-1] = now;
}

void print_finances_history_list(struct finances_history_list * flist)
{
	gint i;
	printf("season\tweek\tmoney\n");
	for (i = 0; i < flist->size; i++)
	{
		printf("%d\t", flist->list[i].season);
		printf("%d\t", flist->list[i].week);
		printf("%d\n", flist->list[i].money);
	}
} 	

/* write a player's history for attribute 'type'
   into the garray which will be used for drawing the graph then */
void
get_history_player(gint team_id, gint player_number, 
		   gint *veclen, GArray *array, gint type)
{
    gint i;
    gfloat value;
    struct player_history_list *history =
	&(teams[team_id].players[player_number].history);

    *veclen = history->size;

    for(i=0;i<*veclen;i++)
    {
	if(type == HISTORY_SKILL)
	    value = (gfloat)(history->list[i].cskill) / 10;
	else if(type == HISTORY_WAGE)
	    value = (gfloat)(history->list[i].wage);

	g_array_append_val(array, value);
    }
}
