/*********************************************************
 * Functions for loading and saving the game             *
 *********************************************************/

#include "game_gui.h"
#include "load_save.h"
#include "misc.h"
#include "xml_read.h"
#include "xml_write.h"

/* find out whether a file ends with '.xml' */
gboolean is_xml_file(gchar *file_name)
{
    gchar buf[SMALL];
    gint i;

    for(i=0;i<4;i++)
	buf[i] = file_name[strlen(file_name) - 1 - i];

    buf[4] = '\0';

    if(strcmp(buf, "lmx.") == 0)
	return TRUE;

    return FALSE;
}

/* check whether a file is a binary or xml
   bygfoot savegame */
gboolean
check_save_game(gchar *file_name)
{
    if(is_xml_file(file_name))
       return check_xml_save(file_name);

    return check_save_game_binary(file_name);
}

/* check whether a binary file is a bygfoot savegame */
gboolean
check_save_game_binary(gchar *file_name)
{
    gint local_my_team;
    gint local_season;
    gint local_week;
    gint local_scout;
    FILE *fil = fopen(file_name, "rb");
    
    if(fil == NULL)
	return FALSE;

    if(options[OPT_XML] == 1)
	return TRUE;

    fread((void*)&local_my_team, sizeof(gint), 1, fil);
    fread((void*)&local_season, sizeof(gint), 1, fil);
    fread((void*)&local_week, sizeof(gint), 1, fil);
    fread((void*)&local_scout, sizeof(gint), 1, fil);

    fclose(fil);

    local_scout = local_scout % 10;

    if(local_scout > 0 && local_scout < 5 &&
       local_week < 50 && local_week > 0 &&
       local_season > 0 &&
       local_my_team > -1 && local_my_team < 178)
	return TRUE;

    return FALSE;
}

void
save_history_best_player(player_stat pl_stat, FILE *fil)
{
    fwrite((void*)&(pl_stat.games), 
	   sizeof(gint), 1, fil);
    fwrite((void*)&(pl_stat.goals), 
	   sizeof(gint), 1, fil);
    fwrite((void*)pl_stat.name, 19, 1, fil);
    fwrite((void*)pl_stat.team_name, 50, 1, fil);
}

void
load_history_best_player(player_stat *pl_stat, FILE *fil)
{
    fread((void*)&(pl_stat->games), 
	   sizeof(gint), 1, fil);
    fread((void*)&(pl_stat->goals), 
	   sizeof(gint), 1, fil);
    fread((void*)&(pl_stat->name), 19, 1, fil);
    fread((void*)&(pl_stat->team_name), 50, 1, fil);
}

void
save_history_element(season_stat *stat, FILE *fil)
{
    gint i;

    fwrite((void*)&(stat->season_number), 
	   sizeof(gint), 1, fil);
    fwrite((void*)&(stat->my_league), 
	   sizeof(gint), 1, fil);
    fwrite((void*)&(stat->my_rank), 
	   sizeof(gint), 1, fil);

    for(i=0;i<STAT_END;i++)
	fwrite((void*)stat->team_names[i], 50, 1, fil);

    for(i=0;i<6;i++)
	save_history_best_player(stat->best_players[i], fil);
}

void
load_history_element(season_stat *stat, FILE *fil)
{
    gint i;

    fread((void*)&(stat->season_number), 
	   sizeof(gint), 1, fil);
    fread((void*)&(stat->my_league), 
	   sizeof(gint), 1, fil);
    fread((void*)&(stat->my_rank), 
	   sizeof(gint), 1, fil);

    for(i=0;i<STAT_END;i++)
	fread((void*)&(stat->team_names[i]), 50, 1, fil);

    for(i=0;i<6;i++)
	load_history_best_player(&(stat->best_players[i]), fil);
}

void
save_history(FILE *fil)
{
    gint i;
    gint nr_of_histories = 0;
    season_stat *stat = history;

    if(history == NULL)
    {
	fwrite((void*)&nr_of_histories,
	       sizeof(gint), 1, fil);
	return;
    }

    nr_of_histories = season - 1;
    fwrite((void*)&nr_of_histories,
	   sizeof(gint), 1, fil);
    
    for(i=0;i<nr_of_histories;i++)
    {
	save_history_element(stat, fil);
	stat = stat->next;
    }
}

