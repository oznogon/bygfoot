/**************************************************
 * Functions working with teams                   *
 **************************************************/

#include "finance.h"
#include "fixture.h"
#include "game_gui.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "team.h"
#include "text_files.h"
#include "window.h"

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

  if(season * week == 1)
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
      bound[0] = 114;
      bound[1] = 130;
    }    
  else if(league == 7)
    {
      bound[0] = 130;
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
direct_comparison(gint team_id1, gint team_id2)
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

/* compare two teams regarding defensive */
gint
team_compare_defensive(const void *value1, const void *value2)
{
  gint *team1 = (gint*)value1;
  gint *team2 = (gint*)value2;

  if(teams[*team1].results[RES_GA] >
     teams[*team2].results[RES_GA])
    return 1;

  if(teams[*team2].results[RES_GA] >
     teams[*team1].results[RES_GA])
    return -1;

  if(teams[*team1].results[RES_GF] >
     teams[*team2].results[RES_GF])
    return -1;

  if(teams[*team2].results[RES_GF] >
     teams[*team1].results[RES_GF])
    return 1;

  if(teams[*team1].style >
     teams[*team2].style)
    return 1;
  
  return -1;
}

/* compare two teams regarding offensive */
gint
team_compare_offensive(const void *value1, const void *value2)
{
  gint *team1 = (gint*)value1;
  gint *team2 = (gint*)value2;

  if(teams[*team1].results[RES_GF] >
     teams[*team2].results[RES_GF])
    return -1;

  if(teams[*team2].results[RES_GF] >
     teams[*team1].results[RES_GF])
    return 1;

  if(teams[*team1].results[RES_GA] >
     teams[*team2].results[RES_GA])
    return 1;

  if(teams[*team2].results[RES_GA] >
     teams[*team1].results[RES_GA])
    return -1;

  if(teams[*team1].style >
     teams[*team2].style)
    return -1;
  
  return 1;
}

/* compare two teams regarding points etc. */
gint
team_compare_tables(const void *value1, const void *value2)
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

/* sort the teams between the bounds according to 'compare' and
   write the result to 'sorted' */
void
sort_teams(gint *bound, gint *sorted, 
	   gint start_idx, gint end_idx,
	   GCompareFunc compare)
{
  gint i;
  GArray *team_array = 
    g_array_new(FALSE, FALSE, sizeof(gint));

  for(i = bound[0]; i < bound[1]; i++)
    g_array_append_val(team_array, i);

  g_array_sort(team_array, compare);

  for(i = start_idx; i <= end_idx; i++)
    sorted[i] = g_array_index(team_array, gint, i - start_idx);
}

/* sort the teams by points and goals and write
   the result into the global ranks array. */
void
sort_teams_ranks(gint league)
{
  gint i;
  gint bound[2], bound2[2];
  gint sorted[24];

  get_league_bounds(league, bound);
    
  if(league < 6)
    sort_teams(bound, sorted, 0, bound[1] - bound[0] - 1, team_compare_tables);
  else
    {
      for(i=0;i<4;i++)
	{
	  bound2[0] = 114 + i * 4;
	  bound2[1] = bound2[0] + 4;
	  sort_teams(bound2, sorted, i * 4, (i + 1) * 4 - 1, team_compare_tables);
	}
    }

  for(i=bound[0];i<bound[1];i++)
    {
      rank_ids[i] = sorted[i - bound[0]];
      rank[sorted[i - bound[0]]] = (league < 6) ? 
	i - bound[0] + 1 : (i - bound[0]) % 4 + 1;
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
		{
		  teams[team_id].players[i].booked = 
		    get_place(teams[team_id].players[i].booked, 2) * 10;
		  teams[team_id].players[i].cskill = 
		    calculate_cskill(teams[team_id].
				     players[i]);
		}
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
    
  for(i=1;i<=6;i++)
    sort_teams_ranks(i);
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

  if(counters[COUNT_INC_CAP] < 0 && 
     stadium_improve_costs(0, improve_index) + BUDGET < 0)
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

/* award the best teams in a season with some money */
void
season_awards_team(gint prized_teams[][2])
{
    gint i;
    gint league = get_league_from_id(my_team);
    gint promotion_fixture_type = 55015 + 
	(league - 2) * 1000;
    gint fix_idx = get_first_fixture(promotion_fixture_type);
    gint bound[2];
    gint best_off_def[2];

    get_league_bounds(league, bound);

    for(i=0;i<3;i++)
    {
      prized_teams[i][0] = rank_ids[bound[0] + i];
      prized_teams[i][1] = round_integer(75000 * powf((8 - league), 0.75) *
					 (5 - i), 4);
    }
    
    if(league == 1)
      prized_teams[PRIZE_TEAM_PROM][0] = rank_ids[bound[0] + 3];
    else
      prized_teams[PRIZE_TEAM_PROM][0] = winner_of(fixtures[fix_idx]);

    prized_teams[PRIZE_TEAM_PROM][1] = 
      round_integer(75000 * powf((8 - league), 0.75), 4);

    sort_teams(bound, best_off_def, 0, 0, team_compare_offensive);
    sort_teams(bound, best_off_def, 1, 1, team_compare_defensive);

    for(i=PRIZE_TEAM_OFFENSIVE;i<=PRIZE_TEAM_DEFENSIVE;i++)
      {
	prized_teams[i][0] = best_off_def[i - PRIZE_TEAM_OFFENSIVE];
	prized_teams[i][1] = round_integer(75000 * powf((8 - league), 0.75) * 3, 4);
      }

    for(i=0;i<PRIZE_TEAM_END;i++)
      if(prized_teams[i][0] == my_team)
	{
	  finances[FIN_PRIZE] += prized_teams[i][1];
	  finances[FIN_MONEY] += prized_teams[i][1];
	}
}
