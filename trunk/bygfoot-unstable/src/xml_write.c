#include <assert.h>

#include "xml_write.h"

#define INDENT0 ""
#define INDENT1 "\t"
#define INDENT2 "\t\t"
#define INDENT3 "\t\t\t"
#define INDENT4 "\t\t\t\t"

#define INDENT_BYGFOOT INDENT0

#define INDENT_OPTIONS INDENT1
#define INDENT_IN_OPTIONS INDENT2
#define INDENT_IN_IN_OPTIONS INDENT3

#define INDENT_TRANSFER INDENT1
#define INDENT_IN_TRANSFER INDENT2

#define INDENT_TEAM INDENT1
#define INDENT_IN_TEAM INDENT2

#define INDENT_PLAYER INDENT2
#define INDENT_IN_PLAYER INDENT3

#define INDENT_STADIUM INDENT1
#define INDENT_IN_STADIUM INDENT2

#define INDENT_HISTORY INDENT1
#define INDENT_IN_HISTORY INDENT2
#define INDENT_HISTORY_ELEMENT INDENT2
#define INDENT_IN_HISTORY_ELEMENT INDENT3
#define INDENT_HISTORY_BEST_PLAYER INDENT3
#define INDENT_IN_HISTORY_BEST_PLAYER INDENT4

#define INDENT_FIXTURES INDENT1
#define INDENT_FIXTURE INDENT2
#define INDENT_IN_FIXTURE INDENT3
#define INDENT_IN_IN_FIXTURE INDENT4

#define INDENT_GOAL INDENT3
#define INDENT_IN_GOAL INDENT4

#define TRANSFER_SIZE 20
#define PLAYERS_NUMBER 20
#define TEAMS_NUMBER 178
#define STADIUMS_NUMBER TEAMS_NUMBER
#define MAGIC_NUMBER_130 130
#define MAGIC_NUMBER_12 12
#define MAGIC_NUMBER_50 50

void
write_xml_save(char * file_name)
{
	/* 	TODO: tests on file_name, and set a default name is file_name is not
		a good file name. */
		
	FILE * xml_file = fopen(file_name, "w");
	
	write_xml_init(xml_file);
	write_xml_options(xml_file);
	write_xml_transfers(xml_file);
	write_xml_stadiums(xml_file);
	write_xml_teams(xml_file);
	write_xml_history(xml_file);
	write_xml_fixtures(xml_file);
	write_xml_end(xml_file);	
	
	fclose(xml_file);
}

void
write_xml_init(FILE * xml_file)
{
	/* TODO : an assert on the xml_file. I don't remember how to test if the FILE ptr is valid */
	fprintf(xml_file, "<?xml version=\"1.0\"?>\n");
	
	/* I'm not sure making a doctype is necessary. This file will be used only in bygfoot */
	/* and a DOCTYPE will be only a xml parsing test. */
	/* fprintf(xml_file, "<!DOCTYPE >\n"); */
	fprintf(xml_file, "%s%s\n", INDENT_BYGFOOT, "<bygfoot-save>");
}

void 
write_xml_end(FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_BYGFOOT, "</bygfoot-save>");
}

void 
write_xml_options(FILE * xml_file)
{    
	fprintf(xml_file, "%s%s\n", INDENT_OPTIONS, "<options>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_OPTIONS, "<my_team>", my_team, "</my_team>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_OPTIONS, "<season>", season, "</season>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_OPTIONS, "<week>", week, "</week>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_OPTIONS, "<scout>", scout, "</scout>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_OPTIONS, "<physio>", physio, "</physio>");
	fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_OPTIONS, "<font_name>", font_name, "</font_name>");
	fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_OPTIONS, "<country_file_name>", country_file_name, "</country_file_name>");
	
	int magic_number;
	for (magic_number = 0; magic_number < MAGIC_NUMBER_130; magic_number ++)
	{
		fprintf(xml_file, "%s%s\n", INDENT_IN_IN_OPTIONS, "<memory>");
		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<rank>", rank[magic_number], "</rank>");
		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<rank_id>", rank_ids[magic_number], "</rank_id>");
		if (magic_number < 2)
		{
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<stadium_fact0>", stadium_facts[0][magic_number], "</stadium_fact0>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<stadium_fact1>", stadium_facts[1][magic_number], "</stadium_fact1>");
		}
		if (magic_number < OPT_END)
		{
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<options>", options[magic_number], "</options>");
		}
		if (magic_number < COUNT_END)
		{
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<counter>", counters[magic_number], "</counter>");
		}
		if (magic_number < FIN_END)
		{
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<finances>", finances[magic_number], "</finances>");
		}
		if (magic_number < LEAGUE_NAMES_END)
		{
			fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_IN_OPTIONS, "<league_name>", league_names[magic_number], "</league_name>");
		}
		if (magic_number < MAGIC_NUMBER_12)
		{
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<injury0>", injuries[0][magic_number], "</injury0>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<booked0>", booked[0][magic_number], "</booked0>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<injury1>", injuries[1][magic_number], "</injury1>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_OPTIONS, "<booked1>", booked[1][magic_number], "</booked1>");
		}
		if (magic_number < MAGIC_NUMBER_50)
		{
			write_xml_goal(&goals[0][magic_number], xml_file);
			write_xml_goal(&goals[1][magic_number], xml_file);
		}
		fprintf(xml_file, "%s%s\n", INDENT_IN_OPTIONS, "</memory>");
	}
	fprintf(xml_file, "%s%s\n", INDENT_OPTIONS, "</options>");
}