void
load_history(FILE *fil)
{
    gint i;
    gint nr_of_histories = 0;
    season_stat *stat;

    free_history();

    fread((void*)&nr_of_histories,
	  sizeof(gint), 1, fil);
    
    if(nr_of_histories == 0)
	return;

    for(i=0;i<nr_of_histories;i++)
    {
	stat = get_new_stat();
	load_history_element(stat, fil);
    }
}

void
save_fixtures(FILE *fil)
{
    gint i, j;

    for(i=0;i<FIX_END;i++)
    {
	fwrite((void*)&fixtures[i].type,
	       sizeof(gint), 1, fil);
	fwrite((void*)&fixtures[i].week_number,
	       sizeof(gint), 1, fil);
	
	for(j=0;j<2;j++)
	{
	    fwrite((void*)&fixtures[i].team_id[j],
		   sizeof(gint), 1, fil);
	    fwrite((void*)&fixtures[i].result[j][0],
		   sizeof(gint), 1, fil);
	    fwrite((void*)&fixtures[i].result[j][1],
		   sizeof(gint), 1, fil);
	    fwrite((void*)&fixtures[i].result[j][2],
		   sizeof(gint), 1, fil);
	}
    }
}

void
load_fixtures(FILE *fil)
{
    gint i, j;

    for(i=0;i<FIX_END;i++)
    {
	fread((void*)&fixtures[i].type,
	       sizeof(gint), 1, fil);
	fread((void*)&fixtures[i].week_number,
	       sizeof(gint), 1, fil);
	
	for(j=0;j<2;j++)
	{
	    fread((void*)&fixtures[i].team_id[j],
		   sizeof(gint), 1, fil);
	    fread((void*)&fixtures[i].result[j][0],
		   sizeof(gint), 1, fil);
	    fread((void*)&fixtures[i].result[j][1],
		   sizeof(gint), 1, fil);
	    fread((void*)&fixtures[i].result[j][2],
		   sizeof(gint), 1, fil);
	}
    }
}

void
save_player(gint team_id, gint player_number, FILE *fil)
{
    fwrite((void*)&teams[team_id].players[player_number].name,
	   19, 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].pos,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].cpos,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].health,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].booked,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].goals,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].games,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].value,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].wage,
	   sizeof(gint), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].team_id,
	   sizeof(gint), 1, fil);

    fwrite((void*)&teams[team_id].players[player_number].skill,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].cskill,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].talent,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].etal,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].fitness,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].
	   players[player_number].last_skill_update,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].age,
	   sizeof(gfloat), 1, fil);
    fwrite((void*)&teams[team_id].players[player_number].peak_age,
	   sizeof(gfloat), 1, fil);    
}

void
load_player(gint team_id, gint player_number, FILE *fil)
{
    fread((void*)&(teams[team_id].
		   players[player_number].name), 19, 1, fil);
    fread((void*)&teams[team_id].players[player_number].pos,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].cpos,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].health,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].booked,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].goals,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].games,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].value,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].wage,
	  sizeof(gint), 1, fil);
    fread((void*)&teams[team_id].players[player_number].team_id,
	  sizeof(gint), 1, fil);

    fread((void*)&teams[team_id].players[player_number].skill,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].cskill,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].talent,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].etal,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].fitness,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].last_skill_update,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].age,
	  sizeof(gfloat), 1, fil);
    fread((void*)&teams[team_id].players[player_number].peak_age,
	  sizeof(gfloat), 1, fil);    
}

void
save_teams(FILE *fil)
{
    gint i, j;

    for(i=0;i<178;i++)
    {
	fwrite((void*)&teams[i].name, 50, 1, fil);
	fwrite((void*)&teams[i].style, sizeof(gint), 1, fil);
	fwrite((void*)&teams[i].structure, sizeof(gint), 1, fil);
	fwrite((void*)&teams[i].id, sizeof(gint), 1, fil);
	for(j=0;j<RES_END;j++)
	    fwrite((void*)&teams[i].results[j], sizeof(gint), 1, fil);
	
	for(j=0;j<20;j++)
	    save_player(i, j, fil);
    }
}

