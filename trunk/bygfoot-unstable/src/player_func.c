/**************************************************
 * Functions working with players                 *
 **************************************************/

#include "defs.h"

/* assign value of a player depending on
   skill, talent, age and league */
gint
assign_value(player pl)
{
    gint value;
    gint league = get_league_from_id(pl.team_id);

    value = (gint)(powf(((pl.skill + pl.talent) * 0.8), 2) * 5000);
    if(league < 6)
	value =
	    (gint)rint( value + 20000 * (5 - league) );
    else
	value = (gint)rint( value + 125000 );

    if(pl.age <= 22)
	value = (gint)((gfloat)value * 1.1);
    else if(pl.age <= 20)
	value = (gint)((gfloat)value * 1.15);
    else if(pl.age >= 32)
	value = (gint)((gfloat)value * 0.9);
    else if(pl.age >= 34)
	value = (gint)((gfloat)value * 0.8);

    value = round_integer(value, 2);

    return value;
}

/* assign wage depending on league and value */
gint
assign_wage(player pl)
{
    gfloat wage;
    gfloat league =
	(gfloat)get_league_from_id(pl.team_id);

    wage = rint(((gfloat)pl.value / 100) *
		gauss_dist(0.85,0.85,1.15,1.15) );

    if(league < 6)
	wage *= (1 + (5 - league) / 13);
    else
	wage *= (1 + (11 - league) / 12);

    return round_integer((gint)wage, 1);
}

/* return a player's talent depending on his league and skill */
gfloat
calculate_talent(gint league, player pl)
{
    if(league < 6)
	return pl.skill +
	    (9.9 - pl.skill) * rnd(0.5 - ((gfloat)league / 8),
				   1.12 - ((gfloat)league / 8));
    return pl.skill +
	(9.9 - pl.skill) * rnd(1.2 - ((gfloat)league / 10), 1);
}

/* determine on which position a given player plays
   depending on his number in the team and on the structure
   the team plays; if 'generation' is 1, this is position
   assignment, else just a query */
gint
get_position_from_structure(gint team_id, gint structure,
				 gint player_number,
				 gint generation)
{
    gint bound[2] =
	{get_place(teams[team_id].structure, 3),
	 get_place(teams[team_id].structure, 3) +
	 get_place(teams[team_id].structure, 2)};
    
    if(player_number == 0)
	return 0;
    else if(1 <= player_number && player_number <= bound[0])
	return 1;
    else if(bound[0] + 1 <= player_number && player_number <= bound[1])
	return 2;
    else if(player_number <= 10)
	return 3;

    if(generation == 0)
	return teams[team_id].players[player_number].pos;
    
    bound[0] = (structure == 0) ? 14 : get_place(structure, 3) + 11;
    bound[1] = (structure == 0) ? 17 : get_place(structure, 3) + 
	get_place(structure, 2) + 11;

    if(player_number == 11)
	return 0;
    if(12 <= player_number && player_number <= bound[0])
	return 1;
    else if(bound[0] + 1 <= player_number && player_number <= bound[1])
	return 2;

    return 3;
}

void
adapt_ability(gint team_id, gint player_number)
{
    gint i;
    gfloat mean_talent = 0;
    gfloat mean_skill = 0;
    gint number_of_players = 0;

    for(i=0;i<20;i++)
	if(i != player_number &&
	   teams[team_id].players[i].pos >= 0)
	{
	    mean_skill += teams[team_id].players[i].skill;
	    mean_talent += teams[team_id].players[i].talent;
	    number_of_players++;
	}

    mean_talent /= (gfloat)number_of_players;
    mean_skill /= (gfloat)number_of_players;
    
    teams[team_id].players[player_number].skill =
	rnd(mean_skill * 0.9, mean_skill * 1.1);
    teams[team_id].players[player_number].talent =
	rnd(mean_talent * 0.9, mean_talent * 1.1);
    
    if(teams[team_id].players[player_number].talent > 9.9)
	teams[team_id].players[player_number].talent = 9.9;    
 
    if(teams[team_id].players[player_number].skill > 9.9 ||
       teams[team_id].players[player_number].skill > 
       teams[team_id].players[player_number].talent)
	teams[team_id].players[player_number].skill = 
	    teams[team_id].players[player_number].talent;

    teams[team_id].players[player_number].cskill = 
	teams[team_id].players[player_number].skill;
}

/* estimate a player's talent value. depends on how
   good the human player's scout is.
   the estimate is between the player's skill
   and 9.9 */
