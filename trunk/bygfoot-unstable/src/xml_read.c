#include <assert.h>

#include "xml_read.h"

#define MAX_TEAMS 178
#define MIN_TEAMS 178
#define MAX_STADIUMS 178
#define MIN_STADIUMS 178
#define MAX_PLAYERS 20
#define MIN_PLAYERS 20
#define MAX_TRANSFERS 20
#define MIN_TRANSFERS 20
#define MAX_RESULTS 12
#define MIN_RESULTS 12
#define MAX_FIXTURES FIX_END
#define MAGIC_NUMBER_130 130
#define MAGIC_NUMBER_12 12
#define MAGIC_NUMBER_50 50

#define XML_READ_VERBOSE TRUE

int
check_xml_save(char * file_name)
{
/* Returns TRUE if the file is correct, else returns FALSE. */
	if (XML_READ_VERBOSE)
		g_print("Testing XML file %s...", file_name);
		
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	doc = xmlParseFile(file_name);
	
	if (doc == NULL ) {
		fprintf(stderr,"\nDocument not parsed successfully. \n");
		return FALSE;
	}

	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"\nempty document\n");
		xmlFreeDoc(doc);
		return FALSE;
	}	
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "bygfoot-save")) {
		fprintf(stderr,"\ndocument of the wrong type, root node = %s != bygfoot-save\n", cur->name);
		xmlFreeDoc(doc);
		return FALSE;
	}
	
	if (XML_READ_VERBOSE) 
		g_print("OK.\n");
		
	return TRUE;	
}

void
read_xml_save(char * file_name)
/* Read the XML save file and set all variables */
{
	/* The XML file must be clean.*/
	assert (check_xml_save(file_name));

	gint stadium_index = 0;
	gint team_index = 0;
	gint transfer_index = 0;

	xmlDocPtr doc;
	xmlNodePtr cur;

	g_print ("XML import: starting.\n");

	doc = xmlParseFile(file_name);
	cur = xmlDocGetRootElement(doc);	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) 
	/* When cur == NULL, parsing is finished */
	{
		int known_node = 0;
		if (!xmlStrcmp(cur->name, (const xmlChar *) "options"))
		{
			known_node = 1;
			read_xml_options(doc, cur);
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "transfer"))
		{
			known_node = 1;
			assert (transfer_index < MAX_TRANSFERS);
			transferlist[transfer_index] = read_xml_transfer(doc, cur);
			transfer_index++;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "stadium"))
		{
			known_node = 1;
			assert (stadium_index < MAX_STADIUMS);
			stadiums[stadium_index] = read_xml_stadium(doc, cur);
			stadium_index++;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "team"))
		{
			known_node = 1;
			assert (team_index < MAX_TEAMS);
			teams[team_index] = read_xml_team(doc, cur);
			team_index++;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "fixtures"))
		{
			known_node = 1;
			read_xml_fixtures(doc, cur);
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "history"))
		{
			known_node = 1;
			read_xml_history(doc, cur);
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			known_node = 1;
			/* Text has no meaning at this level! */
		}		
		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name);
		}
		cur = cur->next;
			
	}
	
	assert (transfer_index >= MIN_TRANSFERS);
	assert (stadium_index >= MIN_STADIUMS);
	assert (team_index >= MIN_TEAMS);
	
	xmlFreeDoc(doc);
	g_print("XML import: OK.\n");	
}
	
void
read_xml_failure(xmlDocPtr doc)
{
	if (doc != NULL)
		xmlFreeDoc(doc);
	exit(-1);
}

void 
read_xml_options(xmlDocPtr doc, xmlNodePtr root)
{
	xmlNodePtr cur = root->xmlChildrenNode;
	gint memory_number = 0;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "my_team"))
		{
			my_team = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "season"))
		{
			season = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "week"))
		{
			week = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "scout"))
		{
			scout = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "physio"))
		{
			physio = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "font_name"))
		{
			read_xml_string(doc, cur, font_name);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "country_file_name"))
		{
			read_xml_string(doc, cur, country_file_name);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "memory"))
		{
			assert (memory_number < MAGIC_NUMBER_130);
			read_xml_options_memory(doc, cur, memory_number);
			known_node = 1;
			memory_number++;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}		
		
		cur = cur->next;	
	}
}

