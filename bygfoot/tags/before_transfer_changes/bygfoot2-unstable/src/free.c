#include "free.h"
#include "transfer_struct.h"
#include "user.h"
#include "variables.h"

/**
   Free all memory allocated by the program.
   This mainly means we have to free a lot of GStrings
   and GArrays. */
void
free_memory(void)
{
    free_variables();
    free_transfer_list();
    free_country(FALSE);
    free_users(FALSE);
    free_live_game(&live_game_temp);
    free_support_dirs();
}

/** Free the transfer list. */
void
free_transfer_list(void)
{
    gint i;

    for(i=0;i<transfer_list->len;i++)
	g_array_free(g_array_index(transfer_list, Transfer, i).offers, TRUE);
    
    free_g_array(&transfer_list);
}

/** Free the memory occupied by the season stats. */
void
free_season_stats(gboolean reset)
{
    gint i, j;

    if(season_stats == NULL)
    {
	if(reset)
	    season_stats = g_array_new(FALSE, FALSE, sizeof(SeasonStat));
	return;
    }

    for(i=0;i<season_stats->len;i++)
    {
	for(j=0;j<g_array_index(season_stats, SeasonStat, i).league_stats->len;j++)
	    free_league_stats(
		&g_array_index(g_array_index(season_stats, SeasonStat, i).league_stats,
			       LeagueStat, j));

	for(j=0;j<g_array_index(season_stats, SeasonStat, i).league_champs->len;j++)
	{
	    g_string_free(
		g_array_index(
		    g_array_index(season_stats, SeasonStat, i).league_champs, ChampStat, j).cl_name, TRUE);
	    g_string_free(
		g_array_index(
		    g_array_index(season_stats, SeasonStat, i).league_champs, ChampStat, j).team_name, TRUE);
	}

	for(j=0;j<g_array_index(season_stats, SeasonStat, i).cup_champs->len;j++)
	{
	    g_string_free(
		g_array_index(
		    g_array_index(season_stats, SeasonStat, i).cup_champs, ChampStat, j).cl_name, TRUE);
	    g_string_free(
		g_array_index(
		    g_array_index(season_stats, SeasonStat, i).cup_champs, ChampStat, j).team_name, TRUE);
	}

	g_array_free(g_array_index(season_stats, SeasonStat, i).cup_champs, TRUE);
	g_array_free(g_array_index(season_stats, SeasonStat, i).league_champs, TRUE);
    }

    free_g_array(&season_stats);

    if(reset)
	season_stats = g_array_new(FALSE, FALSE, sizeof(SeasonStat));
}

/** Free the users array. */
void
free_users(gboolean reset)
{
    gint i;

    if(users == NULL)
    {
	if(reset)
	    users = g_array_new(FALSE, FALSE, sizeof(User));
	return;
    }

    for(i=0;i<users->len;i++)
	free_user(&usr(i));

    free_g_array(&users);

    if(reset)
	users = g_array_new(FALSE, FALSE, sizeof(User));
}

/** Free the memory the user occupies.
    @param user The user we free. */
void
free_user(User *user)
{
    gint i;

    free_g_string(&user->name);
    free_live_game(&user->live_game);
    free_option_list(&user->options, FALSE);

    for(i=0;i<user->events->len;i++)
	free_event(&g_array_index(user->events, Event, i));

    free_g_array(&user->events);
    
    for(i=0;i<user->history->len;i++)
	g_string_free(g_array_index(user->history,
				    UserHistory, i).value_string, TRUE);
    free_g_array(&user->history);
}

/** Free a user event. */
void
free_event(Event *event)
{
    free_g_string(&event->value_string);
}

/** Free an array of options.
    @param array The array we free.
    @param reset Whether to create the array anew (empty).
    @see #Option */
void
free_option_list(OptionList *optionlist, gboolean reset)
{
    gint i;

    if(optionlist->list == NULL)
    {
	if(reset)
	{
	    optionlist->list = g_array_new(FALSE, FALSE, sizeof(Option));
	    g_datalist_init(&optionlist->datalist);
	}

	return;
    }
    
    for(i=0;i<optionlist->list->len;i++)
    {
	free_g_string(&g_array_index(optionlist->list, Option, i).name);
	free_g_string(&g_array_index(optionlist->list, Option, i).string_value);
    }

    free_g_array(&optionlist->list);

    if(reset)
    {
	optionlist->list = g_array_new(FALSE, FALSE, sizeof(Option));
	g_datalist_init(&optionlist->datalist);
    }
}

