/**************************************************
 * Functions that are involved when deciding      *
 * the outcome of a game                          *
 **************************************************/

#include "defs.h"

/* get the number of goals for 'team' in the first leg */
gint get_first_leg_result(fixture fix, gint team)
{
    gint i = 0;

    while(fixtures[i].type > 0 &&
	  fixtures[i].type != fix.type - 5)
	i++;

    if(fixtures[i].type < 0)
    {
	g_print(" get_first_leg_result: reached end of fixtures\n");
	g_print(" fixture %s - %s type %d\n",
		teams[fix.team_id[0]].name,
		teams[fix.team_id[1]].name,
		fix.type);
	exit(-1);
    }

    return fixtures[i].result[team][0];
}

/* determine whether a game is a draw. this is simple if we have a
   single game but slightly more complicated if we have the second leg
   of a home-away pair of games.
   type indicates whether we have a result after extra-time or
   penalties:
   type = 0: regulation
   type = 1: extra-time
   type = 2: penalties */
gboolean is_draw(fixture fix, gint type)
{
    /* after penalties */
    if(type == 2)
	return (fix.result[0][2] == fix.result[1][2]) ?
	    TRUE : FALSE;

    /* FA-Cup or League Cup or CL/CWC final */
    if( (fix.type >= 9000 && fix.type < 12000)
	|| fix.type == 6000 || fix.type == 7000 ||
	fix.type == 25000 || fix.type == 35000)
    {
	if(fix.result[0][0] + fix.result[0][1] == 
	   fix.result[1][0] + fix.result[1][1] && type == 1)
	    return TRUE;

	if(fix.result[0][0] == fix.result[1][0] &&
	   type == 0)
	    return TRUE;
    }

    /* second leg of home-away */
    if(fix.type > 6000 && fix.type < 9000 &&
       fix.type % 10 == 5)
    {	
	if( get_first_leg_result(fix, 0) ==
	    fix.result[0][0] + fix.result[0][1] &&
	    get_first_leg_result(fix, 1) ==
	    fix.result[1][0] + fix.result[1][1])
	    return TRUE;
    }
    
    return FALSE;
}

/* the contribution of a player to the attack or defend value of
   his team */
gfloat prg_player_contribution(gint team_id, gint player_number,
			   gfloat home_advantage, gint attack)
{
    gfloat weights[3];

    if(teams[team_id].players[player_number].cpos == 0)
	return home_advantage *
	    teams[team_id].players[player_number].cskill *
	    powf(teams[team_id].players[player_number].fitness, 0.25);

    weights[1] = 0.625;

    if(attack)
    {
	weights[0] = 0.25;
	weights[2] = 1;	
    }
    else
    {
	weights[0] = 1;
	weights[2] = 0.25;
    }

    return home_advantage *
	weights[ teams[team_id].
		 players[player_number].cpos - 1 ] *
	teams[team_id].players[player_number].cskill *
	powf(teams[team_id].players[player_number].fitness, 0.25);
}

/* calculate the values that decide a game */
void prg_calculate_att_def(fixture fix, gfloat *attack_value,
			   gfloat *defend_value, gfloat *goalie_value,
			   gfloat *home_advantage)
{
    gint i, j;
    /* playing style factor */
    gfloat style_factor;

    for(i=0;i<2;i++)
    {
	style_factor = (gfloat)teams[fix.team_id[i]].style / 13.5;

	attack_value[i] = defend_value[i] = 0;
	
	goalie_value[i] = prg_player_contribution(fix.team_id[i],
						  0, home_advantage[i],
						  0);

	for(j=1;j<11;j++)
	{
	    attack_value[i] +=
		prg_player_contribution(fix.team_id[i],
					j, home_advantage[i], 1);
	    defend_value[i] +=
		prg_player_contribution(fix.team_id[i],
					j, home_advantage[i], 0);
	}    

	attack_value[i] *= 1 + style_factor;
	defend_value[i] *= 1 - style_factor;
    }
}

