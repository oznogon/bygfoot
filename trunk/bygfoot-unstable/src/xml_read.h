#ifndef XML_READ_H
#define XML_READ_H

#include <libxml/parser.h>
#include <gtk/gtk.h>
#include "bygfoot.h"
#include "variables.h"
#include "history.h"

int check_xml_save(char * file_name);
void read_xml_save(char * file_name);

void 	read_xml_options(xmlDocPtr doc, xmlNodePtr root);
void 	read_xml_options_memory(xmlDocPtr doc, xmlNodePtr root, gint memory_number);
goal	read_xml_goal(xmlDocPtr doc, xmlNodePtr root);
transfer read_xml_transfer(xmlDocPtr doc, xmlNodePtr root);
stadium	read_xml_stadium(xmlDocPtr doc, xmlNodePtr root);

team 	read_xml_team	(xmlDocPtr doc, xmlNodePtr root);
GArray*
read_xml_team_history(xmlDocPtr doc, xmlNodePtr root);
team_history
read_xml_team_history_element(xmlDocPtr doc, xmlNodePtr root);

player 	read_xml_player	(xmlDocPtr doc, xmlNodePtr root);
GArray*
read_xml_player_history(xmlDocPtr doc, xmlNodePtr root);
player_history
read_xml_player_history_element(xmlDocPtr doc, xmlNodePtr root);


void	read_xml_fixtures(xmlDocPtr doc, xmlNodePtr root);
fixture read_xml_fixture(xmlDocPtr doc, xmlNodePtr root);
void 
read_xml_history(xmlDocPtr doc, xmlNodePtr root);
season_stat read_xml_season_stat(xmlDocPtr doc, xmlNodePtr root);

void 	read_xml_failure(xmlDocPtr doc);

gint 	read_xml_int	(xmlDocPtr doc, xmlNodePtr node);
gfloat 	read_xml_float	(xmlDocPtr doc, xmlNodePtr node);
void	read_xml_string	(xmlDocPtr doc, xmlNodePtr node, gchar * result);


#endif
