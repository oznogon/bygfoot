/**************************************************
 * Functions that write the fixtures; either at   *
 * the beginning of the game or during the game   *
 **************************************************/

#include "game.h"
#include "fixture.h"
#include "maths.h"
#include "misc.h"
#include "team.h"

/* return the index 0 or 1 for the variables like
   stadium_facts[] or goals[] */
gint
get_statistics_variable_index(fixture fix)
{
    if(fix.type < 6000)
	return 1;

    if(fix.type > 50000 &&
       fix.type % 10 == 5)
	return 1;
    
    return 0;
}

/* find out if the human player's team is involved
   in the fixture */
gboolean
my_team_involved(fixture fix)
{
    if(teams[fix.team_id[0]].id == my_team ||
       teams[fix.team_id[1]].id == my_team)
	return TRUE;

    return FALSE;
}

/* find out whether the human player's team
   plays (or played) the given week */
gboolean
my_team_played(gint week_number)
{
    gint i;

    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type > 0 && 
	   fixtures[i].week_number == week_number &&
	   my_team_involved(fixtures[i]))
	    return TRUE;

    return FALSE;
}

/* return the index of the first fixture with type 'type' */
gint
get_first_fixture(gint type)
{
    gint i = 0;
    while(fixtures[i].type != type && i < FIX_END)
	i++;

    if(i == FIX_END)
    {
	g_print("get_first_free: reached end of fixtures; type %d\n",
		type);
	exit(-2);
    }
    
    return i;
}

gboolean
exists_fixture(gint fixture_type, gint places)
{
    gint i;

    for(i=0;i<FIX_END;i++)
	if( (fixture_type != 0 && 
	     get_place(fixtures[i].type, 10 + places) == get_place(fixture_type, 10 + places)) ||
	     (fixture_type == 0 && my_team_involved(fixtures[i])) )
	    return TRUE;

    return FALSE;
}

/* get the next week from 'week' up- or downwards
   featuring games of the human player's team or
   a certain league */
gint
get_my_fixture_week(gint week_number, gint direction)
{
    gint i, j;
    gint fixture_type = get_place(status, 3) +
	get_place(status, 4) * 10;
    gint places = 2;

    if(fixture_type >= 60 && fixture_type <= 90)
    {
	places = 1;
	fixture_type = get_place(fixture_type, 11);
    }

    if(direction == 1)
    {
	for(i=week_number + 1; i<49; i++)
	    for(j=0;j<FIX_END;j++)
		if(fixtures[j].type > 0 &&
		   ( (get_place(status, 4) == 0 && my_team_involved(fixtures[j])) ||
		     (get_place(status, 4) != 0 && get_place(fixtures[j].type, 10 + places) ==
		      fixture_type) ) &&
		   fixtures[j].week_number == i)
		    return i;
	if(exists_fixture(fixture_type, places))
	    return get_my_fixture_week(0, 1);
	else
	    return 1;
    }

    for(i=week_number - 1; i > 0; i--)
	for(j=0;j<FIX_END;j++)
	    if(fixtures[j].type > 0 &&
	       ( (get_place(status, 4) == 0 && my_team_involved(fixtures[j])) ||
		 (get_place(status, 4) != 0 && get_place(fixtures[j].type, 10 + places) ==
		  fixture_type) ) &&
	       fixtures[j].week_number == i)
		return i;
    
    if(exists_fixture(fixture_type, places))
	return get_my_fixture_week(49, -1);
    else
	return 1;
}

/* write one fixture */
void
write_fixture(gint idx, gint team0, gint team1,
		   gint type, gint week_number)
{
    fixtures[idx].result[0][0] = 
	fixtures[idx].result[0][1] =
	fixtures[idx].result[0][2] = 0;

    fixtures[idx].result[1][0] = 
	fixtures[idx].result[1][1] =
	fixtures[idx].result[1][2] = 0;

    fixtures[idx].type = type;
    fixtures[idx].week_number = week_number;

    fixtures[idx].team_id[0] = team0;
    fixtures[idx].team_id[1] = team1;
}

