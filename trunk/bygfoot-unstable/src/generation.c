/**************************************************
 * Functions that generate teams and players      *
 * at the beginning of the game or of a new       *
 * season                                         *
 **************************************************/

#include "game_gui.h"
#include "generation.h"
#include "gui.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "team.h"
#include "text_files.h"
#include "window.h"

/* name all players in each team between first_team and last_team */
void
give_player_names(gint first_team, gint last_team)
{
    gint i, k, name_start;
    
    for(i = first_team; i < last_team; i++)
    {
	name_start = rndi(0, 10483);
	for(k=0;k<20;k++)
	    if(my_team < 114 || i != my_team)
		strcpy(teams[i].players[k].name,
		       player_names[name_start + k]);
    }
}

/* choose names of uefa cup participants */
void
choose_european_names_uefa(gchar all_names[][50],
				gchar european_names[][50])
{
    gint k = 30;
    gint i, order[400];

    write_permutation(order, 114, 499);

    i = 0;
    while(k != 59)
    {
	/* name should not be on the list already */
	if( (strtol(all_names[order[i]], NULL, 10) == 0 ||
	     strlen(all_names[order[i]]) > 2) &&
	    is_in_list_char(european_names, 0,
			    59, all_names[order[i]]) == 0 )
	{
	    strcpy(european_names[k], all_names[order[i]]);
	    k++;
	}
	
	i++;
    }   
}

/* choose a team name from a given country which isn't
   yet on the list */
void
choose_european_name(gchar all_names[][50],
			  gint country, gint name_idx,
			  gchar european_names[][50])
{
    gint i, j;
    gint bound[2];
    gchar name[50];

    strcpy(name, "0");
    
    for(i=0;i<2;i++)
	for(j=0;j<231;j++)
	    if(strtol(all_names[j], NULL, 10) == country + i &&
	       strlen(all_names[j]) < 3)
		bound[i] = j;
        
    while(is_in_list_char(european_names, 0, 58, name) == 1)
	strcpy(name,
	       all_names[rndi(bound[0] + 1, bound[1] - 1)]);
    
    strcpy(european_names[name_idx], name);
}

/* choose the names for the european
   teams from the names array; this ensures
   that each season there are different teams
   participating in the european cups */
void
choose_european_names(gchar european_names[][50], gchar all_names[][50])
{
    gint i;
    gint order[15];
    gchar ordered_names[15][50];

    for(i=0;i<59;i++)
	strcpy(european_names[i], "0");

    /* choose champions and cup winners */
    for(i=0;i<15;i++)
    {
	choose_european_name(all_names, i + 1, i,
			     european_names);
	choose_european_name(all_names, i + 1, i + 15,
			     european_names);
    }

    /* choose uefa cup participants */
    choose_european_names_uefa(all_names, european_names);

    write_permutation(order, 0, 14);

    /* make CL groups more rndom */
    for(i=0;i<15;i++)
	strcpy(ordered_names[i], european_names[i]);
    for(i=0;i<15;i++)
	strcpy(european_names[i], ordered_names[order[i]]);
}

/* generate the twenty players of a team */
void
generate_players(gint team_id)
{
    gint i;
    
    /* skills will be multiplied by this factor
       to ensure that some teams are really better
       than others */
    gfloat team_factor = rnd(0.9, 1.1);

    for(i=0;i<20;i++)
	generate_player(team_id, team_factor, i);
}

/* fill in data for a team stadium */
void
fill_in_stadium(gint team_id)
{
    stadiums[team_id].games = 0;
    stadiums[team_id].safety = gauss_dist(0.8, 0.8, 1, 1);
    
    /* capacity depends on the team's league */
    switch(get_league_from_id(team_id))
    {
        default:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(25000,35000,60000,80000));
            break;
        case 2:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(12000,14000,30000,45000));
            break;
        case 3:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(7000,9000,20000,18000));
            break;
        case 4:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(4000,7000,15000,10000));
            break;
        case 5:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(3000,4000,10000,9000));
            break;
        case 6:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(60000,60000,100000,110000));
            break;
        case 7:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(50000,50000,80000,80000));
            break;
        case 8:
            stadiums[team_id].capacity =
		(gint)rint(gauss_dist(30000,35000,70000,80000));
            break;
    }

    stadiums[team_id].capacity = 
	round_integer(stadiums[team_id].capacity, 2);

    stadiums[team_id].average_attendance = (gint)rint(
	gauss_dist(0.5 * (gfloat)stadiums[team_id].capacity,
		   0.5 * (gfloat)stadiums[team_id].capacity,
		   (gfloat)stadiums[team_id].capacity,
		   (gfloat)stadiums[team_id].capacity) );
}

