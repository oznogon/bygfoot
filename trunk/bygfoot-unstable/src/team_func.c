/**************************************************
 * Functions working with teams                   *
 **************************************************/

#include "defs.h"
#include "team_func.h"

/* try to set each of the first 11 players on his
   favoured position and sort the substitutes by position */
void
rearrange_team(void)
{
    gint i, j;
    gfloat old_average_cskill = 
	average_skill(my_team, 11, TRUE);

    /* reaarrange field players */
    for(i=0;i<11;i++)
    {
	if(teams[my_team].players[i].pos !=
	   teams[my_team].players[i].cpos)
	{
	    for(j=i+1;j<11;j++)
		if(teams[my_team].players[j].pos ==
		   teams[my_team].players[i].cpos &&
		   teams[my_team].players[j].cskill > 0)
		{
		    swap_players(my_team, i, my_team, j);
		    break;
		}
	}
    }
    
    /* sort substitutes */
    i = 11;
    while(i != 20)
    {
	for(j=i+1;j<20;j++)
	    if(teams[my_team].players[i].pos >= 0 &&
	       teams[my_team].players[j].pos >= 0 &&
	       teams[my_team].players[i].pos >
	       teams[my_team].players[j].pos)
	    {
		swap_players(my_team, i, my_team, j);
		i = 10;
		break;
	    }
	i++;
    }

    if(fabs(average_skill(my_team, 11, TRUE) -
	    old_average_cskill) > 0.05)
	bookmaker_re_tip();    
}

/* return the structure that fits the positions of
   the first 11 players */
gint
find_appropriate_structure(void)
{
    gint i;
    gint structure = 0;

    for(i=1;i<11;i++)
    {
	if(teams[my_team].players[i].pos > 0)
	    structure += 
		(gint)powf(10, 3 - (gfloat)teams[my_team].players[i].pos);
    }
    
    return structure;
}

/* finance and other settings for a new human player team.
   remove some players from the team. the human player's
   team has initially 17 players in the team (so that he
   has some room for new players), the cpu-teams have 20 */    
void
set_up_my_team(void)
{
    gint i, j;
    
    /* the human player's scout and physio are average */
    scout = physio = 3;
    /* playing style: balanced */
    teams[my_team].style = 0;
    
    for(i=1;i<FIN_END;i++)
	finances[i] = 0;
    for(i=0;i<COUNT_END;i++)
	counters[i] = 0;

    counters[COUNT_LOAN] = counters[COUNT_POSITIVE] = -2;

    for(i=0;i<50;i++)
    {
	goals[0][i].minute = goals[1][i].minute = -1;
	
	if(i < 12)
	{
	    for(j=0;j<2;j++)
		injuries[j][i] = booked[j][i] = -1;
	}

	if(i < 2)
	{
	    stadium_facts[i % 2][i % 2 + 1] = 0;
	    stadium_facts[i % 2][i % 2] = 0;
	}
    }

    for(i=2;i>=0;i--)
	remove_player(my_team, 12 + i * 3);

    rearrange_team();
    
    /* set up team finances */
    finances[FIN_MONEY] = 
	round_integer((gint)rint( rnd(0.7, 0.95) * 
				  (gfloat)stadiums[my_team].capacity
				  * 113), 2);

    stadiums[my_team].average_attendance = 0;
}

void
get_league_bounds(gint league, gint *bound)
{
    if(league == 1)
    {
	bound[0] = 0;
	bound[1] = 20;
    }
    else if(league < 5)
    {
	bound[0] = 20 + (league - 2) * 24;
	bound[1] = bound[0] + 24;
    }
    else if(league == 5)
    {
	bound[0] = 92;
	bound[1] = 114;
    }    
    else if(league == 6)
    {
	bound[0] = 115;
	bound[1] = 130;
    }    
    else if(league == 7)
    {
	bound[0] = 131;
	bound[1] = 145;
    }    
    else if(league == 8)
    {
	bound[0] = 145;
	bound[1] = 175;
    }
}

/* return the number of teams in league */
gint
teams_in_league(gint league)
{
    if(league == 1)
	return 20;
    else if(league < 5)
	return 24;

    return 22;
}

/* return the league (0 to 8) of a team based on its id */
gint
get_league_from_id(gint team_id)
{
    if(team_id < 20)
	return 1;
    else if(team_id < 44)
	return 2;
    else if(team_id < 68)
	return 3;
    else if(team_id < 92)
	return 4;
    else if(team_id < 114)
	return 5;
    else if(team_id < 130)
	return 6;
    else if(team_id < 146)
	return 7;

    return 8;
}

/* return the name of the league the team plays in */
void
get_league_name_from_id(gint team_id, gchar *name)
{
    gint league = (team_id >= 0) ? 
	get_league_from_id(team_id) : team_id * -1;

    strcpy(name, league_names[LEAGUE_NAMES_LEAGUE1 + (league - 1)]);
}

