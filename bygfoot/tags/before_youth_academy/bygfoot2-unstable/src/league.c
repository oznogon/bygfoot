#include "cup.h"
#include "free.h"
#include "league.h"
#include "main.h"
#include "maths.h"
#include "misc.h"
#include "option.h"
#include "player.h"
#include "stat.h"
#include "table.h"
#include "team.h"
#include "user.h"
#include "variables.h"

/**
   Create a new league with some default values.
   @return The league we created.
   @see League
*/
League
league_new(gboolean new_id)
{
    League new;

    new.name = g_string_new("");
    new.names_file = g_string_new(opt_str("string_opt_player_names_file"));
    new.sid = g_string_new("");
    new.short_name = g_string_new("");
    new.symbol = g_string_new("");
    
    new.id = (new_id) ? league_id_new : -1;
    new.layer = -1;

    new.average_skill = 7000;

    new.prom_rel.prom_games_dest_sid = g_string_new("");
    new.prom_rel.prom_games_loser_sid = g_string_new("");
    new.prom_rel.prom_games_cup_sid = g_string_new("");
    new.prom_rel.prom_games_number_of_advance = 1;

    new.prom_rel.elements = g_array_new(FALSE, FALSE, sizeof(PromRelElement));

    new.teams = g_array_new(FALSE, FALSE, sizeof(Team));
    
    new.fixtures = g_array_new(FALSE, FALSE, sizeof(Fixture));

    new.table = table_new();
    new.table.clid = new.id;

    new.first_week = new.week_gap = 1;
    new.round_robins = 2;
    new.yellow_red = 1000;

    new.stats = stat_league_new(new.id);

    return new;
}

/**
   Create a new PromRelElement with default values.
   @return The PromRelElement we created.
   @see PromRelElement
*/
PromRelElement
prom_rel_element_new(void)
{
    PromRelElement new;

    new.ranks[0] = new.ranks[1] = 0;
    new.dest_sid = g_string_new("");
    new.type = PROM_REL_NONE;

    return new;
}

/** Get the array index of the given league or cup id.
    @param clid The id of the league or cup.
    @return The index in the leagues or cups array. */
gint
league_cup_get_index_from_clid(gint clid)
{
    gint i;
    gint index = -1;

    if(clid < ID_CUP_START)
    {
	for(i=0;i<ligs->len;i++)
	    if(lig(i).id == clid)
	    {
		index = i;
		break;
	    }
    }
    else
	for(i=0;i<cps->len;i++)
	    if(cp(i).id == clid)
	    {
		index = i;
		break;
	    }

    if(index == -1)
    {
	g_warning("league_cup_get_index_from_clid: couldn't find league or cup with id %d\n", clid);
	main_exit_program(EXIT_POINTER_NOT_FOUND, NULL);
    }

    return index;
}

/** Return the league pointer belonging to the id.
    @param clid The id we look for.
    @return The league pointer or NULL if failed. */
League*
league_from_clid(gint clid)
{
    gint i;

    for(i=0;i<ligs->len;i++)
	if(lig(i).id == clid)
	    return &lig(i);

    g_warning("league_from_clid: didn't find league with id %d\n", clid);

    main_exit_program(EXIT_POINTER_NOT_FOUND, NULL);

    return NULL;
}

/** Return the id of the next league (or cup if we are
    at the end of the leagues array).
    @param clid The id of the current league or cup. */
gint
league_cup_get_next_clid(gint clid)
{
    gint i, return_value = -1;

    if(clid < ID_CUP_START)
    {
	for(i=0;i<ligs->len;i++)
	    if(lig(i).id == clid)
		break;

	if(i != ligs->len - 1)
	    return_value = lig(i + 1).id;
	else if(acps->len > 0)
	    return_value = acp(0)->id;
	else
	    return_value = lig(0).id;
    }
    else
    {
	for(i=0;i<acps->len;i++)
	    if(acp(i)->id == clid)
		break;	

	if(i != acps->len - 1)
	    return_value = acp(i + 1)->id;
	else
	    return_value = lig(0).id;
    }

    return return_value;
}


/** Return the id of the previous league or cup.
    @param clid The id of the current league or cup. */