void
load_teams(FILE *fil)
{
    gint i, j;

    for(i=0;i<178;i++)
    {
	fread((void*)&(teams[i].name), 50, 1, fil);
	fread((void*)&teams[i].style, sizeof(gint), 1, fil);
	fread((void*)&teams[i].structure, sizeof(gint), 1, fil);
	fread((void*)&teams[i].id, sizeof(gint), 1, fil);
	for(j=0;j<RES_END;j++)
	    fread((void*)&teams[i].results[j], sizeof(gint), 1, fil);
	
	for(j=0;j<20;j++)
	    load_player(i, j, fil);
    }
}

void
save_stadiums(FILE *fil)
{
    gint i;

    for(i=0;i<178;i++)
    {
	fwrite((void*)&stadiums[i].capacity, sizeof(gint), 1, fil);
	fwrite((void*)&stadiums[i].average_attendance, 
	       sizeof(gint), 1, fil);
	fwrite((void*)&stadiums[i].games, sizeof(gint), 1, fil);
	fwrite((void*)&stadiums[i].safety, sizeof(gfloat), 1, fil);
    }
}

void
load_stadiums(FILE *fil)
{
    gint i;

    for(i=0;i<178;i++)
    {
	fread((void*)&stadiums[i].capacity, sizeof(gint), 1, fil);
	fread((void*)&stadiums[i].average_attendance, 
	       sizeof(gint), 1, fil);
	fread((void*)&stadiums[i].games, sizeof(gint), 1, fil);
	fread((void*)&stadiums[i].safety, sizeof(gfloat), 1, fil);
    }
}

void
save_transfers(FILE *fil)
{
    gint i;

    for(i=0;i<20;i++)
    {
	fwrite((void*)&transferlist[i].time,
	       sizeof(gint), 1, fil);
	fwrite((void*)&transferlist[i].team_id, 
	       sizeof(gint), 1, fil);
	fwrite((void*)&transferlist[i].player_number, 
	       sizeof(gint), 1, fil);
	fwrite((void*)&transferlist[i].estimates[0], 
	       sizeof(gint), 1, fil);
	fwrite((void*)&transferlist[i].estimates[1], 
	       sizeof(gint), 1, fil);
    }
}

void
load_transfers(FILE *fil)
{
    gint i;

    for(i=0;i<20;i++)
    {
	fread((void*)&transferlist[i].time,
	       sizeof(gint), 1, fil);
	fread((void*)&transferlist[i].team_id, 
	       sizeof(gint), 1, fil);
	fread((void*)&transferlist[i].player_number, 
	       sizeof(gint), 1, fil);
	fread((void*)&transferlist[i].estimates[0], 
	       sizeof(gint), 1, fil);
	fread((void*)&transferlist[i].estimates[1], 
	       sizeof(gint), 1, fil);
    }
}

void
save_options(FILE *fil)
{
    gint i, j;

    fwrite((void*)&my_team, sizeof(gint), 1, fil);
    fwrite((void*)&season, sizeof(gint), 1, fil);
    fwrite((void*)&week, sizeof(gint), 1, fil);
    fwrite((void*)&scout, sizeof(gint), 1, fil);
    fwrite((void*)&physio, sizeof(gint), 1, fil);
    fwrite((void*)&font_name, SMALL, 1, fil);
    fwrite((void*)&country_file_name, SMALL, 1, fil);

    for(i=0;i<130;i++)
    {
	fwrite((void*)&rank[i], sizeof(gint), 1, fil);
	fwrite((void*)&rank_ids[i], sizeof(gint), 1, fil);

	if(i<2)
	{
	    fwrite((void*)&stadium_facts[0][i], sizeof(gint), 1, fil);
	    fwrite((void*)&stadium_facts[1][i], sizeof(gint), 1, fil);
	}
	if(i<OPT_END)
	    fwrite((void*)&options[i], sizeof(gint), 1, fil);
	if(i<COUNT_END)
	    fwrite((void*)&counters[i], sizeof(gint), 1, fil);
	if(i<FIN_END)
	    fwrite((void*)&finances[i], sizeof(gint), 1, fil);
	if(i<LEAGUE_NAMES_END)
	    fwrite((void*)league_names[i], 50, 1, fil);
	if(i<12)
	{
	    for(j=0;j<2;j++)
	    {
		fwrite((void*)&injuries[j][i], sizeof(gint), 1, fil);
		fwrite((void*)&booked[j][i], sizeof(gint), 1, fil);
	    }
	}
	if(i<50)
	{
	    for(j=0;j<2;j++)
	    {
		fwrite((void*)&(goals[j][i].minute), sizeof(gint), 1, fil);
		fwrite((void*)&(goals[j][i].team_id), sizeof(gint), 1, fil);
		fwrite((void*)&(goals[j][i].scorer), sizeof(gint), 1, fil);
		fwrite((void*)&(goals[j][i].time), sizeof(gint), 1, fil);
		fwrite((void*)&(goals[j][i].type), sizeof(gint), 1, fil);
	    }
	}
    }
}