/* write one round of round robin games */
void
write_round_robin_round(gint *ids, gint max, gint special,
			     gint type, gint week_number)
{
    gint i, k;
    gint pairs[max][2];

    pairs[0][0] = ids[max * 2 - 1];
    pairs[0][1] = ids[special];

    for(i=1;i<max;i++)
    {
	pairs[i][0] = ids[(special + i) % (max * 2 - 1)];
	pairs[i][1] =
	    ids[(special + i + (max - 1 - i) * 2 + 1) % (max * 2 - 1)];
    }

    if(special % 2)
	for(i=0;i<max;i++)
	    swap_int(&(pairs[i][0]), &(pairs[i][1]));

    /* find the first free fixture list place */
    k = get_first_fixture(-1);
    
    for(i=0;i<max;i++)
	write_fixture(k + i, pairs[i][0], pairs[i][1],
		      type, week_number);
}

/* write round robin fixtures for the teams with ids
   between 'first_team_id' and 'last_team_id'.
   first_week: the week the first round is played
   week_leap: the number of weeks between two rounds
   the way it's done is difficult to explain, so there
   won't be many comments below */
void
write_round_robin(gint first_team_id,
		       gint last_team_id,
		       gint type, gint first_week,
		       gint week_leap)
{
    gint i, j, k, first_fix;
    gint max = last_team_id - first_team_id + 1;
    gint order[max];
    gint ids[max];

    write_permutation(order, 0, max - 1);

    for(i=0;i<max;i++)
	ids[order[i]] = (gint)teams[first_team_id + i].id;

    /* store the first free fixture index */
    first_fix = get_first_fixture(-1);
    
    /* first half of season */
    for(i=0;i<max - 1;i++)
	/* write one round of round robin games */
	write_round_robin_round(ids, max / 2, i, type,
				first_week + i * week_leap);

    /* second half of season */
    k = get_first_fixture(-1);
    for(i = 0; i < max - 1; i++)
	for(j = 0; j < max / 2; j++)
	    write_fixture(k + (i * max / 2) + j,
			  fixtures[first_fix + i * (max / 2) + j].
			  team_id[1],
			  fixtures[first_fix + i * (max / 2) + j].
			  team_id[0], type,
			  first_week + (max - 1 + i) * week_leap);
}

/* write rndomly home-away-pairings for the teams
   with ids team_ids[0] - team_ids[max - 1]
   used in UEFA and Cup Winners Cup */
void
write_pairings(gint *team_ids, gint max,
		    gint first_type, gint type_leap,
		    gint week_number, gint week_leap,
		    gint home_away)
{
    gint i;
    gint k;
    gint order[max];
    fixture fix;

    write_permutation(order, 0, max - 1);
    
    k = get_first_fixture(-1);

    for(i = 0; i < max / 2; i++)
    {	
	/* if we have FA or League Cup, we let
	   the weaker team play at home */
	if(!home_away &&
	   get_league_from_id(team_ids[order[2 * i]]) <
	   get_league_from_id(team_ids[order[2 * i + 1]]) )
	    swap_int(&(order[2 * i]), &(order[2 * i + 1]));
	    
	write_fixture(k + i * (1 + home_away),
		      team_ids[order[2 * i]],
		      team_ids[order[2 * i + 1]],
		      first_type + i * type_leap,
		      week_number);

	if(home_away)
	{
	    write_fixture(k + i * 2 + 1, team_ids[order[2 * i + 1]],
			  team_ids[order[2 * i]],
			  first_type + i * type_leap + 5,
			  week_number + week_leap);

	    if(week_leap == 0)
	    {
		fix = fixtures[k + i * 2 + 1];
		fixtures[k + i * 2 + 1] = fixtures[k + i * 2];
		fixtures[k + i * 2] = fix;
	    }
	}
    }
}

void
write_fixtures_euro_cups(void)
{
    gint i;
    gint ids[32];

    /* uefa */
    for(i=0;i<32;i++)
	ids[i] = (gint)teams[146 + i].id;

    write_pairings(ids, 32, 8010, 10, 10, 4, 1);

    /* cwc */
    for(i=0;i<16;i++)
	ids[i] = (gint)teams[130 + i].id;

    write_pairings(ids, 16, 7810, 10, 22, 4, 1);
}