gint
league_cup_get_previous_clid(gint clid)
{
    gint i, return_value = -1;

    if(clid < ID_CUP_START)
    {
	for(i=ligs->len - 1;i>=0;i--)
	    if(lig(i).id == clid)
		break;

	if(i != 0)
	    return_value = lig(i - 1).id;
	else if(acps->len > 0)
	    return_value = acp(acps->len - 1)->id;
	else
	    return_value = lig(ligs->len - 1).id;
    }
    else
    {
	for(i=acps->len - 1;i>=0;i--)
	    if(acp(i)->id == clid)
		break;

	if(i != 0)
	    return_value = acp(i - 1)->id;
	else
	    return_value = lig(ligs->len - 1).id;
    }

    return return_value;
}

/** Return the fixture coming after the specified week and round. 
    @param league The league the fixtures of which we examine. 
    @return A fixture pointer or NULL. */
Fixture*
league_cup_get_next_fixture(gint clid, gint week_number, gint week_round_number)
{
    gint i;
    GArray *fixtures = league_cup_get_fixtures(clid);

    for(i=0;i<fixtures->len;i++)
	if(g_array_index(fixtures, Fixture, i).week_number > week_number ||
	   (g_array_index(fixtures, Fixture, i).week_number == week_number &&
	    g_array_index(fixtures, Fixture, i).week_round_number >= week_round_number))
	    return &g_array_index(fixtures, Fixture, i);

    return NULL;
}

/** Return the fixture coming just before the specified week and round. 
    @param league The league the fixtures of which we examine. 
    @return A fixture pointer or NULL. */
Fixture*
league_cup_get_previous_fixture(gint clid, gint week_number, gint week_round_number)
{
    gint i;
    GArray *fixtures = league_cup_get_fixtures(clid);

    for(i=fixtures->len - 1;i>=0;i--)
	if(g_array_index(fixtures, Fixture, i).week_number < week_number ||
	   (g_array_index(fixtures, Fixture, i).week_number == week_number &&
	    g_array_index(fixtures, Fixture, i).week_round_number < week_round_number))
	    return &g_array_index(fixtures, Fixture, i);

    return NULL;
}


/** Return the average stadium capacity of cpu teams
    in the specified league or cup. */
gint
league_cup_average_capacity(gint clid)
{
    gint i, len;
    gfloat sum = 0;
    const GArray *teams = NULL;
    const GPtrArray *teamsp = NULL;

    if(clid < ID_CUP_START)
    {	
	teams = (GArray*)league_cup_get_teams(clid);
	len = teams->len;
	for(i=0;i<teams->len;i++)
	    if(team_is_user(&g_array_index(teams, Team, i)) == -1)
		sum += g_array_index(teams, Team, i).stadium.capacity;
    }
    else
    {
	teamsp = (GPtrArray*)league_cup_get_teams(clid);
	len = teamsp->len;
	for(i=0;i<teamsp->len;i++)
	    if(team_is_user((Team*)g_ptr_array_index(teamsp, i)) == -1)
		sum += ((Team*)g_ptr_array_index(teamsp, i))->stadium.capacity;
    }
	
    return sum / (gfloat)len;
}

/** Get the index of the league with the specified string id. */
gint
league_index_from_sid(const gchar *sid)
{
    gint i;

    for(i=0;i<ligs->len;i++)
	if(strcmp(lig(i).sid->str, sid) == 0)
	    return i;

    g_warning("league_index_from_sid: no index found for sid '%s'.\n", sid);

    main_exit_program(EXIT_INT_NOT_FOUND, NULL);

    return -1;
}

/** Remove the team with the specified id from the teams
    array without freeing the memory (used in promotion/relegation). */
void
league_remove_team_with_id(League *league, gint id)
{
    gint i;

    for(i=0;i<league->teams->len;i++)
	if(g_array_index(league->teams, Team, i).id == id)
	{
	    g_array_remove_index(league->teams, i);
	    return;
	}

    g_warning("league_remove_team_with_id: team with id %d in league %s not found\n",
	      id, league->name->str);
}


/** Add the teams to promote/relegate (from the prom_rel elements)
    from the league to the array. */