void
load_options(FILE *fil)
{
    gint i, j;

    fread((void*)&my_team, sizeof(gint), 1, fil);
    fread((void*)&season, sizeof(gint), 1, fil);
    fread((void*)&week, sizeof(gint), 1, fil);
    fread((void*)&scout, sizeof(gint), 1, fil);
    fread((void*)&physio, sizeof(gint), 1, fil);
    fread((void*)&font_name, SMALL, 1, fil);
    fread((void*)&country_file_name, SMALL, 1, fil);

    for(i=0;i<130;i++)
    {
	fread((void*)&rank[i], sizeof(gint), 1, fil);
	fread((void*)&rank_ids[i], sizeof(gint), 1, fil);

	if(i<2)
	{
	    fread((void*)&stadium_facts[0][i], sizeof(gint), 1, fil);
	    fread((void*)&stadium_facts[1][i], sizeof(gint), 1, fil);
	}
	if(i<OPT_END)
	    fread((void*)&options[i], sizeof(gint), 1, fil);
	if(i<COUNT_END)
	    fread((void*)&counters[i], sizeof(gint), 1, fil);
	if(i<FIN_END)
	    fread((void*)&finances[i], sizeof(gint), 1, fil);
	if(i<LEAGUE_NAMES_END)
	    fread((void*)&(league_names[i]), 50, 1, fil);
	if(i<12)
	{
	    for(j=0;j<2;j++)
	    {
		fread((void*)&injuries[j][i], sizeof(gint), 1, fil);
		fread((void*)&booked[j][i], sizeof(gint), 1, fil);
	    }
	}
	if(i<50)
	{
	    for(j=0;j<2;j++)
	    {
		fread((void*)&(goals[j][i].minute), sizeof(gint), 1, fil);
		fread((void*)&(goals[j][i].team_id), sizeof(gint), 1, fil);
		fread((void*)&(goals[j][i].scorer), sizeof(gint), 1, fil);
		fread((void*)&(goals[j][i].time), sizeof(gint), 1, fil);
		fread((void*)&(goals[j][i].type), sizeof(gint), 1, fil);
	    }
	}
    }
}

void
save_game(gchar *file_name)
{
    FILE *fil;
    gchar local_file[SMALL];

    sprintf(local_file, "%s", file_name);

    if(options[OPT_XML] == 1)
	if(!is_xml_file(file_name))
	    strcat(local_file, ".xml");

    fil = fopen(local_file, "ab");
    if(fil == NULL)
    {
	g_print("save_game: could not open file: %s\n", local_file);
	return;
    }

    fclose(fil);

    g_string_printf(save_file, "%s", local_file);

    if(is_xml_file(local_file))
    {
	write_xml_save(local_file);
	return;
    }
    
    fil = fopen(local_file, "wb");
    save_options(fil);
    save_transfers(fil);
    save_stadiums(fil);
    save_teams(fil);
    save_fixtures(fil);
    save_history(fil);

    fclose(fil);
}

void
load_game(gchar *file_name)
{
    FILE *fil;
    gchar local_file[SMALL];

    sprintf(local_file, "%s", file_name);

    fil = fopen(local_file, "rb");

    if(fil == NULL)
    {
	g_print("load_game: could not open file: %s\n", local_file);
	return;
    }

    g_string_printf(save_file, "%s", local_file);

    if(is_xml_file(local_file))
    {
	fclose(fil);    
	read_xml_save(local_file);
	return;
    }
    
    load_options(fil);
    load_transfers(fil);
    load_stadiums(fil);
    load_teams(fil);
    load_fixtures(fil);
    load_history(fil);

    fclose(fil);
}
