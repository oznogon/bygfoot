#include "finance.h"
#include "fixture.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "team.h"
#include "window.h"

/* return the costs for improving stadium safety or
   capacity */
gint
stadium_improve_costs(gint safety, gint build)
{
    gint league = get_league_from_id(my_team);
    gint cost_unit = (-75000 + safety * 60000) *
	((6 + (league > 5) * 8) - league);
    
    if(build < 0)
	return cost_unit;
    else
	return
	    round_integer((build + 1) *
			  (1 - 0.05 * build) *
			  cost_unit, 2);
}

/* return either the maximum amount of money the bank
   grants a team or the human player's drawing credit;
   both values depend on the players' values
   and the stadium */
gint
calculate_max_loan_credit(gint credit)
{
    gint i;
    gint max = 0;
    gfloat drawing_credit_factor = 1 - credit * 0.75;

    for(i=0;i<20;i++)
	if(teams[my_team].players[i].pos >= 0)
	    max += (gint)((gfloat)teams[my_team].players[i].value / 10);

    max += stadiums[my_team].capacity * 30;

    return round_integer(max * drawing_credit_factor, 2);
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