void
read_xml_options_memory(xmlDocPtr doc, xmlNodePtr root, gint memory_number)
{
	xmlNodePtr cur = root->xmlChildrenNode;
	gint goal_number = 0;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		if (!xmlStrcmp(cur->name, (const xmlChar *) "rank"))
		{
			assert (memory_number < MAGIC_NUMBER_130);
			rank[memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "rank_id"))
		{
			assert (memory_number < MAGIC_NUMBER_130);
			rank_ids[memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "stadium_fact0"))
		{
			assert (memory_number < 2);
			stadium_facts[0][memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "stadium_fact1"))
		{
			assert (memory_number < 2);
			stadium_facts[1][memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "options"))
		{
			assert (memory_number < OPT_END);
			options[memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}			
		if (!xmlStrcmp(cur->name, (const xmlChar *) "counter"))
		{
			assert (memory_number < COUNT_END);
			counters[memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "finances"))
		{
			assert (memory_number < FIN_END);
			finances[memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "league_name"))
		{
			assert (memory_number < LEAGUE_NAMES_END);
			read_xml_string(doc, cur, league_names[memory_number]);
			known_node = 1;
		}			
		if (!xmlStrcmp(cur->name, (const xmlChar *) "injury0"))
		{
			assert (memory_number < MAGIC_NUMBER_12);
			injuries[0][memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "injury1"))
		{
			assert (memory_number < MAGIC_NUMBER_12);
			injuries[1][memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "booked0"))
		{
			assert (memory_number < MAGIC_NUMBER_12);
			booked[0][memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "booked1"))
		{
			assert (memory_number < MAGIC_NUMBER_12);
			booked[1][memory_number] = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "goal"))
		{
			assert (memory_number < MAGIC_NUMBER_50);
			assert (goal_number < 2);
			goals[goal_number][memory_number] = read_xml_goal(doc, cur);
			known_node = 1;
			goal_number++;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
		cur = cur->next;		
	}
}

goal
read_xml_goal(xmlDocPtr doc, xmlNodePtr root)
{
	goal my_goal;
	xmlNodePtr cur = root->xmlChildrenNode;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "minute"))
		{
			my_goal.minute = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "team_id"))
		{
			my_goal.team_id = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "scorer"))
		{
			my_goal.scorer = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "time"))
		{
			my_goal.time = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "type"))
		{
			my_goal.type = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
		cur = cur->next;
	}
	return my_goal;
}

transfer 
read_xml_transfer(xmlDocPtr doc, xmlNodePtr root)
{
	transfer my_transfer;
	xmlNodePtr cur = root->xmlChildrenNode;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "time"))
		{
			my_transfer.time = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "team_id"))
		{
			my_transfer.team_id = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "player_number"))
		{
			my_transfer.player_number = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "estimates0"))
		{
			my_transfer.estimates[0] = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "estimates1"))
		{
			my_transfer.estimates[1] = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}		
				
		cur = cur->next;
	}
	return my_transfer;
	
}

stadium 
read_xml_stadium(xmlDocPtr doc, xmlNodePtr root)
{
	stadium my_stadium;
	
	xmlNodePtr cur = root->xmlChildrenNode;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "capacity"))
		{
			my_stadium.capacity = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "average_attendance"))
		{
			my_stadium.average_attendance = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "games"))
		{
			my_stadium.games = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "safety"))
		{
			my_stadium.safety = read_xml_float(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
					
		cur = cur->next;
	}	
	return my_stadium;
}