/* calculate the scoring probabilites of the field players */
void prg_calculate_player_probs(fixture fix,
				gfloat player_score_probs[][9])
{
    gint i, j;
    gfloat position_factor[3] =
	{1, 3, 7};
    gfloat sum;

    for(i=0;i<2;i++)
    {
	sum = 0;
	for(j=1;j<11;j++)
	    sum += teams[fix.team_id[i]].players[j].cskill *
		position_factor[teams[fix.team_id[i]].
				players[j].cpos - 1];
	
	player_score_probs[i][0] = (1 / sum) * 
	    teams[fix.team_id[i]].players[1].cskill *
	    position_factor[teams[fix.team_id[i]].
			    players[1].cpos - 1];
	for(j=2;j<10;j++)
	    player_score_probs[i][j - 1] = player_score_probs[i][j - 2] +
		teams[fix.team_id[i]].players[j].cskill *
		position_factor[teams[fix.team_id[i]].players[j].
				cpos - 1] / sum;
    }
}

/* calculate the values that decide a game */
void prg_calculate_values(fixture fix, gfloat *attack_value,
			  gfloat *defend_value, gfloat *goalie_value,
			  gfloat *scoring_probability,
			  gfloat player_score_probs[][9],
			  gfloat *home_advantage)
{
    gint i;

    prg_calculate_att_def(fix, attack_value, defend_value,
			  goalie_value, home_advantage);
    prg_calculate_player_probs(fix, player_score_probs);
 
    /* the probability that a team converts a goal chance.
       if the other team's goalie is injured or booked,
       the probability is of course rather high */
    for(i=0;i<2;i++)
    {
	scoring_probability[i] = (goalie_value[(i + 1) % 2] != 0) ?
	    0.0145 * attack_value[i] *
	    (1 - 0.05 * goalie_value[(i + 1) % 2]) *
	    (1 - 0.00769 * defend_value[(i + 1) % 2]) : 0.95;
    }
}

/* store information about a goal */
void prg_write_goal(fixture fix, gint team, gint scorer,
		    gint type)
{
    gint i;
    gint goal_idx[2];

    /* first increase the number of goals for the player */
    if((fix.type < 6000 || my_team > 114) &&
       teams[fix.team_id[team]].players[scorer].pos != 0)
	teams[fix.team_id[team]].players[scorer].goals++;
    
    /* if the human player's team is not involved
       return */
    if(!my_team_involved(fix))
	return;

    /* otherwise write extended information into a 'goal' struct */
    /* find the appropriate variable */
    goal_idx[0] = get_statistics_variable_index(fix);
    for(i=0;i<50;i++)
	if(goals[goal_idx[0]][i].minute < 0)
	{
	    goal_idx[1] = i;
	    break;
	}
    
    goals[goal_idx[0]][goal_idx[1]].type = type;

    /* assign rndom time */
    /* penalties */
    if(type > 1)
	goals[goal_idx[0]][goal_idx[1]].minute = 125;
    else
	goals[goal_idx[0]][goal_idx[1]].minute = (type) ?
	    rndi(94, 120) :
	    rndi(1, 93);

    goals[goal_idx[0]][goal_idx[1]].team_id = fix.team_id[team];

    goals[goal_idx[0]][goal_idx[1]].scorer = scorer;	    
}

/* simulate one goal chance */
void prg_simulate_chance(fixture *fix,
			 gfloat *scoring_probability,
			 gfloat player_score_probs[][9],
			 gint extra_time, gint team,
			 gboolean bookmaker)
{
    gint i, scorer = -1;
    gfloat rndom = rnd(0,1);

    /* find out whether it's a goal */
    if(rndom > scoring_probability[team])
	return;

    /* goals in extra time are written to result[][1] */
    fix->result[team][extra_time]++;
    
    /* increase number of goals for the goalie
       (only in league games and only if it's not
       a defender or so playing as goalie)
       and only if we don't simulate the game
       only for a bookmaker tip */
    if(!teams[fix->team_id[(team + 1) % 2]].
       players[0].pos && fix->type < 6000 &&
       !bookmaker)
	teams[fix->team_id[(team + 1) % 2]].
	    players[0].goals++;
    
    /* find out who's scored it */
    rndom = rnd(0,1);
    
    if(rndom >= player_score_probs[team][8])
	scorer = 10;
    else if(rndom < player_score_probs[team][0])
	scorer = 1;
    else
	for(i=1;i<9;i++)
	    if(player_score_probs[team][i - 1] <= rndom &&
	       rndom < player_score_probs[team][i])
		scorer = i + 1;

    if(!bookmaker)
	prg_write_goal(*fix, team, scorer, extra_time);
}

