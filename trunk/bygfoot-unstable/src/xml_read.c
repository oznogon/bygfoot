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
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			known_node = 1;
			/* Text has no meaning at this level! */
		}		
		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node %s is not known. Are you sure you use the good bygfoot version?\n", cur->name);
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
		if (!xmlStrcmp(cur->name, (const xmlChar *) "text"))
		{
			/* text is not significant here */
			known_node = 1;
		}		
		if (!known_node)
		{
			fprintf(stderr, "Warning! XML node %s is not known. Are you sure you use the good bygfoot version?\n", cur->name);
		}		
		
		cur = cur->next;	
	}
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
			fprintf(stderr, "Warning! XML node %s is not known. Are you sure you use the good bygfoot version?\n", cur->name);
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
			fprintf(stderr, "Warning! XML node %s is not known. Are you sure you use the good bygfoot version?\n", cur->name);
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
			fprintf(stderr, "Warning! XML node %s is not known. Are you sure you use the good bygfoot version?\n", cur->name);
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
			fprintf(stderr, "Warning! XML node %s is not known. Are you sure you use the good bygfoot version?\n", cur->name);
		}
		
		cur = cur->next;
	}
	return my_player;
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
	sprintf(result, "%s", content);
	xmlFree(content);
}
	