gfloat
estimate_talent(player pl)
{
    gint i;

    /* the maximal deviance in both directions */
    gfloat deviance_bound[2] =
	{pl.talent - pl.skill, 9.9 - pl.talent};

    /* the scout's maximal deviance */
    gfloat scout_deviance = (gfloat)(scout % 10);

    /* adjust deviance_bounds with regard to the scout's
       deviance */
    for(i=0;i<2;i++)
	deviance_bound[i] = (deviance_bound[i] < scout_deviance) ?
	    deviance_bound[i] : scout_deviance;

    /*d*/
    if(pl.talent - deviance_bound[0] < pl.skill && debug)
	printf("estimtal error: team %d player %s skill %.1f bound %.1f\n",
	       pl.team_id, pl.name, pl.skill, pl.talent - deviance_bound[0]);

    return rnd(pl.talent - deviance_bound[0],
	       pl.talent + deviance_bound[1]);
}

/* return the mean skill for european players.
   depends on the mean skill of premiership players */
gfloat
mean_skill_euro(gint league)
{
    gint i, j, nr_of_players;
    gfloat sum;

    sum = 0;
    nr_of_players = 0;
    for(i=0;i<20;i++)
	for(j=0;j<20;j++)
	    if(teams[i].players[j].pos >= 0)
	    {
		nr_of_players++;
		sum += teams[i].players[j].skill;
	    }
    
    /* CL */
    if(league == 0)
	return ((sum / (gfloat)nr_of_players) + 0.8 < 9.5) ?
	    (sum / (gfloat)nr_of_players) + 0.8 : 9.5;

    /* CWC */
    if(league == 1)
	return ((sum / (gfloat)nr_of_players) + 0.5 < 9.2) ?
	    (sum / (gfloat)nr_of_players) + 0.5 : 9.2;

    /* UEFA */
    return ((sum / (gfloat)nr_of_players) + 0.2 < 8.8) ?
	(sum / (gfloat)nr_of_players) + 0.2 : 8.8;
}

/* fill in a player's data depending on league and
   the player's number in his team */
void
generate_player(gint team_id, gfloat team_factor,
		     gint player_number)
{
    /* mean skill value for different leagues */
    gfloat mean_skills[8] = {8.0, /* Premier Division */
			     7.0, /* Division 1 */
			     6.0, /* Division 2 */
			     5.0, /* Division 3 */
			     4.0, /* Nationwide Conference */
			     mean_skill_euro(0), /* Champions' League */
			     mean_skill_euro(1), /* Cup Winners' Cup */
			     mean_skill_euro(2)}; /* UEFA Cup */
    gint league = get_league_from_id(team_id);
    player *pl = &(teams[team_id].players[player_number]);

    /* assign position: goalie, defender etc. */
    pl->pos = pl->cpos =
	get_position_from_structure(team_id, 0, player_number, 1);

    pl->age = gauss_dist(18,18,36,36);

    /* goalies have a higher peak age */
    pl->peak_age = rnd(30, 33) + (pl->pos == 0) * rnd(1, 2.5);
    pl->goals =
	pl->games =    
	pl->booked =
	pl->health = 0;
    
    pl->team_id = team_id;
	
    pl->last_skill_update = rndi(0, 10);

    /* assign skill depending on league, team quality and age */
    pl->skill = pl->cskill =
	gauss_dist(1.0, mean_skills[league - 1] - 2,
		   mean_skills[league - 1] + 2, 9.9) * team_factor;
    
    if(pl->age <= 20 || pl->age >= 34)
	pl->cskill = pl->skill *= 0.8;
    else if(pl->age <= 22 || pl->age >= 32)
	pl->cskill = pl->skill *= 0.9;

    /* cut-off */
    if(pl->skill > 9.9 ||
       strcmp(pl->name,"Both") == 0) // :-)
	pl->cskill = pl->skill = rnd(9.7, 9.9);
    else if(pl->skill < 1.0)
	pl->cskill = pl->skill = rnd(1.0, 1.2);
    
    pl->talent = calculate_talent(league, *pl);

    /* scout's estimate for the
       player's talent */
    pl->etal = estimate_talent(*pl);

    /* value and wage depend on league, skill and age */
    pl->value = assign_value(*pl);
    pl->wage = assign_wage(*pl);

    /* fitness is measured in percent */
    pl->fitness = gauss_dist(.7,.85,.99,.99);       
}

/* calculate a player's current skill depending on position,
   health etc. */