/* simulate a number of goal chances and store the results
   in appropriate places */
void prg_simulate_chances(fixture *fix, gfloat *attack_value,
			  gfloat *defend_value,
			  gfloat *scoring_probability,
			  gfloat player_score_probs[][9],
			  gint extra_time, gboolean bookmaker)
{    
    gint i, j;
    gint number_of_chances[2];
    
    for(i=0;i<2;i++)
    {
	number_of_chances[i] =
	    (gint)rint( attack_value[i] / defend_value[(i + 1) % 2] *
			8 * (1 - 0.66 * (gfloat)extra_time) );

	if(number_of_chances[i] > 14)
	    number_of_chances[i] = 14;

	for(j = 0; j < number_of_chances[i]; j++)
	    prg_simulate_chance(fix, scoring_probability,
				player_score_probs,
				extra_time, i, bookmaker);
    }
}

/* if bookmaker is TRUE we don't touch other variables
   than the fixture to write the result because we only want
   to have a bookmaker's tip */
void process_result_game(fixture *fix, gfloat *home_advantage,
			 gint extra_time, gboolean bookmaker)
{
    /* attacking and defending values of both teams */
    gfloat attack_value[2], defend_value[2];
    /* goalie value and scoring probabilities */
    gfloat goalie_value[2], scoring_probability[2];
    /* the individual probabilities for each player to 
       score a goal:
       P(player i of team j scored the goal) =
       player_score_probs[j][i + 1] - 
       player_score_probs[j][i]; */
    gfloat player_score_probs[2][9];

    /* calculate the values that decide a game */
    prg_calculate_values(*fix, attack_value, defend_value,
			 goalie_value, scoring_probability,
			 player_score_probs, home_advantage);
    
    /* simulate a couple of goal chances */
    prg_simulate_chances(fix, attack_value, defend_value,
			 scoring_probability,
			 player_score_probs, extra_time,
			 bookmaker);
}

/* simulate a penalty shot */
void simulate_penalty(fixture *fix, gfloat goalie_value,
		      gint player_number, gint team)
{
    gfloat rndom = rnd(0,1);
    gfloat scoring_probability;
    
    scoring_probability = 0.75 + 0.02 *
	teams[fix->team_id[team]].players[player_number].skill *
	powf(teams[fix->team_id[team]].
	     players[player_number].fitness, 0.25) -
	0.015 * goalie_value;
        
    /* record a failure as a special type of goal */
    if(rndom > scoring_probability)
    {	
	prg_write_goal(*fix, team, player_number, 15 + rndi(0, 3));
	return;
    }

    fix->result[team][2] += 1;
    
    prg_write_goal(*fix, team, player_number, 2);
}