void
write_fixtures_english_cups(void)
{
    /* in the first round, 14 / 36 teams from the
       premier league and division 1 are left out
       and participate only in round 2 */
    gint i;
    gint order[24];
    gint ids[100];

    /* fa cup */
    write_permutation(order, 0, 19);
    
    for(i=0;i<100;i++)
	ids[i] = (i < 6) ? order[14 + i] : 14 + i;
    
    write_pairings(ids, 100, 9600, 0, 20, 0, 0);
    
    /* league cup */
    write_permutation(order, 20, 43);
    
    for(i=0;i<56;i++)
	ids[i] = (i < 8) ? order[16 + i] : 36 + i;
    
    write_pairings(ids, 56, 11600, 0, 11, 0, 0);
}

/* write the fixtures for the round robin games of
   a season and the first rounds of the cups */
void
write_season_fixtures(void)
{
    gint i;

    /* league fixtures */

    /* premier division */
    write_round_robin(0, 19, 1000, 1, 1);
    /* divisions 1-3 */
    for(i=0;i<3;i++)
	write_round_robin(20 + i * 24, 19 + (i + 1) * 24,
			  (i + 2) * 1000, 1, 1);
    /* national conference */
    write_round_robin(92, 113, 5000, 1, 1);
    /* Champions League groups */
    for(i=0;i<4;i++)
	write_round_robin(114 + i * 4, 117 + i * 4,
			  6510 + i * 10, 2, 4);
    
    /* cups */
    /* uefa and cwc */
    write_fixtures_euro_cups();
    /* fa and league cup */
    write_fixtures_english_cups();
}

void
find_left_out(gint type, gint *left_out)
{
    gint i, k = 0;
    gint max = (type < 10000) ? 114 : 92;
    gint ids[max];

    /* if we don't have second round here,
       we return */
    if(type % 1000 < 600)
    {
	left_out[0] = -1;
	return;
    }

    for(i=0;i<max;i++)
	ids[i] = -1;

    while(fixtures[k].type > 0)
    {
	if(fixtures[k].type == type)
	{
	    ids[fixtures[k].team_id[0]] = fixtures[k].team_id[0];
	    ids[fixtures[k].team_id[1]] = fixtures[k].team_id[1];
	}
	
	k++;
    }

    k = 0;
    for(i=0;i<max;i++)
    {
	if(ids[i] == -1)
	{
	    left_out[k] = i;
	    k++;
	}
    }
    
    left_out[k] = -1;
}

/* return winner or runner-up of the game
   with fixture type 'type' */
gint
get_winner_runner_up(gint type, gint winner)
{
    gint i;

    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type == type)
	    break;
    
    if(i == FIX_END)
    {
	g_print("get_runner_up: could not find fixture %d\n",
		type);
	return -1;
    }

    if(winner == 1)
	return winner_of(fixtures[i]);
    
    return (fixtures[i].team_id[0] == winner_of(fixtures[i])) ?
	fixtures[i].team_id[1] : fixtures[i].team_id[0];
}

/* return the id of the winner of fix
   without taking into account whether there
   was a first leg; write 'L' or 'W' or 'D' from the
   point of view of team 'team_id' into buf if buf isn't NULL  */
gint
winner_of_regular(fixture fix, gchar *buf, gint team_id)
{
    gint i;

    /* first team won */
    if( (fix.result[0][0] > fix.result[1][0]) ||

	(fix.result[0][0] == fix.result[1][0] &&
	 fix.result[0][1] > fix.result[1][1]) ||

	(fix.result[0][0] == fix.result[1][0] &&
	 fix.result[0][1] == fix.result[1][1] &&
	 fix.result[0][2] > fix.result[1][2]) )	
    {
	if(buf != NULL)
	{
	    if(team_id == 0)
		strcpy(buf, "W ");
	    else
		strcpy(buf, "L ");
	}

	return fix.team_id[0];
    }
    
    /* second team won */
    for(i=0;i<3;i++)
	if(fix.result[0][i] < fix.result[1][i])
	{
	    if(buf != NULL)
	    {
		if(team_id == 1)
		    strcpy(buf, "W ");
		else
		    strcpy(buf, "L ");
	    }

	    return fix.team_id[1];
	}


    if(buf != NULL)
	strcpy(buf, "D ");

    /* draw */
    return -1;
}