/* return random playing style from -2 to 2 */
gint
assign_playing_style(void)
{
    gfloat Random;

    Random = rnd(0, 1);

    /* all out defend */
    if(Random < 0.1)
	return -2;
    /* defend */
    else if(Random < 0.25)
	return -1;
    /* balanced */
    else if(Random < 0.75)
	return 0;
    /* attack */
    else if(Random < 0.9)
	return 1;
    /* all out attack */
    else
	return 2;
}

/* return a playing structure like 442 or 433 */
gint
assign_playing_structure(void)
{
    gfloat rndom = rnd(0, 1);
    
    if(rndom < 0.35)
	return 442;
    else if(rndom < 0.55)
	return 352;
    else if(rndom < 0.7)
	return 433;
    else if(rndom < 0.85)
	return 343;

    return 532;
}

/* write a new name from 'filename'
   to 'dest' */
void
get_name(gchar *dest, gchar *filename, gint max)
{
    gint rndom = rndi(0, max);
    gchar player_names[max + 10][50];
    
    get_names(filename, player_names);
    
    strcpy(dest, player_names[rndom]);
}

/* compare the one or two matches the teams
   have played against each other */
gint
direct_comparison(team_id1, team_id2)
{
    gint i = 0;
    gint result[2][2] = {{-1, -1}, {-1, -1}};

    while(fixtures[i].type > 0)
    {
	if(fixtures[i].team_id[0] == team_id1 &&
	   fixtures[i].team_id[1] == team_id2 && 
	   fixtures[i].type < 7000)
	{
	    result[0][0] = fixtures[i].result[0][0];
	    result[0][1] = fixtures[i].result[1][0];
	}
	else if(fixtures[i].team_id[0] == team_id2 &&
		fixtures[i].team_id[1] == team_id1 && 
		fixtures[i].type < 7000)
	{
	    result[1][0] = fixtures[i].result[0][0];
	    result[1][1] = fixtures[i].result[1][0];
	}

	i++;
    }

    if(result[0][0] + result[1][1] >
       result[0][1] + result[1][0])
	return 1;

    if(result[0][0] + result[1][1] ==
       result[0][1] + result[1][0] &&
       result[1][1] > result[0][1])
	return 1;
    
    return 0;
}

/* compare two teams regarding points etc. */
gint
is_better(const void *value1, const void *value2)
{
    gint *team_id1 = (gint*)value1;
    gint *team_id2 = (gint*)value2;

    if(teams[*team_id1].results[RES_PTS] > 
       teams[*team_id2].results[RES_PTS])
	return -1;

    if(teams[*team_id1].results[RES_PTS] == 
       teams[*team_id2].results[RES_PTS] &&
       teams[*team_id1].results[RES_GF] -
       teams[*team_id1].results[RES_GA] >
       teams[*team_id2].results[RES_GF] - 
       teams[*team_id2].results[RES_GA])
	return -1;

    if(teams[*team_id1].results[RES_PTS] == 
       teams[*team_id2].results[RES_PTS] &&
       teams[*team_id1].results[RES_GF] - 
       teams[*team_id1].results[RES_GA] ==
       teams[*team_id2].results[RES_GF] - 
       teams[*team_id2].results[RES_GA] &&
       teams[*team_id1].results[RES_GF] > 
       teams[*team_id2].results[RES_GF])
	return -1;
    
    /* compare the one or two matches the teams
       have played against each other.
       we take 'teams[*team_id1].id' instead of
       simply '*team_id1' because in the fixtures,
       the english participant in the CL is always
       entered with an id lower than a CL id */
    if(teams[*team_id1].results[RES_PTS] == 
       teams[*team_id2].results[RES_PTS] &&
       teams[*team_id1].results[RES_GF] - 
       teams[*team_id1].results[RES_GA] ==
       teams[*team_id2].results[RES_GF] - 
       teams[*team_id2].results[RES_GA] &&
       teams[*team_id1].results[RES_GF] == 
       teams[*team_id2].results[RES_GF] &&
       direct_comparison(teams[*team_id1].id,
			 teams[*team_id2].id) == 1)
	return -1;
    
    return 1;
}

/* sort the teams by points and goals and write
   the result into the global ranks array.
   i know that the sorting technique is horrible,
   but we don't have to work with millions of comparisons
   here. and i'm no programmer, after all. */
void
sort_teams(gint min, gint max)
{
    gint i;
    GArray *temporary_array =
	g_array_new(FALSE, FALSE, sizeof(gint));

    for(i = min; i <= max; i++)
	g_array_append_val(temporary_array, i);

    g_array_sort(temporary_array, is_better);

    for(i = min;i <= max; i++)
    {
	rank_ids[i] = 
	    g_array_index(temporary_array, gint, i - min);
	rank[g_array_index(temporary_array, gint, i - min)] =
	    i - min + 1;
    }
}