/* calculate penalties. here we have no home advantage */
void process_result_penalties(fixture *fix)
{
    gint i, j, k;
    gint ended = 0;
    gint shooter[2];
    gint order[2][11];
    gfloat player_values[2][11];
    gfloat goalie_values[2];

    for(i=0;i<2;i++)
    {
	/* compute goalie values */
	goalie_values[i] = 
	    teams[fix->team_id[i]].players[0].cskill *
	    powf(teams[fix->team_id[i]].players[0].fitness, 0.25);

	/* sort players so that we might take the five best
	   players to shoot the penalties. here we count the
	   real skill values not cskill */
	for(j=0;j<11;j++)
	    player_values[i][j] =
		teams[fix->team_id[i]].players[j].skill *
		powf(teams[fix->team_id[i]].players[j].fitness, 0.25) *
		((j) ? 1 : 0);
	
	sort_float_array(player_values[i],
			 order[i],0, 10);
    }
    
    /* simulate the first five penalties */
    for(i=0;i<5;i++)
	for(j=0;j<2;j++)
	{
	    /* test for premature ending */
	    if(!ended)
		for(k=0;k<2;k++)
		    if( !ended &&
			(fix->result[k][2] -
			fix->result[(k + 1) % 2][2] >
			5 - i - k % 2 * j) )
			ended = 1;
	    
	    if(!ended)
		simulate_penalty(fix, goalie_values[(j + 1) % 2],
				 order[j][i], j);
	}
    
    shooter[0] = shooter[1] = 5;

    /* if the game's still a draw simulate more penalties */
    while(is_draw(*fix, 2))
    {
	/* here all players have to shoot if they're healthy
	   and not booked */
	for(i=0;i<2;i++)
	    while(teams[fix->team_id[i]].
		  players[order[i][shooter[i]]].cskill *
		  teams[fix->team_id[i]].
		  players[order[i][shooter[i]]].cpos == 0)
		shooter[i] = (shooter[i] + 1) % 11;
	
	for(i=0;i<2;i++)
	{
	    simulate_penalty(fix, goalie_values[(i + 1) % 2],
			     order[i][shooter[i]], i);
	    shooter[i] = (shooter[i] + 1) % 11;
	}
    }
}

void calculate_booking_player(fixture fix, gint team,
			      gint player_number)
{
    gint i;
    gfloat rndom = rnd(0,1);

    if(debug && fix.team_id[team] == my_team)
	return;

    player *pl =
	&(teams[fix.team_id[team]].players[player_number]);

    /* probabilities that a player
       with cpos i is sent off: prob[i][0];
       booked with yellow: prob[i][1] - bookprob[i][0] */
    gfloat prob[4][2]={{0.01,0.07},
		       {0.04,0.15},
		       {0.025,0.13},
		       {0.015,0.1}};
    
    /* probab. how long a player is banned if he's sent off
       in weeks: 1,2,3,4 or 5 */
    gfloat duration[6]={0,0.7,0.85,0.95,0.975,1};

    /* injured or banned players don't really participate
       in the game */
    if(!pl->cskill ||
       rndom > prob[pl->cpos][1] )
	return;

    /* sent off */
    if(rndom <= prob[pl->cpos][0])
    {
	rndom = rnd(0,1);
	for(i=1;i<6;i++)
	    if(duration[i - 1] < rndom &&
	       rndom <= duration[i])
	    {
		pl->booked += i + 1;
		pl->cskill = 0;
	    }
    }
    /* yellow */
    else if(rndom <= prob[pl->cpos][1])
    {
	/* if it's his 5th yellow card, he gets banned for
	   a week */
	if(pl->booked == 40)
	{
	    pl->booked = 2;
	    pl->cskill = 0;
	}
	else
	    pl->booked += 10;	
    }

    if(fix.team_id[team] == my_team)
	booked[player_number] = player_number;
}