/* return the id of the winner of fix */
gint
winner_of(fixture fix)
{
    if(fix.type < 6000 ||
       get_place(fix.type, 12) == 65)
    {
	g_print("winner_of: league game argument\n");
        exit(-3);
    }

    /* single game */
    if((fix.type >= 9000 && fix.type <= 40000) ||
       fix.type == 6000 || fix.type == 7000)
	return
	    (fix.result[0][0] + fix.result[0][1] + fix.result[0][2] < 
	     fix.result[1][0] + fix.result[1][1] + fix.result[1][2]) ?
	    fix.team_id[1] : fix.team_id[0];
    
    /* home/away games */
    /* penalties? */
    if(fix.result[0][2] + fix.result[1][2] != 0)
	return (fix.result[0][2] < fix.result[1][2]) ?
	    fix.team_id[1] : fix.team_id[0];
    /* extra time? */
    if(fix.result[0][1] + fix.result[1][1] != 0)
	return (fix.result[0][1] < fix.result[1][1]) ?
	    fix.team_id[1] : fix.team_id[0];
    /* normal */
    if( fix.result[0][0] + get_first_leg_result(fix, 1) >
	fix.result[1][0] + get_first_leg_result(fix, 0) ||
	(fix.result[0][0] + get_first_leg_result(fix, 1) ==
	 fix.result[1][0] + get_first_leg_result(fix, 0) &&
	 get_first_leg_result(fix, 1) > fix.result[1][0]) )
	return fix.team_id[0];

    return fix.team_id[1];
}

/* find the winners of the fixtures with appropriate types */
void
find_winners(gint first_type, gint leap, gint max, gint *winners)
{
    gint i = 0, k;
    
    k = get_first_fixture(first_type);
    for(i=0;i<max;i++)
	winners[i] = winner_of(fixtures[k + i * leap]);
}

void
update_fixtures_english_cups(gint type)
{
    gint i, j;
    gint max = (gint)powf(2, (type % 1000) / 100);
    gint ids[max];
    gint left_out[37];
    gint winners[max];
    gint order[max];

    write_permutation(order, 0, max - 1);

    /* when writing the second round, we have
       to find out which teams have been left out
       of the second round */
    find_left_out(type, left_out);

    for(i=0;i<37;i++)
    {
	if(left_out[i] == -1)
	    break;

	ids[i] = left_out[i];
    }

    find_winners(type, 1, max - i, winners);

    for(j=i;j<max;j++)
	ids[j] = winners[j - i];

    write_pairings(ids, max, type - 100, 0, week + 3, 4, 0);
}

/* write quarter-final fixtures after the group phase */
void
update_champions_league_group(void)
{
    gint i, j;
    gint pairs[2];
    
    for(i=0;i<2;i++)
	for(j=0;j<2;j++)
	{
	    pairs[0] = teams[rank_ids[114 + i * 8 + j]].id;
	    pairs[1] = teams[rank_ids[114 + i * 8 + (5 - j)]].id;

	    write_pairings(pairs, 2, 6410 + (i + j) * 10,
			   0, 30, 4, 1);
	}
}

void
update_home_away_fixtures(gint first_type, gint max, gint new_type)
{
    gint i;
    gint winners[max];
    gint order[max];
    gint ids[max];

    find_winners(first_type, 2, max, winners);
    write_permutation(order, 0, max - 1);
    
    for(i=0;i<max;i++)
	ids[i] = winners[order[i]];

    write_pairings(ids, max, new_type, 10, week + 3, 4, 1);
}