team 
read_xml_team(xmlDocPtr doc, xmlNodePtr root)
{
	team my_team;
	
	gint player_index = 0;
	gint result_index = 0;
	
	xmlNodePtr cur = root->xmlChildrenNode;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		if (!xmlStrcmp(cur->name, (const xmlChar *) "name"))
		{
			read_xml_string(doc, cur, my_team.name);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "style"))
		{
			my_team.style = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "structure"))
		{
			my_team.structure = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "id"))
		{
			my_team.id = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "result"))
		{
			assert (result_index < MAX_RESULTS);
			my_team.results[result_index] = read_xml_int(doc, cur);
			result_index++;
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "player"))
		{
			assert (player_index < MAX_PLAYERS);
			my_team.players[player_index] = read_xml_player(doc, cur);
			player_index++;
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}						
	
		cur = cur->next;
	}
	assert (result_index >= MIN_RESULTS);
	assert (player_index >= MIN_PLAYERS);
	
	return my_team;
}

player 
read_xml_player(xmlDocPtr doc, xmlNodePtr root)
{
	player my_player;
	
	xmlNodePtr cur = root->xmlChildrenNode;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		if (!xmlStrcmp(cur->name, (const xmlChar *) "name"))
		{
			read_xml_string(doc, cur, my_player.name);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "pos"))
		{
			my_player.pos = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "cpos"))
		{
			my_player.cpos = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "health"))
		{
			my_player.health = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "goals"))
		{
			my_player.goals = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "booked"))
		{
			my_player.booked = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "games"))
		{
			my_player.games = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "value"))
		{
			my_player.value = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "wage"))
		{
			my_player.wage = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "team_id"))
		{
			my_player.team_id = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "skill"))
		{
			my_player.skill = read_xml_float(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "cskill"))
		{
			my_player.cskill = read_xml_float(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "talent"))
		{
			my_player.talent = read_xml_float(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "etal"))
		{
			my_player.etal = read_xml_float(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "fitness"))
		{
			my_player.fitness = read_xml_float(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "last_skill_update"))
		{
			my_player.last_skill_update = read_xml_float(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "age"))
		{
			my_player.age = read_xml_float(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "peak_age"))
		{
			my_player.peak_age = read_xml_float(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
		
		cur = cur->next;
	}
	return my_player;
}

player_stat
read_xml_player_stat(xmlDocPtr doc, xmlNodePtr root)
{
	player_stat my_player_stat;
	
	xmlNodePtr cur = root->xmlChildrenNode;
	gint known_node = 0;
	
	while (cur != NULL)
	{
		if (!xmlStrcmp(cur->name, (const xmlChar *) "games"))
		{
			my_player_stat.games = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "goals"))
		{
			my_player_stat.goals = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "name"))
		{
			read_xml_string(doc, cur, my_player_stat.name);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "team_name"))
		{
			read_xml_string(doc, cur, my_player_stat.team_name);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
		cur = cur->next;
	}
	return my_player_stat;
}

season_stat 
read_xml_season_stat(xmlDocPtr doc, xmlNodePtr root)
{
	season_stat my_season_stat;
	
	xmlNodePtr cur = root->xmlChildrenNode;
	gint known_node = 0;
	gint team_names_index = 0;
	gint history_best_player_index = 0;
	
	while (cur != NULL)
	{
		if (!xmlStrcmp(cur->name, (const xmlChar *) "season_number"))
		{
			my_season_stat.season_number = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "my_league"))
		{
			my_season_stat.my_league = read_xml_int(doc, cur);
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "my_rank"))
		{
			my_season_stat.my_rank = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "team_name"))
		{
			assert (team_names_index < STAT_END);
			read_xml_string(doc, cur, my_season_stat.team_names[team_names_index]);
			team_names_index++;
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "history_best_player"))
		{
			assert (history_best_player_index < 6);
			my_season_stat.best_players[history_best_player_index] = read_xml_player_stat(doc, cur);
			history_best_player_index++;
			known_node = 1;
		}		
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}		
		cur = cur->next;
	}
	return my_season_stat;
}