gfloat
calculate_cskill(player pl)
{
    gfloat reduce_factor = 0.75;

    if(pl.health > 0 || pl.booked % 10 > 0)
	return 0;
    
    if(pl.pos == pl.cpos)
	return pl.skill;

    /* goalies play poorly as field players and vice versa */
    if(pl.cpos == 0 || pl.pos == 0)
	reduce_factor = 0.5;
    else if(abs(pl.cpos - pl.pos) == 2)
	reduce_factor = 0.65;

    return (pl.talent * reduce_factor < pl.skill) ?
	pl.talent * reduce_factor : pl.skill;
}

/* update a player's skill depending on age, talent
   and how many weeks ago the last update was made */
void
update_skill(gint team_id, gint player_number)
{
    gfloat rndom = rnd(0,1);
    gfloat update_prob;
    player *pl =  &(teams[team_id].players[player_number]);
    
    /* older players always continue in their decay :-( */
    if(pl->age > pl->peak_age)
    {
	if(pl->health == 0)
	    pl->last_skill_update++;
	else
	    pl->last_skill_update += 1.5;
    }
    /* injured players are retarded in their development;
       players who never play rarely get better */
    if(pl->games >= (gfloat)week / 3 && pl->health == 0)
	pl->last_skill_update++;
    else if(pl->health == 0 && rnd(0,1) < 0.5)
	pl->last_skill_update++;
    
    /* at the beginning of a new season there's alway an update */
    if(pl->last_skill_update < 12 && week != 49)
	return;
    
    update_prob = pl->last_skill_update * 0.2 - 2.2;

    if(rndom > update_prob && week != 49)
	return;
    
    pl->last_skill_update = (week == 49) ?
	rndi(0, 10) : 0;

    if(pl->peak_age - pl->age > 5)
    {
	if(pl->talent - pl->skill < 0.4)
	    pl->skill += ((pl->talent - pl->skill) *
			  rnd(0.3, 0.7));
	else if(pl->talent - pl->skill < 0.8)
	    pl->skill += ((pl->talent - pl->skill) *
			  rnd(0.2, 0.5));
	else
	    pl->skill += ((pl->talent - pl->skill) *
			  rnd(0.1, 0.4));
    }
    else if(pl->peak_age - pl->age >= 0)
    {
	if(pl->talent - pl->skill < 0.4)
	    pl->skill += ((pl->talent - pl->skill) *
			  rnd(0.2, 0.5));
	else if(pl->talent - pl->skill < 0.8)
	    pl->skill += ((pl->talent - pl->skill) *
			  rnd(0.1, 0.4));
	else
	    pl->skill += ((pl->talent - pl->skill) *
			  rnd(0.1, 0.3));
    }
    else if(pl->age - pl->peak_age < 2)
    {
	pl->skill += (gauss_dist(0.03, 0.03,
				 0.11, 0.11) * rndi(-1, 1));
	if(pl->skill > 9.9)
	    pl->skill = 9.9;
    }
    else
    {
	if(pl->talent - pl->skill < 0.5)
	    pl->skill -= gauss_dist(0.1, 0.15, 0.3, 0.4);
	else
	    pl->skill *=
		gauss_dist(0.8,
			   0.87 + 1 / (pl->age - pl->peak_age + 8),
			   0.99, 0.99);
    }

    /* if the skill's changed, the cskill has to be
       adjusted, too; if the new skill surpasses the estimated
       talent, the latter has to be re-estimated */
    pl->cskill =
	calculate_cskill(*pl);
    
    if(pl->skill > pl->etal)
	pl->etal = estimate_talent(*pl);

    /* adjust value */
    pl->value = assign_value(*pl);

    /* adjust wage only if it increases */
    if(pl->wage < assign_wage(*pl))
	pl->wage = assign_wage(*pl);
}

/* calculate the fitness increase or decrease of a player */
gfloat calculate_new_fitness(player pl, gint increase)
{
    gfloat new_fitness = pl.fitness;

    if(increase == 1)
    {
	if(pl.age < 24)
	    new_fitness += gauss_dist(0.10,0.12,0.25,0.3);
	else if(pl.age > pl.peak_age + 1)
	    new_fitness += gauss_dist(0.05,0.10,0.16,0.3);
	else
	    new_fitness += gauss_dist(0.05,0.10,0.22,0.3);
	
	if(new_fitness > 0.999)
	    new_fitness = 0.999;
    }
    else
    {
	if(pl.age > pl.peak_age + 2)
	    new_fitness -= gauss_dist(0.04,0.04,0.08,0.1);
	else if(pl.age < 23 || pl.age > pl.peak_age + 1)
	    new_fitness -= gauss_dist(0.03,0.03,0.08,0.8);
	else
	    new_fitness -= gauss_dist(0.02,0.02,0.06,0.08);
	
	if(new_fitness < 0)
	    new_fitness = 0;
    }

    return new_fitness;
}