void
update_final(gint first_type, gint new_type)
{
    gint winners[2];

    find_winners(first_type, 2, 2, winners);

    write_pairings(winners, 2, new_type, 0, week + 3, 0, 0);
}

void
update_uefa_supercup(void)
{
    gint i;
    gint idx = get_first_fixture(-1);
    gint team_id[2] = {-1, -1};

    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type == 6000)
	    team_id[0] = winner_of(fixtures[i]);
	else if(fixtures[i].type == 8815)
	    team_id[1] = winner_of(fixtures[i]);

    if(team_id[0] == -1 || team_id[1] == -1)
    {
	g_print("update_uefa_supercup: could not find winners\n");
	return;
    }

    write_fixture(idx, team_id[0], team_id[1],
		  35000, 47);
}

void
update_charity_shield(void)
{
    gint i;
    gint idx = get_first_fixture(-1);
    gint team_id[2] = {rank_ids[0], -1};

    /* find the fa cup winner */
    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type == 9000)
	    team_id[1] = winner_of(fixtures[i]);

    if(team_id[1] == -1)
    {
	g_print("update_charity_shield: could not find fa cup winner\n");
	return;
    }

    /* if there's a double winner, take league runner-up */
    if(team_id[1] == team_id[0])
	team_id[1] = rank_ids[1];

    write_fixture(idx, team_id[0], team_id[1],
		  25000, 45);
}

void
update_relegation_second(void)
{
    gint i, j, k;
    gint pairs[2];
    gint qualified_teams[4][2];

    for(j=0;j<4;j++)
	for(i=0;i<FIX_END;i++)
	    for(k=0;k<2;k++)
		if(fixtures[i].type == 51015 + j * 1000 + k * 10)
		    qualified_teams[j][k] = winner_of(fixtures[i]);

    for(i=0;i<4;i++)
    {
	pairs[0] = qualified_teams[i][0];
	pairs[1] = qualified_teams[i][1];

	write_pairings(pairs, 2, 55010 + i * 1000,
		       0, 48, 0, 1);
    }
}

void
update_relegation_first(void)
{
    gint i, j;
    gint qualified_teams[4][4];
    gint pairs[2];

    /* first round */
    for(i=0;i<4;i++)
	for(j=0;j<4;j++)
	    qualified_teams[i][j] = rank_ids[23 + j + i * 24];

    for(i=0;i<4;i++)
	for(j=0;j<2;j++)
	{
	    pairs[0] = qualified_teams[i][j];
	    pairs[1] = qualified_teams[i][3 - j];

	    write_pairings(pairs, 2, 51010 + i * 1000 + j * 10,
			   0, 47, 0, 1);
	}
}

/* weekly update of the fixtures, i.e. new fixtures are
   generated when a round of a cup is over */
void
update_fixtures(void)
{
    /* league cup */
    if(week > 11 && week < 33 && week % 4 == 0)
	update_fixtures_english_cups(11600 -
				     ((week - 12) / 4) * 100);
    /* fa cup */
    else if(week > 20 && week < 42 && week % 4 == 1)
	update_fixtures_english_cups(9600 -
				     ((week - 21) / 4) * 100);
    /* european cups */
    /* UEFA */
    if(week > 14 && week < 40 &&
       week % 8 == 7)
	update_home_away_fixtures(8015 + ((week - 15) / 8) * 200,
				  powf(2, 4 - ((week - 15) / 8)),
				  8210 + ((week - 15) / 8) * 200);
    /* CL and CWC */
    /* quarter-finals */
    if(week == 27)
    {
	update_champions_league_group();	
	update_home_away_fixtures(7815, 8, 7410);
    }
    /* semi-finals */
    if(week == 35)
    {
	update_home_away_fixtures(6415, 4, 6210);
	update_home_away_fixtures(7415, 4, 7210);
    }
    /* final */
    if(week == 43)
    {
	update_final(6215, 6000);
	update_final(7215, 7000);
    }

    /* charity shield */
    if(week == 44)
	update_charity_shield();

    if(week == 46)
    {
	update_uefa_supercup();
	update_relegation_first();
    }

    if(week == 47)
	update_relegation_second();
}