void
write_xml_goal(goal * gl, FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_GOAL, "<goal>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_GOAL, "<minute>", gl->minute, "</minute>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_GOAL, "<team_id>", gl->team_id, "</team_id>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_GOAL, "<scorer>", gl->scorer, "</scorer>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_GOAL, "<time>", gl->time, "</time>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_GOAL, "<type>", gl->type, "</type>");
	fprintf(xml_file, "%s%s\n", INDENT_GOAL, "</goal>");
}

void
write_xml_history_best_player(player_stat * pl_stat, FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_HISTORY_BEST_PLAYER, "<history_best_player>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY_BEST_PLAYER, "<games>", pl_stat->games, "</games>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY_BEST_PLAYER, "<goals>", pl_stat->goals, "</goals>");
	fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_HISTORY_BEST_PLAYER, "<name>", pl_stat->name, "</name>");
	fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_HISTORY_BEST_PLAYER, "<team_name>", pl_stat->team_name, "</team_name>");
	fprintf(xml_file, "%s%s\n", INDENT_HISTORY_BEST_PLAYER, "</history_best_player>");
}

void
write_xml_history_element(season_stat * stat, FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_HISTORY_ELEMENT, "<history_element>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY_ELEMENT, "<season_number>", stat->season_number, "</season_number>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY_ELEMENT, "<my_league>", stat->my_league, "</my_league>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY_ELEMENT, "<my_rank>", stat->my_rank, "</my_rank>");

	gint i;
    for(i=0;i<STAT_END;i++)
		fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_HISTORY_ELEMENT, "<team_name>", stat->team_names[i], "</team_name>");
	
	for(i=0;i<6;i++)
		write_xml_history_best_player(&(stat->best_players[i]), xml_file);
}

void
write_xml_history(FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_HISTORY, "<history>");
	
    if(history == NULL)
    {
		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY, "<nr_of_histories>", 0, "</nr_of_histories>");
    } else
	{
	    gint i;
		gint nr_of_histories = season-1;
		season_stat *stat = history;

		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_HISTORY, "<nr_of_histories>", nr_of_histories, "</nr_of_histories>");
    
		for(i=0;i<nr_of_histories;i++)
		{
			write_xml_history_element(stat, xml_file);
			stat = stat->next;
		}
	}
	fprintf(xml_file, "%s%s\n", INDENT_HISTORY, "</history>");
}

void 
write_xml_fixtures(FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_FIXTURES, "<fixtures>");

	gint i, j;

    for(i=0;i<FIX_END;i++)
    {
		fprintf(xml_file, "%s%s\n", INDENT_FIXTURE, "<fixture>");
		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_FIXTURE, "<type>", fixtures[i].type, "</type>");
		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_FIXTURE, "<week>", fixtures[i].week_number, "</week>");

		for(j=0;j<2;j++)
		{
			fprintf(xml_file, "%s%s\n", INDENT_IN_FIXTURE, "<result>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_FIXTURE, "<team_id>", fixtures[i].team_id[j], "</team_id>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_FIXTURE, "<result0>", fixtures[i].result[j][0], "</result0>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_FIXTURE, "<result1>", fixtures[i].result[j][1], "</result1>");
			fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_IN_FIXTURE, "<result2>", fixtures[i].result[j][2], "</result2>");
			fprintf(xml_file, "%s%s\n", INDENT_IN_FIXTURE, "</result>");
		}
		fprintf(xml_file, "%s%s\n", INDENT_FIXTURE, "</fixture>");
	}
	fprintf(xml_file, "%s%s\n", INDENT_FIXTURES, "</fixtures>");    
}