/* find a rndom substitute player who's not injured or
   banned for a given position */
gint
find_substitute(gint team_id, gint position)
{
    gint i;
    gint order[9];

    write_permutation(order, 11, 19);

    for(i=0;i<9;i++)
	if(teams[team_id].players[order[i]].pos == position &&
	   teams[team_id].players[order[i]].cskill > 0)
	    return order[i];
    
    return -1;
}

/* copy player source to player dest */
void
copy_player(player source, player *dest)
{
    strcpy(dest->name, source.name);

    dest->pos = source.pos;
    dest->cpos = source.cpos;
    dest->health = source.health;
    dest->goals = source.goals;
    dest->booked = source.booked;
    dest->games = source.games;
    dest->last_skill_update = source.last_skill_update;    
    dest->age = source.age;
    dest->peak_age = source.peak_age;
    dest->value = source.value;
    dest->wage = source.wage;
    dest->skill = source.skill;
    dest->cskill = source.cskill;
    dest->talent = source.talent;
    dest->etal = source.etal;
    dest->fitness = source.fitness;
    dest->team_id = source.team_id;
}

void
replace_player_by_new(gint team_id, gint player_number)
{
    generate_player(team_id, 1, player_number);
    
    strcpy(teams[team_id].players[player_number].name,
	   player_names[rndi(0,10503)]);

    /* adapt talent and skill to the rest of the team */
    if(rnd(0,1) < 0.5)
	adapt_ability(team_id, player_number);
}

/* swap two players */
void
swap_players(gint team_id1, gint player_number1,
		  gint team_id2, gint player_number2)
{
    gint i, j;
    player swap;

    copy_player(teams[team_id1].players[player_number1],
		&swap);
    copy_player(teams[team_id2].players[player_number2],
		&(teams[team_id1].players[player_number1]));
    copy_player(swap,
		&(teams[team_id2].players[player_number2]));

    teams[team_id1].players[player_number1].cpos =
	get_position_from_structure(team_id1, 0, player_number1, 0);
    teams[team_id2].players[player_number2].cpos =
	get_position_from_structure(team_id2, 0, player_number2, 0);

    teams[team_id1].players[player_number1].cskill =
	calculate_cskill(teams[team_id1].players[player_number1]);
    teams[team_id2].players[player_number2].cskill =
	calculate_cskill(teams[team_id2].players[player_number2]);

    /* update the transferlist if necessary */
    for(i=0;i<20;i++)
	if(transferlist[i].time > 0)
	{
	    if(transferlist[i].team_id == team_id1 &&
	       transferlist[i].player_number ==
	       player_number1)
	    {
		transferlist[i].team_id = team_id2;
		transferlist[i].player_number =
		    player_number2;
	    }
	    else if(transferlist[i].team_id == team_id2 &&
		    transferlist[i].player_number ==
		    player_number2)
	    {
		transferlist[i].team_id = team_id1;
		transferlist[i].player_number =
		    player_number1;
	    }
	}

    /* update penalty shooter */
    if(team_id1 == my_team && team_id2 == my_team &&
       (player_number1 == options[OPT_PENALTY_SHOOTER] ||
	player_number2 == options[OPT_PENALTY_SHOOTER]) )
	options[OPT_PENALTY_SHOOTER] =
	    (player_number1 == options[OPT_PENALTY_SHOOTER]) ?
	    player_number2 : player_number1;

    /* update injury, bookings and goals information */
    if(injuries[0][11] == -1)
	return;
    
    for(i=0;i<50;i++)
    {
	if(i<11)
	{
	    for(j=0;j<2;j++)
		if(team_id1 == injuries[j][11] &&
		   team_id2 == injuries[j][11])
		{
		    if(injuries[j][i] == player_number1)
			injuries[j][i] = player_number2;
		    else if(injuries[j][i] == player_number2)
			injuries[j][i] = player_number1;
		    if(booked[j][i] == player_number1)
			booked[j][i] = player_number2;
		    else if(booked[j][i] == player_number2)
			booked[j][i] = player_number1;
		}
	}

	for(j=0;j<2;j++)
	    if(goals[j][i].team_id == team_id1 &&
	       goals[j][i].scorer == player_number1)
	    {
		goals[j][i].team_id = team_id2;
		goals[j][i].scorer = player_number2;
	    }
	    else if(goals[j][i].team_id == team_id2 &&
		    goals[j][i].scorer == player_number2)
	    {
		goals[j][i].team_id = team_id1;
		goals[j][i].scorer = player_number1;
	    }
    }
}