/* return the id of the team ranked 'rank' in 'league' */
gint
league_rank_id(gint league, gint rank)
{
    gint i;

    if(league == 1)
	return rank_ids[rank - 1];
    else if(league < 5)
	for(i=0;i<3;i++)
	    if(league == i + 2)
		return rank_ids[rank + 20 + i * 24 - 1];
    return rank_ids[91 + rank];
}

/* update the player values of a team: make them
   older, better/worse etc. */
void
update_teams_players(gint team_id)
{
    gint i;
    /* if the team had no game, all players recover */
    gint start = (my_team_played(week)) ?
	11 : 0;
    /* cpu-teams have an average physio */
    gint local_physio = (team_id == my_team) ? physio : 3;
    gfloat rndom;

    /* first number: no healing
       second: recovery time diminished by 1 week
       third: two weeks */
    gfloat heal_probs[3] = {(gfloat)local_physio / 20,
			    0.75 + (gfloat)local_physio / 20, 1};

    for(i=0;i<20;i++)
    {
	if(teams[team_id].players[i].pos >= 0)
	{
	    /* increase games and goals in european teams;
	       otherwise they'd only participate in a couple of games
	       per season */
	    if(team_id > 113 && team_id != my_team)
	    {
		if(rnd(0, 1) < 0.5)
		    teams[team_id].players[i].games++;

		rndom = rnd(0, 1);
		if( (teams[team_id].players[i].pos == 0 &&
		     rndom < 0.8) ||
		    rndom < 0.1 + 
		    (gfloat)teams[team_id].players[i].pos / 10)
		    teams[team_id].players[i].goals++;
	    }

	    /* injured players get better */
	    if(teams[team_id].players[i].health > 0)
	    {
		rndom = rnd(0,1);
		
		if(rndom > heal_probs[0])
		{
		    teams[team_id].players[i].health--;		    
		    if(teams[team_id].players[i].health % 100 > 0 &&
		       rndom > heal_probs[1])
			teams[team_id].players[i].health--;
		}

		/* recovered */
		if(teams[team_id].players[i].health % 100 == 0)
		{
		    teams[team_id].players[i].health = 0;
		    teams[team_id].players[i].cskill = 
			calculate_cskill(teams[team_id].
					 players[i]);
		    if(team_id != my_team)
			teams[team_id].players[i].fitness =
			    gauss_dist(0.7, 0.85, 0.99, 0.99);
		    else
			teams[team_id].players[i].fitness =
			    gauss_dist(0.5, 0.55, 0.8, 0.8);
		}
	    }
	    /* fitness of players that played gets reduced,
	       that of the others increased;
	       in the human player's team players get older and
	       better or worse */
	    else if(team_id == my_team)
	    {
		if(i >= start)
		    teams[team_id].players[i].fitness = 
			calculate_new_fitness(
			    teams[team_id].players[i], 1);
		else if(!debug)
		    teams[team_id].players[i].fitness =
			calculate_new_fitness(teams[team_id].
					      players[i], 0);

		teams[team_id].players[i].age += 0.02;
		
		update_skill(team_id, i);		
	    }
	    
	    if(teams[team_id].players[i].booked % 10 > 0)
	    {
		teams[team_id].players[i].booked--;

		if(teams[team_id].players[i].booked % 10 == 0)
		    teams[team_id].players[i].cskill = 
			calculate_cskill(teams[team_id].
					 players[i]);
	    }
	}
    }
}

/* replace injured players by healthy substitutes in the
   cpu-teams */
void
update_teams_injuries(gint team_id)
{
    gint i;
    gint substitute;
    
    for(i=0;i<11;i++)
    {
	if(teams[team_id].players[i].cskill == 0)
	{
	    substitute =
		find_substitute(team_id,
				teams[team_id].players[i].pos);
	    /* if there's no substitute, 'repair' the player */
	    if(substitute == -1)
	    {		
		teams[team_id].players[i].health =
		    teams[team_id].players[i].booked = 0;
		teams[team_id].players[i].fitness =
		    gauss_dist(0.7, 0.75, 0.99, 0.99);
		teams[team_id].players[i].cskill =
		    calculate_cskill(teams[team_id].players[i]);
	    }
	    else
		swap_players(team_id, i, team_id, substitute);
	}
    }
}

/* substitute a couple of players in a cpu-team */
void
update_teams_substitutes(gint team_id)
{
    gint i;
    gint substitute;
    gfloat rndom;

    /* mostly the better goalie plays */
    substitute_goalie(team_id);
    
    /* field players are simply rndomly substituted */
    for(i=1;i<11;i++)
    {
	rndom = rnd(0,1);
	if(rndom < 0.182)
	{
	    
	    substitute = 
		find_substitute(team_id,
				teams[team_id].players[i].
				cpos);
	    if(substitute != -1)
		swap_players(team_id, i, team_id, substitute);
	}
    }
}