void
league_get_team_movements_prom_rel(const League *league, GArray *team_movements)
{
    gint i, j, k;
    TeamMove new_move;
    const GArray *elements = league->prom_rel.elements;
    GPtrArray *dest_sids = NULL;

    for(i=0;i<elements->len;i++)
    {
	dest_sids =
	    misc_separate_strings(g_array_index(elements, PromRelElement, i).dest_sid->str);
	gint order[dest_sids->len];

	k = 0;
	math_generate_permutation(order, 0, dest_sids->len - 1);

	for(j=g_array_index(elements, PromRelElement, i).ranks[0];
	    j<=g_array_index(elements, PromRelElement, i).ranks[1]; j++)
	{
	    new_move.tm = *(g_array_index(league->table.elements, TableElement, j - 1).team);
	    new_move.league_idx = 
		league_index_from_sid(((GString*)g_ptr_array_index(dest_sids, order[k++ % dest_sids->len]))->str);
	    new_move.prom_rel_type = g_array_index(elements, PromRelElement, i).type;

	    g_array_append_val(team_movements, new_move);

	    if(team_is_user(g_array_index(league->table.elements, TableElement, j - 1).team) != -1)
	    {
		if(g_array_index(elements, PromRelElement, i).type == PROM_REL_PROMOTION)
		    user_history_add(&usr(team_is_user(
					      g_array_index(league->table.elements, TableElement, j - 1).team)),
				     USER_HISTORY_PROMOTED, new_move.tm.id, lig(new_move.league_idx).id, -1, "");
		else
		    user_history_add(&usr(team_is_user(
					      g_array_index(league->table.elements, TableElement, j - 1).team)),
				     USER_HISTORY_RELEGATED, new_move.tm.id, lig(new_move.league_idx).id, -1, "");
	    }
	}
	
	free_g_string_array(&dest_sids);
    }
}


/** Add the team movements from the promotion games
    to the array. */
void
league_get_team_movements_prom_games(const League *league, GArray *team_movements,
				     const GPtrArray *prom_games_teams, gboolean up)
{
    gint i, k;
    TeamMove new_move;
    GPtrArray *dest_sids = (up) ?
	misc_separate_strings(league->prom_rel.prom_games_dest_sid->str) :
	misc_separate_strings(league->prom_rel.prom_games_loser_sid->str);
    gint order[dest_sids->len];
    gint start_idx = 0, 
	end_idx = league->prom_rel.prom_games_number_of_advance;
    gint prom_type = PROM_REL_PROMOTION,
	user_his_type = USER_HISTORY_PROMOTED;

    if(!up)
    {
	start_idx = league->prom_rel.prom_games_number_of_advance;
	end_idx = prom_games_teams->len;
	prom_type = PROM_REL_RELEGATION;
	user_his_type = USER_HISTORY_RELEGATED;
    }

    k = 0;
    math_generate_permutation(order, 0, dest_sids->len - 1);

    for(i=start_idx;i<end_idx;i++)
    {	    
	new_move.tm = *((Team*)g_ptr_array_index(prom_games_teams, i));
	new_move.league_idx = 
	    league_index_from_sid(((GString*)g_ptr_array_index(dest_sids, order[k++ % dest_sids->len]))->str);
	new_move.prom_rel_type = prom_type;
	g_array_append_val(team_movements, new_move);
	
	if(team_is_user((Team*)g_ptr_array_index(prom_games_teams, i)) != -1)
	    user_history_add(&usr(team_is_user((Team*)g_ptr_array_index(prom_games_teams, i))),
			     user_his_type, new_move.tm.id, lig(new_move.league_idx).id, -1, "");
    }
    
    free_g_string_array(&dest_sids);
}


/** Add the teams to promote/relegate from the league to the array
    and remove them from the leagues. */
void
league_get_team_movements(League *league, GArray *team_movements)
{
    GPtrArray *prom_games_teams = NULL;
    const Cup *prom_cup = NULL;

    league_get_team_movements_prom_rel(league, team_movements);

    if(query_league_has_prom_games(league))
    {
	prom_cup = cup_from_sid(league->prom_rel.prom_games_cup_sid->str);
    
	if(prom_cup == NULL)
	{
	    g_warning("league_get_team_movements: promotion games cup not found for league %s (cup sid %s).\n",
		      league->name->str, league->prom_rel.prom_games_cup_sid->str);
	    return;
	}

	prom_games_teams = cup_get_teams_sorted(prom_cup);

	league_get_team_movements_prom_games(league, team_movements, prom_games_teams, TRUE);

	if(strlen(league->prom_rel.prom_games_loser_sid->str) > 0)
	    league_get_team_movements_prom_games(league, team_movements, 
						 prom_games_teams, FALSE);
	
	g_ptr_array_free(prom_games_teams, TRUE);
    }

    g_array_sort_with_data(league->teams, team_compare_func,
			   GINT_TO_POINTER(TEAM_COMPARE_LEAGUE_RANK + 100));
}