/* move the player from team1 to team2 and replace
   him by a new one */
void
move_player(gint team1, gint player_number, gint team2)
{
    gint i;

    for(i=0;i<20;i++)
	if(teams[team2].players[i].pos < 0)
	    break;
    
    copy_player(teams[team1].
		players[player_number],
		&(teams[team2].players[i]));

    teams[team2].players[i].cpos =
	get_position_from_structure(team2, 0, i, 0);

    teams[team2].players[i].cskill =
	calculate_cskill(teams[team2].players[i]);

    teams[team2].players[i].team_id = team2;

    teams[team2].players[i].value =
	assign_value(teams[team2].players[i]);
    
    replace_player_by_new(team1, player_number);
}

/* rndomly substitute the goalie of a cpu-team;
   the better goalie is favoured quadratically depending
   on the skills, however. */
void
substitute_goalie(gint team_id)
{
    gfloat bound[2];
    
    if(teams[team_id].players[11].cskill == 0)
	return;

    bound[0] = powf(teams[team_id].players[0].skill, 4) *
	powf(teams[team_id].players[0].fitness, 0.25);
    bound[1] = bound[0] + 
	powf(teams[team_id].players[11].skill, 4) *
	powf(teams[team_id].players[11].fitness, 0.25);

    if(rnd(0,bound[1]) < bound[0])
	return;

    swap_players(team_id, 0, team_id, 11);
}

/* estimate value and wage of the player
   on the transferlist with index idx */
void
estimate_value_wage(gint idx)
{
    gint team_id = transferlist[idx].team_id;
    gint player_number = transferlist[idx].player_number;
    gfloat scout_deviance = 0.02 + (scout % 10) * 0.03;
    
    if(team_id == my_team)
    {
	transferlist[idx].estimates[0] =
	    teams[team_id].players[player_number].value;
	transferlist[idx].estimates[1] =
	    teams[team_id].players[player_number].wage;
	
	return;
    }

    transferlist[idx].estimates[0] = round_integer(
	rnd(teams[team_id].players[player_number].value *
	    1 - scout_deviance,
	    teams[team_id].players[player_number].value *
	    1 + scout_deviance),
	2);
    
    transferlist[idx].estimates[1] = round_integer(
	rnd(teams[team_id].players[player_number].wage *
	    1 - scout_deviance,
	    teams[team_id].players[player_number].wage *
	    1 + scout_deviance),
	2);
}

/* remove a player from a team */
void
remove_player(gint team_id, gint player_number)
{
    gint i;

    for(i=19;i>player_number;i--)
	if(teams[team_id].players[i].pos >= 0)
	{
	    swap_players(team_id, player_number, team_id, i);
	    teams[team_id].players[i].pos = -1;
	    return;
	}

    teams[team_id].players[player_number].pos = -1;
}

/* return 1 if there are injured/banned players
   in the human player's team, 0 otherwise */
gint
unfit_players(void)
{
    gint i;

    for(i=0;i<11;i++)
	if(teams[my_team].players[i].pos >= 0 &&
	   teams[my_team].players[i].health % 10 +
	   teams[my_team].players[i].booked % 10 != 0)
	    return 1;

    return 0;
}

/* fire a player and deduce either a one-time compensation
   or set a counter for paying the wage for some months */
void
fire_player(gboolean one_time)
{
    gint i;
    gint player_number = 
	selected_rows[0];
    gint compensation =
	round_integer(teams[my_team].players[player_number].value * 0.25, 2);

    if(one_time)
    {
	if(compensation > BUDGET)
	{
	    show_popup_window("You haven't got the money.", NULL);	
	    return;
	}

	finances[FIN_MONEY] -= compensation;
	finances[FIN_WAGES] -= compensation;
    }
    else
    {
	for(i = COUNT_OLD_WAGE1; i <= COUNT_OLD_WAGE5; i++)
	    if(counters[i] == 0)
		break;

	if(i > COUNT_OLD_WAGE5)
	{
	    g_print("fire_player: no free old wage counter found\n\n");
	    return;
	}

	counters[i] = teams[my_team].players[player_number].wage * 10 + 32;
    }
    
    remove_player(my_team, player_number);
    selected_rows[0] = -1;
    on_button_back_to_main_clicked(NULL, NULL);
}

/* return the number of players in the human player's team */
gint
players_in_team(void)
{
    gint i;
    gint number_of_players = 0;

    for(i=0;i<20;i++)
	if(teams[my_team].players[i].pos >= 0)
	    number_of_players++;

    return number_of_players;
}