void calculate_injury_player(fixture fix, gint team,
			     gint player_number)
{
    gint i;
    gfloat rndom = rnd(0,1);
    
    if(debug && fix.team_id[team] == my_team)
	return;

    /* for convenience, so as not to write
       teams[fix.team_id[team]].players[player_number]
       each time */
    player *pl =
	&(teams[fix.team_id[team]].players[player_number]);

    /* a field player is more
       likely to get injured */
    gfloat position_factor =
	(pl->cpos) ? 0.015 : 0.0075;

    /* the probability is multiplied
       by a factor that's reciprocal
       to the player's fitness */
    gfloat prob = 
	(pl->fitness < 0.025) ?
	40 * position_factor :
	1 / pl->fitness * position_factor;

    /* probabilities of different injuries */
    gfloat injury_probs[12]={0,
			     0.2, // concussion
			     0.4, // pulled muscle
			     0.55, // hamstring 
			     0.7, // groin injury
			     0.8, // fractured ankle
			     0.875, // cracked rib
			     0.915, // broken leg
			     0.955, // broken ankle
			     0.975, //  broken arm
			     0.99, // broken shoulder
			     1};  // torn crucial ligament

    /* duration of the injuries in weeks
       in the form of {min,max} */
    gint duration[11][2]={{1,4},{2,5},{3,7},{2,7},{4,8},
			  {4,12},{5,12},{6,16},{3,8},
			  {5,14},{12,30}};
    
    for(i=1;i<12;i++)
	if(injury_probs[i - 1] * prob < rndom &&
	   rndom <= injury_probs[i] * prob &&
	   pl->cskill != 0)
	{
	    pl->health = i * 1000 + 1 +	(gint)rint(
		gauss_dist(duration[i-1][0], duration[i-1][0],
			   duration[i-1][1], duration[i-1][1]));
	    pl->fitness = pl->cskill = 0;

	    /* if the player is in the human player's 
	       team store the fact that he's injured */
	    if(fix.team_id[team] == my_team)
		injuries[player_number] = player_number;
	}
}

/* find out if there were injuries or bookings */
void process_calculate_inj_book(fixture fix)
{
    gint i, j;

    /* injuries occur in the human player's team
       miraculously only during league games */
    if(fix.type >= 6000 && my_team_involved(fix) &&
       my_team < 114)
	return;
    
    for(i=0;i<2;i++)
	for(j=0;j<11;j++)
	{
	    calculate_booking_player(fix, i, j);
	    calculate_injury_player(fix, i, j);
	}
}

gfloat get_stadium_percentage(fixture fix)
{
    gfloat bound[2] = {0.8, 1.1};
    gfloat percentage;

    /* league game */
    if(fix.type < 6000)
    {
	/* how interesting is the opponent? */
	if(rank[fix.team_id[1]] < 9)
	    bound[0] = 0.9;
	else if(rank[fix.team_id[1]] > 16)
	    bound[0] = 0.75;
    }
    
    /* CL or CWC */
    else if(fix.type < 8000)
    {
	bound[0] = 0.85;
	bound[1] = 1.25;
	
	/* from quarter finals on: very popular */
	if(fix.type - fix.type % 1000 < 500)
	    bound[0] = 0.95;
    }

    /* UEFA Cup */
    else if(fix.type < 9000)
    {
	bound[0] = 0.75;
	bound[1] = 1.2;

	/* semi finals / finals */
	if(fix.type - fix.type % 1000 > 600)
	    bound[0] = 0.95;
	/* quarter finals */
	else if(fix.type - fix.type % 1000 > 400)
	    bound[0] = 0.9;
    }
    /* FA Cup and League Cup */
    else
    {
	bound[0] = 0.8;
	bound[1] = 1.15;
	
	/* if the away team is in a higher league
	   or we have 1/8 finals or so */
	if(get_league_from_id(fix.team_id[0]) - 
	   get_league_from_id(fix.team_id[1]) > 1 ||
	   fix.type - fix.type % 1000 < 400)
	{
	    bound[0] = 0.9;
	    bound[1] = 1.3;
	}
	else if(get_league_from_id(fix.team_id[0]) - 
		get_league_from_id(fix.team_id[1]) == 1 ||
		fix.type - fix.type % 1000 == 400)
	{
	    bound[0] = 0.85;
	    bound[1] = 1.25;
	}
    }
    
    percentage = rnd(bound[0], bound[1]);

    /* relegation games are important */
    if(percentage > 1 || fix.type > 12000)
	percentage = 1;

    return percentage;
}