/** Free a live game variable. */
void
free_live_game(LiveGame *match)
{
    gint i, j, k;

    if(match->units == NULL)
	return;

    for(i=0;i<match->units->len;i++)
	free_g_string(&g_array_index(match->units, LiveGameUnit, i).event.commentary);

    for(i=0;i<2;i++)
    {
	for(j=0;j<LIVE_GAME_STAT_ARRAY_END;j++)
	{
	    for(k=0;k<match->stats.players[i][j]->len;k++)
		free_g_string((GString**)&g_ptr_array_index(match->stats.players[i][j], k));
	
	    free_g_ptr_array(&match->stats.players[i][j]);
	}
    }
    
    free_g_array(&match->units);
}


/**
   Free the main variable of the game, #country.
*/
void
free_country(gboolean reset)
{
    gint i;
    GString **strings[3] = 
	{&country.name, &country.symbol, &country.sid};

    for(i=0;i<3;i++)
    {
	free_g_string(strings[i]);
	if(reset)
	    *strings[i] = g_string_new("");
    }

    free_leagues_array(&ligs, reset);
    free_cups_array(&cps, reset);
    free_g_ptr_array(&acps);

    if(reset)
	acps = g_ptr_array_new();
}

/**
   Free the memory occupied by a leagues array.
   @param leagues The pointer to the array we free.
*/
void
free_leagues_array(GArray **leagues, gboolean reset)
{
    gint i;

    if(*leagues == NULL)
    {
	if(reset)
	    *leagues = g_array_new(FALSE, FALSE, sizeof(League));
	return;
    }

    for(i=0;i<(*leagues)->len;i++)
	free_league(&g_array_index(*leagues, League, i));

    free_g_array(leagues);

    if(reset)
	*leagues = g_array_new(FALSE, FALSE, sizeof(League));
}

/**
   Free the memory occupied by a league.
   @param league The pointer to the league we free.
*/
void
free_league(League *league)
{
    gint i;
    GString **strings[6] = 
	{&league->name,
	 &league->short_name,
	 &league->symbol,
	 &league->sid,
	 &league->prom_rel.prom_games_dest_sid,
	 &league->prom_rel.prom_games_cup_sid};
    GArray **arrays[2] = 
	{&league->teams,
	 &league->prom_rel.elements};

    for(i=0;i<6;i++)
	free_g_string(strings[i]);

    if(league->teams != NULL)
	free_teams_array(&league->teams, FALSE);

    for(i=0;i<2;i++)
	free_g_array(arrays[i]);

    free_table(&league->table);

    free_g_array(&league->fixtures);

    free_league_stats(&league->stats);
}

/** Free the league stats. */
void
free_league_stats(LeagueStat *stats)
{
    gint i;

    for(i=0;i<stats->teams_off->len;i++)
	g_string_free(g_array_index(stats->teams_off, Stat, i).value_string, TRUE);
    free_g_array(&stats->teams_off);

    for(i=0;i<stats->teams_def->len;i++)
	g_string_free(g_array_index(stats->teams_def, Stat, i).value_string, TRUE);
    free_g_array(&stats->teams_def);

    for(i=0;i<stats->player_scorers->len;i++)
	g_string_free(g_array_index(stats->player_scorers, Stat, i).value_string, TRUE);
    free_g_array(&stats->player_scorers);

    for(i=0;i<stats->player_goalies->len;i++)
	g_string_free(g_array_index(stats->player_goalies, Stat, i).value_string, TRUE);
    free_g_array(&stats->player_goalies);
}


/** Free a table. */
void
free_table(Table *table)
{
    free_g_string(&table->name);

    free_g_array(&table->elements);
}

/** Free the memory occupied by a teams array.
    @param teams The pointer to the array we free. */
void
free_teams_array(GArray **teams, gboolean reset)
{
    gint i;

    if(*teams == NULL)
    {
	if(reset)
	    *teams = g_array_new(FALSE, FALSE, sizeof(Team));
	return;
    }

    for(i=0;i<(*teams)->len;i++)
	free_team(&g_array_index(*teams, Team, i));

    free_g_array(teams);

    if(reset)
	*teams = g_array_new(FALSE, FALSE, sizeof(Team));
}