/* replace some players rndomly so that there's some
   change in the cpu-teams */
void
update_teams_new_players(gint team_id)
{
    gint i;
    gfloat rndom;

    for(i=0;i<20;i++)
    {
	rndom = rnd(0,1);
	if(rndom < 0.003)
	    replace_player_by_new(team_id, i);
    }
}

/* change the playing structure of a cpu-team */
void
update_teams_new_structure(gint team_id)
{
    gint i;

    teams[team_id].structure = assign_playing_structure();

    for(i=0;i<11;i++)
	teams[team_id].players[i].pos =
	    teams[team_id].players[i].cpos =
	    get_position_from_structure(team_id, 0, i, 1);
}

/* weekly update of the teams.
   make players older (and maybe better /worse).
   for cpu-teams: change the team a bit, replace injured players
   by substitutes, replace old players by newly
   generated ones etc. */
void
update_teams(void)
{
    gint i;

    for(i=0;i<178;i++)
	if(i != 114 && i != 130 &&
	   i < 175)
	{
	    update_teams_players(i);
	    if(i != my_team)
	    {
		update_teams_injuries(i);
		update_teams_substitutes(i);
		update_teams_new_players(i);
		if(rnd(0,1) < 0.05)
		    update_teams_new_structure(i);
		if(rnd(0,1) < 0.1)
		    teams[i].style = 
			assign_playing_style();
	    }
	}
}

gint
players_on_transferlist(void)
{
    gint i, sum = 0;

    for(i=0;i<20;i++)
	if(transferlist[i].time > 0)
	    sum++;

    return sum;
}

gint
is_on_transferlist(gint team_id, gint player_number)
{
    gint i;

    for(i=0;i<20;i++)
	if(transferlist[i].time > -1 && 
	   transferlist[i].team_id == team_id &&
	   transferlist[i].player_number == player_number)
	    return 1;

    return 0;
}

/* find out whether the human player
   has to be notified of a new player on the
   transfer list */