void 
read_xml_history(xmlDocPtr doc, xmlNodePtr root)
{
	xmlNodePtr cur = root->xmlChildrenNode;
	gint known_node = 0;
	gint history_number = 0;
	
	while (cur != NULL)
	{
		if (!xmlStrcmp(cur->name, (const xmlChar *) "number_of_histories"))	
		{
			season = read_xml_int(doc, cur) + 1;
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "history_element"))	
		{	
			assert (history_number < season - 1);
			history[history_number] = read_xml_season_stat(doc, cur);
			history_number++;
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}				
		cur = cur->next;
	}
}

void 
read_xml_fixtures(xmlDocPtr doc, xmlNodePtr root)
{
	gint index_fixtures = 0;
	
	xmlNodePtr cur = root->xmlChildrenNode;

	while (cur != NULL)
	{
		gint known_node = 0;
		if (!xmlStrcmp(cur->name, (const xmlChar *) "fixture"))
		{
			assert(index_fixtures < MAX_FIXTURES);
			fixtures[index_fixtures] = read_xml_fixture(doc, cur);
			known_node = 1;
			index_fixtures++;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
		cur = cur->next;
	}
}	
		
fixture 
read_xml_fixture(xmlDocPtr doc, xmlNodePtr root)
{
	xmlNodePtr cur = root->xmlChildrenNode;
	fixture my_fixture;
	gint result_index = 0;
	
	while (cur != NULL)
	{
		gint known_node = 0;
		if (!xmlStrcmp(cur->name, (const xmlChar *) "type"))
		{
			my_fixture.type = read_xml_int(doc, cur);
			known_node = 1;
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "week"))
		{
			my_fixture.week_number = read_xml_int(doc, cur);
			known_node = 1;
		}	
		if (!xmlStrcmp(cur->name, (const xmlChar *) "result"))
		{
			assert ((result_index >= 0) && (result_index < 2));
			xmlNodePtr cur2	= cur->xmlChildrenNode;
			
			while (cur2 != NULL)
			{
				gint known_node2 = 0;
				if (!xmlStrcmp(cur2->name, (const xmlChar *) "team_id"))
				{
					my_fixture.team_id[result_index] = read_xml_int(doc, cur2);
					known_node2 = 1;
				}	
				if (!xmlStrcmp(cur2->name, (const xmlChar *) "result0"))
				{
					my_fixture.result[result_index][0] = read_xml_int(doc, cur2);
					known_node2 = 1;
				}	
				if (!xmlStrcmp(cur2->name, (const xmlChar *) "result1"))
				{
					my_fixture.result[result_index][1] = read_xml_int(doc, cur2);
					known_node2 = 1;
				}				
				if (!xmlStrcmp(cur2->name, (const xmlChar *) "result2"))
				{
					my_fixture.result[result_index][2] = read_xml_int(doc, cur2);
					known_node2 = 1;
				}			
				if (!xmlStrcmp(cur2->name, (const xmlChar *) "text"))
				{
					/* text is not significant here */
					known_node2 = 1;
				}		
				if (!known_node2)
				{
					fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur2->name, cur->name);
				}			
				cur2 = cur2->next;
			}
			result_index++;
			known_node = 1;	
		}
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node \'%s\' in \'%s\' is not known. Are you sure you use the good bygfoot version?\n", cur->name, root->name);
		}
		cur = cur->next;
	}
	return my_fixture;
}

/****************** helpers functions for xml reading ************************/

gint
read_xml_int(xmlDocPtr doc, xmlNodePtr node)
{
	xmlChar * content;
	gint result;
	
	content = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
	sscanf(content, "%d", &result);
	xmlFree(content);
	return result;
}

gfloat
read_xml_float(xmlDocPtr doc, xmlNodePtr node)
{
	xmlChar * content;
	gfloat result;
	
	content = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
	sscanf(content, "%f", &result);
	xmlFree(content);
	return result;
}

void
read_xml_string(xmlDocPtr doc, xmlNodePtr node, gchar * result)
{
	xmlChar * content;
	
	content = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
	if (content != NULL)
	{		
		sprintf(result, "%s", content);
	} else {
		sprintf(result, "%s", "\0");
	}
	xmlFree(content);
}