void 
write_xml_player(player * pl, FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_PLAYER, "<player>");
	fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_PLAYER, "<name>", pl->name, "</name>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<pos>", pl->pos, "</pos>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<cpos>", pl->cpos, "</cpos>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<health>", pl->health, "</health>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<goals>", pl->goals, "</goals>");	
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<booked>", pl->booked, "</booked>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<games>", pl->games, "</games>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<value>", pl->value, "</value>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<wage>", pl->wage, "</wage>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_PLAYER, "<team_id>", pl->team_id, "</team_id>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<skill>", pl->skill, "</skill>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<cskill>", pl->cskill, "</cskill>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<talent>", pl->talent, "</talent>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<etal>", pl->etal, "</etal>");
	fprintf(xml_file, "%s%s%.2f%s\n", INDENT_IN_PLAYER, "<fitness>", pl->fitness, "</fitness>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<last_skill_update>", pl->last_skill_update, "</last_skill_update>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<age>", pl->age, "</age>");
	fprintf(xml_file, "%s%s%.1f%s\n", INDENT_IN_PLAYER, "<peak_age>", pl->peak_age, "</peak_age>");   
	fprintf(xml_file, "%s%s\n", INDENT_PLAYER, "</player>");
}

void
write_xml_teams(FILE * xml_file)
{
	gint t;
	for (t = 0; t < TEAMS_NUMBER; t++)
	{
		write_xml_team(&teams[t], xml_file);
	}
}

void 
write_xml_team(team * tm, FILE * xml_file)
{
	gint j, p;
	fprintf(xml_file, "%s%s\n", INDENT_TEAM, "<team>");
	fprintf(xml_file, "%s%s%s%s\n", INDENT_IN_TEAM, "<name>", tm->name, "</name>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TEAM, "<style>", tm->style, "</style>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TEAM, "<structure>", tm->structure, "</structure>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TEAM, "<id>", tm->id, "</id>");
	
	for(j=0;j<RES_END;j++)
		fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TEAM, "<result>", tm->results[j], "</result>");
	
	for(p=0; p<PLAYERS_NUMBER; p++)
		write_xml_player(&(tm->players[p]), xml_file);
		
	fprintf(xml_file, "%s%s\n", INDENT_TEAM, "</team>");
}

void
write_xml_stadiums(FILE * xml_file)
{
	gint s;
	for (s = 0; s < STADIUMS_NUMBER; s++)
	{
		write_xml_stadium(&stadiums[s], xml_file);
	}
}

void 
write_xml_stadium(stadium * st, FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_STADIUM, "<stadium>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_STADIUM, "<capacity>", st->capacity, "</capacity>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_STADIUM, "<average_attendance>", st->average_attendance, "</average_attendance>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_STADIUM, "<games>", st->games, "</games>");
	fprintf(xml_file, "%s%s%.2f%s\n", INDENT_IN_STADIUM, "<safety>", st->safety, "</safety>");
	fprintf(xml_file, "%s%s\n", INDENT_STADIUM, "</stadium>");
}

void
write_xml_transfers(FILE * xml_file)
{
	gint t;
		
	for (t = 0; t < TRANSFER_SIZE; t++)
	{
		write_xml_transfer(&transferlist[t], xml_file);
	}
}
	
void
write_xml_transfer(transfer * tf, FILE * xml_file)
{
	fprintf(xml_file, "%s%s\n", INDENT_TRANSFER, "<transfer>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TRANSFER, "<time>", tf->time, "</time>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TRANSFER, "<team_id>", tf->team_id, "</team_id>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TRANSFER, "<player_number>", tf->player_number, "</player_number>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TRANSFER, "<estimates0>", tf->estimates[0], "</estimates0>");
	fprintf(xml_file, "%s%s%d%s\n", INDENT_IN_TRANSFER, "<estimates1>", tf->estimates[1], "</estimates1>");
	fprintf(xml_file, "%s%s\n", INDENT_TRANSFER, "</transfer>");
}