void
check_notify(gint idx)
{
    gint i;
    gfloat skill_bound[2];
    gfloat etal_bound[2];
    gint age_bound[2];
    player pl = teams[transferlist[idx].team_id].
	players[transferlist[idx].player_number];
    gint value_bound = (options[OPT_NOTIFY_VALUE] > 0) ? 
      options[OPT_NOTIFY_VALUE] : transferlist[idx].estimates[0];
    gint positions[4] = {-1, -1, -1, -1};
    gint leagues[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    
    if(options[OPT_NOTIFY] == 0 || pl.team_id == my_team)
	return;

    for(i=0;i<2;i++)
    {
	skill_bound[i] = (options[OPT_NOTIFY_SKILL] > 0) ?
	    (gfloat)get_place(options[OPT_NOTIFY_SKILL], 12 + i * 10) / 10 : i * 10;
	etal_bound[i] = (options[OPT_NOTIFY_ETAL] > 0) ?
	    (gfloat)get_place(options[OPT_NOTIFY_ETAL], 12 + i * 10) / 10 : i * 10;
	age_bound[i] = (options[OPT_NOTIFY_AGE] > 0 && scout < 3) ?
	    get_place(options[OPT_NOTIFY_AGE], 12 + i * 10) : 17 + i * 20;
    }

    for(i=0;i<4;i++)
	if(options[OPT_NOTIFY_POS0 + i] == 1 || 
	   options[OPT_NOTIFY_POS] == 0)
	    positions[i] = i;
    for(i=0;i<6;i++)
	if(options[OPT_NOTIFY_LEAGUE1 + i] == 1 ||
	   options[OPT_NOTIFY_LEAGUE] == 0)
	    leagues[i] = i + 1;
    for(i=6;i<8;i++)
	if(options[OPT_NOTIFY_LEAGUE6] == 1 ||
	   options[OPT_NOTIFY_LEAGUE] == 0)
	    leagues[i] = i + 1;

    if( (pl.skill >= skill_bound[0] && pl.skill <= skill_bound[1]) &&
	(pl.etal >= etal_bound[0] && pl.etal <= etal_bound[1]) &&
	(pl.age >= age_bound[0] && pl.age <= age_bound[1]) &&
	transferlist[idx].estimates[0] <= value_bound &&
	is_in_list(positions, 0, 3, pl.pos) == 1 &&
	is_in_list(leagues, 0, 7, get_league_from_id(pl.team_id)) == 1 )
	notify_status[NOTIFY_TRANSFERS] = TRUE;
}

/* sort the transfer list */
void
sort_transferlist(gint sort_attrib)
{
    gint i;
    gint order[20];
    gint swap[20][5];
    gfloat to_order[20];
    gint sort_attrib_local = sort_attrib % 10;

    for(i=0;i<20;i++)
    {
	if(transferlist[i].time == -1)
	    to_order[i] = -100;
	else
	{
	    if(sort_attrib_local == SORT_POS)
		to_order[i] = 
		    (gfloat)teams[transferlist[i].team_id].players[transferlist[i].player_number].pos;
	    else if(sort_attrib_local == SORT_SKILL)
		to_order[i] = 
		    teams[transferlist[i].team_id].players[transferlist[i].player_number].skill;
	    else if(sort_attrib_local == SORT_AGE)
		to_order[i] = 
		    teams[transferlist[i].team_id].players[transferlist[i].player_number].age;
	    else if(sort_attrib_local == SORT_ETAL)
		to_order[i] = 
		    teams[transferlist[i].team_id].players[transferlist[i].player_number].etal;
	    else if(sort_attrib_local == SORT_LEAGUE)
		to_order[i] = 
		    (gfloat)get_league_from_id(transferlist[i].team_id);

	    if(sort_attrib > 10)
		to_order[i] *= -1;
	}
    }

    sort_float_array(to_order, order, 0, 19);

    for(i=0;i<20;i++)
    {
	swap[i][0] = transferlist[order[i]].team_id;
	swap[i][1] = transferlist[order[i]].player_number;
	swap[i][2] = transferlist[order[i]].time;
	swap[i][3] = transferlist[order[i]].estimates[0];
	swap[i][4] = transferlist[order[i]].estimates[1];
    }

    for(i=0;i<20;i++)
    {
	transferlist[i].team_id = swap[i][0];
	transferlist[i].player_number = swap[i][1];
	transferlist[i].time = swap[i][2];
	transferlist[i].estimates[0] = swap[i][3];
	transferlist[i].estimates[1] = swap[i][4];
    }    
}

/* add the specified player to the transferlist; he'll stay there
   for 'time' weeks */
void
add_transfer(gint team_id, gint player_number, gint time)
{
    gint i;

    for(i=0;i<20;i++)
	if(transferlist[i].time == -1)
	    break;

    if(i == 20)
	return;

    transferlist[i].time = time;
    transferlist[i].team_id = team_id;
    transferlist[i].player_number = player_number;

    estimate_value_wage(i);

    /* find out whether the human player
       has to be notified */
    check_notify(i);

    if(options[OPT_SORT_TRANSFERS] > 0)
	sort_transferlist(options[OPT_SORT_TRANSFERS]);
}

/* add a rndom number of new players to the transferlist */
void
add_new_transfers(void)
{
    gint rndom = rndi(0, 4);
    gint time;
    gint team_id, player_number;

    while(rndom > 0)
    {
	team_id = rndi(0, 174);
	player_number = rndi(0, 19);

	while(team_id == my_team || 
	      is_on_transferlist(team_id, player_number) == 1 ||
	      team_id == 114 || team_id == 130)
	{
	    team_id = rndi(0, 174);
	    player_number = rndi(0, 19);
	}

	time = rndi(2, 5);

	if(teams[team_id].players[player_number].skill < 9 ||
	   rnd(0,1) < 0.25)
	{
	    rndom--;
	    add_transfer(team_id, player_number, time);
	}
    }
}

/* remove a player from the transfer list */
void
remove_transfer(gint number, gboolean resort)
{
    gint i;

    for(i=19;i>number;i--)
	if(transferlist[i].time > 0)
	{
	    transferlist[number].time = 
		transferlist[i].time;
	    transferlist[number].team_id = 
		transferlist[i].team_id;
	    transferlist[number].player_number = 
		transferlist[i].player_number;
	    transferlist[number].estimates[0] =
		transferlist[i].estimates[0];
	    transferlist[number].estimates[1] =
		transferlist[i].estimates[1];	    

	    break;
	}
    
    transferlist[i].time = -1;
    
    if(options[OPT_SORT_TRANSFERS] > 0 && resort)
	sort_transferlist(options[OPT_SORT_TRANSFERS]);
}

/* each week some players are placed for a rndom
   number of weeks (2-5) on the transfer list.
   week 35 is transfer deadline, all players are removed
   and no new players appear until the following season */
void
update_transferlist(void)
{
    gint i;

    /* deadline passed */
    if(week > 35)
	return;

    /* deadline clearing */
    if(week == 35)
    {
	for(i=19;i>=0;i--)
	    if(transferlist[i].time > -1)
		remove_transfer(i, FALSE);
	return;
    }

    /* remove expired transfers */
    for(i=0;i<20;i++)
	if(transferlist[i].time > 0)
	    transferlist[i].time--;

    for(i=0;i<20;i++)	
	if(transferlist[i].time == 0)
	    remove_transfer(i, FALSE);

    add_new_transfers();

    if(options[OPT_SORT_TRANSFERS] > 0)
	sort_transferlist(options[OPT_SORT_TRANSFERS]);
}

/* move a player from the transferlist to the human player's team
   and replace him */
void
execute_transfer(gint idx)
{
    move_player(transferlist[idx].team_id,
		transferlist[idx].player_number,
		my_team);
    
    remove_transfer(idx, TRUE);
}

/* calculate the money for ticket sale and
   journey costs */
void
game_money(fixture fix)
{
    gint stadium_idx = get_statistics_variable_index(fix);
    gint journey_costs = 0;
    gint ticket_sale = 0;
    gfloat league = (gfloat)get_league_from_id(my_team);

    if( fix.team_id[1] == my_team ||
	(fix.type % 1000 == 0 &&
	 fix.type > 5000) )
    {
	/* journey costs */
	/* in european cups */
	if( (fix.type - fix.type % 100 >=  6000 &&
	     fix.type - fix.type % 100 < 9000) ||
	    fix.type == 35000)
	    journey_costs = -1 *
		round_integer((gint)gauss_dist(1500,3000,5000,7500), 2);
	else
	    journey_costs = -1 *
		round_integer((gint)gauss_dist(800,1000,2000,2500), 2);
    }

    /* ticket sale */
    if(fix.team_id[0] == my_team ||
	    (fix.type % 1000 == 0 &&
	     fix.type > 5000) )
    {
	ticket_sale = 
	    round_integer((gint)
			  ((gfloat)stadium_facts[stadium_idx][0] *
			   (13 + (league - 1) * 3)),
			  2);
	/* neutral stadium? */
	if(fix.type % 1000 == 0 &&
	   fix.type > 5000)
	    ticket_sale = (ticket_sale - ticket_sale % 2) / 2;
    }

    finances[FIN_MONEY] += (journey_costs + ticket_sale);

    finances[FIN_TICKETS] += ticket_sale;
    finances[FIN_JOURNEY] += journey_costs;
}

/* deduct player, scout and physio wages */
void
update_finances_wages(void)
{    
    gint i;
    gint league = get_league_from_id(my_team);
    gint player_wages = 0;
    gint scout_wage;
    gint physio_wage = 0, physio_wage_unit;

    physio_wage_unit = (1.1 - (gfloat)physio / 10) * 
	gauss_dist((6 + (league > 5) * 8 - league) * 3000,
		   (6 + (league > 5) * 8 - league) * 3000,
		   (6 + (league > 5) * 8 - league) * 8000,
		   (6 + (league > 5) * 8 - league) * 8000);

    scout_wage = 500 + 
	(6 + (league > 5) * 8 - league) * 
	(5 - scout % 10) * 1000;
    
    for(i=0;i<20;i++)
	if(teams[my_team].players[i].pos >= 0)
	{
	    player_wages += teams[my_team].players[i].wage;

	    if(teams[my_team].players[i].health > 0)
		physio_wage += physio_wage_unit;
	}

    /* wages of fired players */
    for(i = COUNT_OLD_WAGE1; i <= COUNT_OLD_WAGE5; i++)
	if(counters[i] > 0)
	{
	    counters[i]--;
	    player_wages += (gint)((gfloat)counters[i] / 10);

	    if(counters[i] % 100 == 0)
		counters[i] = 0;
	}
    
    finances[FIN_MONEY] -=
	(scout_wage + player_wages + physio_wage);
    finances[FIN_SCOUT] = -1 * scout_wage;
    finances[FIN_WAGES] = -1 * player_wages;
    finances[FIN_PHYSIO] = -1 * physio_wage;
}

/* return interest rate depending on user's debt */
gfloat
interest_rate(void)
{
    if(finances[FIN_DEBTS] > 2000000)
	return 0.01;
    else if(finances[FIN_DEBTS] > 1000000)
	return 0.015;
    else if(finances[FIN_DEBTS] > 750000)
	return 0.02;
    else if(finances[FIN_DEBTS] > 500000)
	return 0.025;
    else if(finances[FIN_DEBTS] > 250000)
	return 0.04;
    else if(finances[FIN_DEBTS] > 100000)
	return 0.05;
    else
	return 0.075;
}

/* update the finances of the human player's team */
void
update_finances(void)
{
    gint i, k = 0;
    /* interest rate */

    for(i=1;i<FIN_DEBTS;i++)
	finances[i] = 0;

    /* money from ticket sale and journey costs */
    while(fixtures[k].type != -1)
    {
	if(fixtures[k].week_number == week &&
	   my_team_involved(fixtures[k]))
	    game_money(fixtures[k]);
	k++;
    }

    /* wages */
    update_finances_wages();
    
    /* debt interest */
    

    finances[FIN_DEBTS] *= (1 + interest_rate());
}

/* update counters like number of weeks the human player's bank account
   is negative etc. */
void
update_counters(void)
{
    /* pay back limit for a loan */
    if(counters[COUNT_LOAN] > -1)
	counters[COUNT_LOAN]--;

    if(counters[COUNT_LOAN] == -1 &&
       finances[FIN_DEBTS] == 0)
	counters[COUNT_LOAN] = -2;
    
    /* time limit to get the bank account positive */
    if(counters[COUNT_POSITIVE] > -1)
	counters[COUNT_POSITIVE]--;

    if(counters[COUNT_POSITIVE] == -1 &&
       BUDGET > 0)
	counters[COUNT_POSITIVE] = -2;
    
    /* number of times the player overdrew the account.
       the time the team owners give him to get positive
       gets shorter and shorter */
    if(BUDGET < 0 &&
       counters[COUNT_POSITIVE] == -2)
    {
	counters[COUNT_OVERDRAWN]++;
	counters[COUNT_POSITIVE] = 8 - 2 * counters[COUNT_OVERDRAWN];
    }
    
    counters[COUNT_INC_SAF] = 0;

    if(my_team > 114)
	return;
	    
    /* give points for success or failure 
       if the human player plays in a normal
       league */
    if(rank[my_team] < 7)
	counters[COUNT_SUCCESS] += (7 - rank[my_team]);
    else if(rank[my_team] > 
	    teams_in_league(get_league_from_id(my_team)) - 6)
	counters[COUNT_SUCCESS] -=
	    (rank[my_team] + 6 -
	     teams_in_league(get_league_from_id(my_team)));
    /* if he's mediocre the points tend to zero */
    else
    {
	if(counters[COUNT_SUCCESS] < 0)
	    counters[COUNT_SUCCESS]++;
	else if(counters[COUNT_SUCCESS] > 0)
	    counters[COUNT_SUCCESS]--;
    }

    if(abs(counters[COUNT_SUCCESS]) < 35)
	counters[COUNT_WARNING] = 0;
}

/* change the playing structure of the human player's team */
void
change_structure(gint new_structure)
{
    gint i;
    gfloat old_average_cskill = average_skill(my_team, 11, TRUE);

    teams[my_team].structure = new_structure;

    for(i=0;i<11;i++)
    {
	teams[my_team].players[i].cpos =
	    get_position_from_structure(my_team, 0, i, 0);

	teams[my_team].players[i].cskill = 
	    calculate_cskill(teams[my_team].players[i]);
    }

    if(fabs(average_skill(my_team, 11, TRUE) -
	    old_average_cskill) > 0.05)
	bookmaker_re_tip();    
}

/* pick a new structure from the list of common structures
   for the human player's team */
void
cycle_through_structures(void)
{
    gint structures[5] = {532, 442, 352, 343, 433};
    gint new_structure = 0;

    if(teams[my_team].structure == 442)
	new_structure = 2;
    else if(teams[my_team].structure == 352)
	new_structure = 3;
    else if(teams[my_team].structure == 343)
	new_structure = 4;
    else if(teams[my_team].structure == 433)
	new_structure = 0;
    else
	new_structure = 1;

    change_structure(structures[new_structure]);
}

/* sort the tables */
void
update_ranks(void)
{
    gint i;
    
    /* english leagues */
    sort_teams(0, 19);
    for(i=0;i<3;i++)
	sort_teams(20 + i * 24, 19 + (i + 1) * 24);
    sort_teams(92, 113);

    /* CL */
    for(i=0;i<4;i++)
	sort_teams(114 + i * 4, 113 + (i + 1) * 4);
}

/* change the scout if the player has chosen so */
void
update_scout(void)
{
    gint i, j;

    if(scout < 10)
	return;
    
    scout = get_place(scout, 2);

    /* re-estimate talents and transferlist
       data */
    for(i=0;i<175;i++)
	if(i != 114 && i != 130)
	    for(j=0;j<20;j++)
		if(teams[i].players[j].pos >= 0)
		    teams[i].players[j].etal =
			estimate_talent(teams[i].players[j]);
    for(i=0;i<20;i++)
	if(transferlist[i].time > 0)
	    estimate_value_wage(i);
}

/* enlarge the stadium if necessary */
void
update_stadium(void)
{
    gint improve_index;

    if(counters[COUNT_INC_CAP] == 0)
	return;
        
    improve_index =
	(abs(counters[COUNT_INC_CAP]) - abs(counters[COUNT_INC_CAP]) % 10) / 500 - 1;

    if(stadium_improve_costs(0, improve_index) + finances[FIN_MONEY] +
       calculate_max_loan_credit(1) < 0)
    {
	show_popup_window("You didn't have enough money to enlarge your stadium.", NULL);
	counters[COUNT_INC_CAP] = 0;
	return;
    }

    if(counters[COUNT_INC_CAP] < 0)
    {
	counters[COUNT_INC_CAP] = abs(counters[COUNT_INC_CAP]);

	finances[FIN_MONEY] += stadium_improve_costs(0, improve_index);
	finances[FIN_STADIUM] += stadium_improve_costs(0, improve_index);
    }

    counters[COUNT_INC_CAP]--;

    /* it's time to make the enlargement */
    if(counters[COUNT_INC_CAP] % 10 == 9)
    {
	stadiums[my_team].capacity += counters[COUNT_INC_CAP] + 1;
	counters[COUNT_INC_CAP] = 0;
    }
}

void
change_my_team(gint new_team)
{
    gint i;
    
    /* re-generate old team */
    for(i=0;i<20;i++)
	replace_player_by_new(my_team, i);

    my_team = new_team;

    set_up_my_team();
}

void
swap_stadiums(gint id1, gint id2)
{
    swap_int(&stadiums[id1].capacity,
	     &stadiums[id2].capacity);

    swap_int(&stadiums[id1].average_attendance,
	     &stadiums[id2].average_attendance);

    swap_int(&stadiums[id1].games,
	     &stadiums[id2].games);
    
    swap_float(&stadiums[id1].safety,
	       &stadiums[id2].safety);
}

/* swap two teams */
void
swap_teams(gint team_id1, gint team_id2)
{
    gint i;
    gchar swap[50];

    strcpy(swap, teams[team_id1].name);
    strcpy(teams[team_id1].name, teams[team_id2].name);
    strcpy(teams[team_id2].name, swap);
    
    swap_int(&teams[team_id1].structure,
	     &teams[team_id2].structure);
    
    
    for(i=0;i<RES_END;i++)
	swap_int(&teams[team_id1].results[i],
		 &teams[team_id2].results[i]);

    for(i=0;i<20;i++)
	swap_players(team_id1, i, team_id2, i);

    if(team_id1 == my_team || team_id2 == my_team)
    {
	my_team = (team_id1 == my_team) ?
	    team_id2 : team_id1;
	swap_stadiums(team_id1, team_id2);
    }
}

/* find out at the end of a season whether the human player's team
   was ranked among the first 3 or got promoted via promotion games
   (or was 4th in league 1).
   if so, he gets some prize money */
void
prize_money(void)
{
    gint promotion_fixture_type = 55015 + 
	(get_league_from_id(my_team) - 2) * 1000;
    gint fix_idx;
    gint prize = 0;

    if(my_team > 114)
	return;

    if( (get_league_from_id(my_team) == 1 && week == 39 && rank[my_team] < 5) ||
	(((get_league_from_id(my_team) > 1 && 
	  get_league_from_id(my_team) < 5 && week == 47) ||
	 (get_league_from_id(my_team) == 5 && week == 43)) &&
	 rank[my_team] < 4) )
	prize = round_integer(75000 * powf((8 - get_league_from_id(my_team)), 0.75) *
			      (5 - rank[my_team]), 4);

    if(week == 49)
    {
	fix_idx = get_first_fixture(promotion_fixture_type);

	if(winner_of(fixtures[fix_idx]) == my_team)
	    prize = round_integer(75000 * powf((8 - get_league_from_id(my_team)), 0.75), 4);
    }
	
    finances[FIN_PRIZE] = prize;
    finances[FIN_MONEY] += prize;    
}

/* return average skill of a team */
gfloat
average_skill(gint team_id, gint bound, gboolean cskill)
{
    gint i, nr_of_players = 0;
    gfloat sum = 0;

    for(i=0;i<bound;i++)
	if(teams[team_id].players[i].pos >= 0 &&
	   teams[team_id].players[i].name[0] != '#')
	{
	    if(cskill)
		sum += teams[team_id].players[i].cskill * 
		    powf(teams[team_id].players[i].fitness, 0.25);
	    else
		sum += teams[team_id].players[i].skill;
	    nr_of_players++;
	}

    if(nr_of_players == 0)
	return 0;

    return sum / (gfloat)nr_of_players;
}

void
print_average_skill(gint league)
{
    gint i;
    gint bound[2];
    gfloat sum = 0;

    get_league_bounds(league, bound);

    for(i=bound[0];i<bound[1];i++)
    {
	sum += average_skill(i, 20, FALSE);
	if(i == my_team)
	    g_print("*** ");
	g_print("Team: %s average skill: %.2f style %d\n",
		teams[i].name, average_skill(i, 20, FALSE), teams[i].style);
    }
    
    g_print("\n average skill league %d: %.2f\n\n", league, sum / (bound[1] - bound[0] + 1));
}