gint stadium_events(fixture fix)
{
    gfloat rndom = rnd(0,1);
    gfloat money_loss =
	gauss_dist(0.6, 0.6, 1.1, 1.1);
    gfloat safety_reduce = 
	gauss_dist(0.08, 0.08, 0.15, 0.15);
    gfloat capacity_reduce_factor;
    gint type = 1;

    /* stadium safety deteriorates a bit */
    if(stadiums[my_team].safety > 0.01)
	stadiums[my_team].safety -= rnd(0, 0.008);

    /* special events only in league games */
    if(fix.type >= 6000 && my_team < 114)
	return 0;
    
    /* breakdown is covered at initialisation of the variables */
    if( rndom <= 0.5 * (1 - powf(stadiums[my_team].safety, 0.3)) );
    /* riots */
    else if( rndom <= 0.8 * (1 - powf(stadiums[my_team].safety, 0.3)) )
    {
	safety_reduce = gauss_dist(0.1, 0.1, 0.2, 0.2);
	money_loss = gauss_dist(0.7, 0.7, 1.2, 1.2);
	type = 2;
    }
    /* fire */
    else if( rndom <= (1 - powf(stadiums[my_team].safety, 0.3)) )
    {
	
	safety_reduce = gauss_dist(0.1, 0.15, 0.25, 0.25);
	money_loss = gauss_dist(0.9, 0.9, 1.3, 1.3);
	type = 3;
    }
    else
	return 0;
    
    capacity_reduce_factor =
	gauss_dist(0.9 - (gfloat)type / 15,
		   0.9 - (gfloat)type / 15,
		   1 - (gfloat)type / 15,
		   1 - (gfloat)type / 15);

    stadiums[my_team].safety -= safety_reduce;
    stadiums[my_team].capacity = 
	(gint)((gfloat)stadiums[my_team].capacity * 
	       capacity_reduce_factor);
    if(stadiums[my_team].safety < 0)
	stadiums[my_team].safety = 0;
    
    finances[FIN_STAD_BILLS] =
	stadiums[my_team].capacity * money_loss;
    finances[FIN_MONEY] -= finances[FIN_STAD_BILLS];

    return type;
}

void process_stadium(fixture fix)
{
    gint i;
    gint idx = get_statistics_variable_index(fix);
    gint attendance;
    gfloat percentage;

    if(!my_team_involved(fix))
	return;

    /* get the fullness percentage of the stadium
       depending on the type of game played */
    percentage = get_stadium_percentage(fix);

    attendance =
	(gint)rint( powf(stadiums[fix.team_id[0]].safety, 0.25) *
		    stadiums[fix.team_id[0]].capacity * percentage);
    
    /* the attendance almost never
       violates certain limits */
    for(i=2;i<6;i++)
	if(fix.type >= i * 1000 &&
	   fix.type < (i + 1) * 1000)
	    if((attendance < 4000 + (5 - i) * 5000 ||
		attendance > 22000 - (i - 2) * 4000) &&
	       rnd(0,1) > 0.05)
		attendance = rndi(4000 + (5 - i) * 5000,
				 22000 - (i - 2) * 4000);
    
    if(fix.type == 1000)
	if(attendance < 30000 || attendance > 90000)
	    attendance = rndi(30000, 90000);

    if(attendance > stadiums[fix.team_id[0]].capacity)
	attendance = stadiums[fix.team_id[0]].capacity;
    
    /* in cup finals we have a neutral stadium with
       amazing capacity */
    if(fix.type % 1000 == 0 && fix.type > 5000)
	attendance = gauss_dist(60000,65000,100000,120000);

    /* update stadium stats */
    stadiums[fix.team_id[0]].average_attendance = 
	(stadiums[fix.team_id[0]].average_attendance *
	 stadiums[fix.team_id[0]].games + attendance) /
	(stadiums[fix.team_id[0]].games + 1);

    stadiums[fix.team_id[0]].games++;

    stadium_facts[idx][0] = attendance;

    /* find out if there was fire or a riot
       or breakdown */
    if(fix.team_id[0] == my_team)
	stadium_facts[idx][1] =
	    stadium_events(fix);
}