/**
   Free the memory occupied by a team.
   @param tm The pointer to the team we free.
*/
void
free_team(Team *tm)
{
    gint i;

    free_g_string(&tm->name);
    free_g_string(&tm->symbol);

    if(tm->players != NULL)
    {
	for(i=0;i<tm->players->len;i++)
	    free_player(&g_array_index(tm->players, Player, i));

	free_g_array(&tm->players);
    }
}

/** Free the memory occupied by a player.
    @param pl The pointer to the player we free. */
void
free_player(Player *pl)
{
    free_g_string(&pl->name);

    free_g_array(&pl->cards);
}

/**
   Free the memory occupied by a cups array.
   @param cups The pointer to the array we free.
*/
void
free_cups_array(GArray **cups, gboolean reset)
{
    gint i;

    if(*cups == NULL)
    {
	if(reset)
	    *cups = g_array_new(FALSE, FALSE, sizeof(Cup));
	return;
    }

    for(i=0;i<(*cups)->len;i++)
	free_cup(&g_array_index(*cups, Cup, i));

    free_g_array(cups);

    if(reset)
	*cups = g_array_new(FALSE, FALSE, sizeof(Cup));
}

/**
   Free the memory occupied by a cup.
   @param cup The pointer to the cup we free.
*/
void
free_cup(Cup *cup)
{
    gint i, j;
    GString **strings[4] =
	{&cup->name,
	 &cup->short_name,
	 &cup->symbol,
	 &cup->sid};

    for(i=0;i<4;i++)
	free_g_string(strings[i]);

    for(i=0;i<cup->rounds->len;i++)
	if(g_array_index(cup->rounds, CupRound, i).round_robin_number_of_groups > 0)
	{
	    for(j=0;j<g_array_index(cup->rounds, CupRound, i).tables->len;j++)
		free_table(&g_array_index(g_array_index(cup->rounds, CupRound, i).tables, Table, j));

	    for(j=0;j<g_array_index(cup->rounds, CupRound, i).choose_teams->len;j++)
		free_cup_choose_team(
		    &g_array_index(g_array_index(cup->rounds, CupRound, i).choose_teams, CupChooseTeam, j));
	    
	    free_g_array(&g_array_index(cup->rounds, CupRound, i).choose_teams);
	    free_teams_array(&g_array_index(cup->rounds, CupRound, i).teams, FALSE);
	}

    free_g_array(&cup->rounds);
    free_g_array(&cup->fixtures);

    free_g_ptr_array(&cup->bye);
    free_g_ptr_array(&cup->team_names);
    free_g_ptr_array(&cup->teams);
    free_g_string_array(&cup->properties);
}

/**
   Free the memory occupied by a CupChooseTeam.
   @param cup_choose_team The pointer to the team we free.
*/
void
free_cup_choose_team(CupChooseTeam *cup_choose_team)
{
    free_g_string(&cup_choose_team->sid);
}

/** Free some global variables (except for the country variable). */
void
free_variables(void)
{
    free_g_string_array(&player_names);

    free_option_list(&options, FALSE);
    free_option_list(&constants, FALSE);
    free_option_list(&constants_app, FALSE);

    free_g_string(&save_file);

    g_rand_free(rand_generator);

    free_season_stats(FALSE);
}

/**
   Free a GPtrArray containing GStrings.
   @param array The array to be freed.
*/
void
free_g_string_array(GPtrArray **array)
{
    gint i;

    if(*array == NULL)
	return;

    for(i=0;i<(*array)->len;i++)
	free_g_string((GString**)&g_ptr_array_index(*array, i));

    free_g_ptr_array(array);
}

/** Free a GString if it isn't NULL.
    @param string The string to be freed. */
void
free_g_string(GString **string)
{
    if(*string == NULL)
	return;

    g_string_free(*string, TRUE);

    *string = NULL;
}

/** Free a GArray if it isn't NULL.
    @param array The array to be freed. */
void
free_g_array(GArray **array)
{
    if(*array == NULL)
	return;

    g_array_free(*array, TRUE);

    *array = NULL;
}

/** Free a GPtrArray if it isn't NULL.
    @param array The array to be freed. */
void
free_g_ptr_array(GPtrArray **array)
{
    if(*array == NULL)
	return;

    g_ptr_array_free(*array, TRUE);

    *array = NULL;
}

/** Free the glist containing the support directories. */
void
free_support_dirs(void)
{
  GList *elem = support_directories;

  if(elem == NULL)
      return;

  while(elem)
  {
      g_free ((gchar*)elem->data);
      elem = elem->next;
  }

  g_list_free(support_directories);
  support_directories = NULL;
}
