#include "maths.h"
#include "misc.h"
#include "player.h"
#include "team.h"
#include "transfer.h"

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
    gint rndom = rndi(0, 5);
    gint time;
    gint team_id, player_number;

    while(rndom > 0)
    {
	team_id = (rnd(0, 1) < 0.5) ? 
	    rndi(0, 174) : rndi(0, 113);
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