/** Nullify league stuff at the beginning of a season. */
void
league_season_start(League *league)
{
    gint i, j;
    gint idx = league_index_from_sid(league->sid->str);
    gboolean user_champ = 
	(team_is_user(
	    team_of_id(g_array_index(lig(0).table.elements, TableElement, 0).team_id)) != -1);
    gfloat team_change_factor = 0;

    for(i=0;i<league->table.elements->len;i++)
    {
	g_array_index(league->table.elements, TableElement, i).team = 
	    &g_array_index(league->teams, Team, i);
	g_array_index(league->table.elements, TableElement, i).team_id = 
	    g_array_index(league->teams, Team, i).id;
	g_array_index(league->table.elements, TableElement, i).old_rank = i;

	for(j=0;j<TABLE_END;j++)
	    g_array_index(league->table.elements, TableElement, i).values[j] = 0;
    }

    for(i=0;i<league->teams->len;i++)
    {
	team_change_factor = 
	    (team_is_user(&g_array_index(league->teams, Team, i)) == -1) *
	    math_rnd(const_float("float_season_end_team_change_lower") +
		     (user_champ && idx == 0) * const_float("float_season_end_user_champ_addition"),
		     const_float("float_season_end_team_change_upper") +
		     (user_champ && idx == 0) * const_float("float_season_end_user_champ_addition"));

	for(j=0;j<g_array_index(league->teams, Team, i).players->len;j++)
	    player_season_start(
		&g_array_index(g_array_index(league->teams, Team, i).players, Player, j), team_change_factor);

	if(team_is_user(&g_array_index(league->teams, Team, i)) == -1)
	    team_update_cpu_structure(&g_array_index(league->teams, Team, i));

	g_array_index(league->teams, Team, i).stadium.average_attendance =
	    g_array_index(league->teams, Team, i).stadium.games = 
	    g_array_index(league->teams, Team, i).stadium.possible_attendance = 0;
    }
}

/** Find out whether the team with specified rank in the league
    would participate in promotion games were the season to end. */
gboolean
query_league_rank_in_prom_games(const League *league, gint rank)
{
    gint i, j, k;
    const Cup *cup = NULL;
    const CupRound *cup_round = NULL;

    for(i=0;i<ligs->len;i++)
	if(query_league_has_prom_games((&lig(i))))
	{
	    cup = cup_from_sid(lig(i).prom_rel.prom_games_cup_sid->str);
	    for(k=0;k<cup->rounds->len;k++)
	    {
		cup_round = &g_array_index(cup->rounds, CupRound, k);
		for(j=0;j<cup_round->choose_teams->len;j++)
		{
		    if(strcmp(g_array_index(cup_round->choose_teams, CupChooseTeam, j).sid->str,
			      league->sid->str) == 0 &&
		       ((rank >= g_array_index(cup_round->choose_teams,
					       CupChooseTeam, j).start_idx &&
			 rank <= g_array_index(cup_round->choose_teams, 
					       CupChooseTeam, j).end_idx && 
			 g_array_index(cup_round->choose_teams, 
				       CupChooseTeam, j).randomly) ||
			(rank >= g_array_index(cup_round->choose_teams, 
					       CupChooseTeam, j).start_idx &&
			 rank < g_array_index(cup_round->choose_teams, 
					      CupChooseTeam, j).start_idx + 
			 g_array_index(cup_round->choose_teams, 
				       CupChooseTeam, j).number_of_teams &&
			 !g_array_index(cup_round->choose_teams, 
					CupChooseTeam, j).randomly)))
			return TRUE;
		}
	    }
	}

    return FALSE;
}

/** Find out whether there are/were league matches. */
gboolean
query_league_matches_in_week(const League *league, gint week_number)
{
    gint i;

    for(i=0;i<league->fixtures->len;i++)
	if(g_array_index(league->fixtures, Fixture, i).week_number == week_number)
	    return TRUE;

    return FALSE;
}