/* read team names from a file */
void
give_teams_names(gboolean only_euro)
{
    gint i, j = 0;
    gchar team_names[500][50];
    gchar european_names[59][50];
    FILE *fil = fopen(country_file_name, "r");

    if(fil == NULL)
    {
	g_print("give_teams_names: could not open country_file %s\n\n",
		country_file_name);
	return;
    }
    fclose(fil);

    /* this is for the 'choose_european_names' function which
       will ignore the names consisting of a '1' */
    for(i = 0; i < 500; i++)
	strcpy(team_names[i], "1");
    
    get_names(country_file_name, team_names);

    /* non-euro-teams */
    if(!only_euro)
	for(i=0;i<114;i++)
	    strcpy(teams[i].name, team_names[i]);
    
    /* euro-teams */
    choose_european_names(european_names, team_names);

    for(i=115;i<175;i++)
	if(i != 130 && i != my_team)
	{
	    strcpy(teams[i].name, european_names[j]);
	    j++;
	}
}

/* fill in all the data for a team: name, players,
   stadium etc. */
void
fill_in_team(gint team_id)
{
    gint i;

    for(i=0;i<RES_END;i++)
	teams[team_id].results[i] = 0;

    teams[team_id].id = team_id;
    
    teams[team_id].structure =  
	assign_playing_structure();

    teams[team_id].style =
	assign_playing_style();
    
    generate_players(team_id);
}

/* generate the european teams participating in
   champions' league etc. */
void
fill_in_euro_teams(void)
{
    gint i;
    
    give_player_names(114, 178);
    give_teams_names(TRUE);

    /* generate teams; team 114 is the english champion,
       130 the english cup winner and 175-177 the english
       uefa cup participants, so we don't generate them */
    for(i=115;i<175;i++)
	if(i != 130 && i != my_team)
	{
	    fill_in_stadium(i);
	    fill_in_team(i);
	}
}

/* fill in non-euro-teams */
void
fill_in_teams(void)
{
    gint i;

    /* give all players in the game names
       so that we only open the player names file
       twice (again when filling in the european
       teams) */
    give_player_names(0, 114);        

    for(i=0;i<114;i++)
    {
	fill_in_stadium(i);
	fill_in_team(i);
    }
}

void
give_leagues_names(void)
{
    gint j = 0;
    gint linenr = 0;
    gchar trash[BIG];
    gchar buf[SMALL];
    FILE *fil = fopen(country_file_name, "r");

    /* find the appropriate place in the file */
    while(1)
    {
	fscanf(fil, "%[^\n]", trash);
	linenr++;
	if(strcmp(trash, "1000") == 0)
	    break;
	fscanf(fil, "%[\n]*", trash);
    }

    while(1)
    {
	fscanf(fil, "%[\n]*", trash);

	fscanf(fil, "%[^\n]", trash);
	linenr++;
	if(strcmp(trash, "2000") == 0 || j == LEAGUE_NAMES_END)
	    break;
	
	if(trash[0] != '#')
	{
	    if(strlen(trash) < 50)
	    {
		strcpy(league_names[j], trash);
		j++;
	    }
	    else
	    {
		sprintf(buf,"*** The lines in '%s' which aren't comment lines (leading '#') must have length < 50. ***\nLine %d is too long! ***\n\n",
			country_file_name, linenr);
		g_print("%s", buf);
	    }
	}
    }
    
    fclose(fil);
    return;
}

/* select team and cup names according to the country given as
   an integer or a string */
void
country_names(gint country_number, const gchar *country_filename)
{
    gchar filename[SMALL];
    gchar buf[SMALL];
    gchar *buf2;
    FILE *fil;
 
    if(country_filename != NULL &&
       strlen(country_filename) > 0)
    {
	sprintf(buf, "%s", country_filename);
	buf2 = find_support_file(buf);

	if(buf2 != NULL)
	{
	    strcpy(filename, buf2);
	    g_free(buf2);
	}
	else
	    strcpy(filename, country_filename);

	fil = fopen(filename, "r");
	if(fil == NULL)
	{
	    strcpy(filename, country_filename);
	    fil = fopen(country_filename, "r");
	}
    }
    else
    {
	text_file_number_to_char(country_number, filename, TRUE);
	fil = fopen(filename, "r");
    }

    if(fil == NULL)
    {
	g_print("*** Could not open file: %s .", filename);
	sprintf(buf, "Could not open file: %s .", filename);
	show_popup_window(buf, NULL);
	return;
    }

    fclose(fil);   

    strcpy(country_file_name, filename);

    /* name teams */
    give_teams_names(FALSE);

    /* name leagues and cups */
    give_leagues_names();
}

/* generate teams */
void
generate_teams(void)
{
    gint i;
    gint order[20];

    /* select team and cup names according to country;
       initially england; if the first team name isn't empty, 
       the user's specified a file as commandline option */
    if(strlen(teams[0].name) == 0)
	country_names(0, NULL);

    fill_in_teams();
    fill_in_euro_teams();

    /* copy rndom english teams into the 
       european cups in the first season */
    write_permutation(order, 0, 19);
    /* CL */
    teams[114].id = order[0];
    strcpy(teams[114].name, "buh!");
    /* CWC */
    teams[130].id = order[1];
    strcpy(teams[130].name, "buh!");
    /* UEFA */
    for(i=0;i<3;i++)
    {
	teams[175 + i].id = order[i + 2];
	strcpy(teams[175 + i].name, "buh!");
    }
}