void process_game_general(fixture fix)
{
    gint i, j;
    gint team_id;

    if(!(fix.type < 6000 ||
	 get_place(fix.type, 12) == 65))
	return;

    /* increase games and points for round robin games */
    for(i=0;i<2;i++)
    {
	/* with the english participant in the CL we
	   have to be careful to distinguish between
	   league games and CL games */
	if(fix.type > 6000)
	    team_id = (fix.team_id[i] < 114) ? 114 : fix.team_id[i];
	else
	    team_id = fix.team_id[i];

	/* games */
	teams[team_id].results[RES_GAM]++;
	/* goals for and against */
	teams[team_id].results[RES_GF] += fix.result[i][0];
	teams[team_id].results[RES_GA] += fix.result[(i + 1) % 2][0];

	/* won */
	if(fix.result[i][0] > fix.result[(i + 1) % 2][0])
	{
	    teams[team_id].results[RES_PTS] += 3;
	    teams[team_id].results[RES_WON]++;
	}
	/* draw */
	else if(fix.result[i][0] == fix.result[(i + 1) % 2][0])
	{
	    teams[team_id].results[RES_PTS]++;
	    teams[team_id].results[RES_DRAW]++;
	}
	/* lost */
	else
	    teams[team_id].results[RES_LOST]++;

	for(j=0;j<11;j++)
	    if(teams[team_id].players[j].cskill != 0)
		teams[team_id].players[j].games++;
    }
}

void process_game_sort_goals(fixture fix)
{
    gint i;
    gint idx = 
	get_statistics_variable_index(fix);
    gfloat to_order[50];
    gint team_id[50], scorer[50], type[50];
    gint order[50];

    if(!my_team_involved(fix))
	return;

    for(i=0;i<50;i++)
    {	
	if(goals[idx][i].minute != -1)
	{
	    team_id[i] = goals[idx][i].team_id;
	    scorer[i] = goals[idx][i].scorer;
	    type[i] = goals[idx][i].type;

	    if(goals[idx][i].type < 2)
		to_order[i] = (gfloat)goals[idx][i].minute * -1;
	    else
		to_order[i] = -125 - (gfloat)i / 20;
	}
	else
	    to_order[i] = -150;
    }

    sort_float_array(to_order, order, 0, 49);

    for(i=0;i<50;i++)
	if(goals[idx][i].minute != -1)
	{
	    goals[idx][i].team_id = team_id[order[i]];
	    goals[idx][i].scorer = scorer[order[i]];
	    goals[idx][i].type = type[order[i]];
	    goals[idx][i].minute = (gint)(to_order[order[i]] * -1);
	}
}

/* calculate the result of the game stored in 'fix' */
void process_game(fixture *fix)
{
    gfloat home_advantage[2] = {1, 1};

    if(fix->type % 1000 != 0 || 
       fix->type < 6000)
	home_advantage[0] =
	    gauss_dist(1.05, 1.05, 1.12, 1.12);

    /* calculate the result of 90 minutes */
    process_result_game(fix, home_advantage, 0, FALSE);

    /* if necessary calculate result of the extra time */
    if(is_draw(*fix, 0))
	process_result_game(fix, home_advantage, 1, FALSE);

    /* if necessary calculate penalties */
    if(is_draw(*fix, 1))
	process_result_penalties(fix);
    
    process_game_sort_goals(*fix);
    
    /* increasing number of games etc. */
    process_game_general(*fix);

    /* calculate injuries and bookings*/
    process_calculate_inj_book(*fix);

    /* write attendance etc. */
    process_stadium(*fix);        
}

/* calculate the results of a week's games */
void process_week_games(gint week_number)
{
    gint k;

    for(k=0;k<50;k++)
    {
	goals[0][k].minute = goals[1][k].minute = -1;
	if(k < 11)
	{
	    injuries[k] = injuries[k] = -1;
	    booked[k] = booked[k] = -1;
	}

	if(k<2)
	    stadium_facts[k][0] =
		stadium_facts[k][1] = 0;
    }
    
    for(k=FIX_END - 1;k>=0;k--)
    {
	if(fixtures[k].type != -1 &&
	   fixtures[k].week_number == week_number)
	    process_game(&fixtures[k]);	
    }
}
