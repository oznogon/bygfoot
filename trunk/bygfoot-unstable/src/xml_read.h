#ifndef XML_READ_H
#define XML_READ_H

#include <libxml/parser.h>

#include "bygfoot.h"

int check_xml_save(char * file_name);
void read_xml_save(char * file_name);

void 	read_xml_options(xmlDocPtr doc, xmlNodePtr cur);
transfer read_xml_transfer(xmlDocPtr doc, xmlNodePtr cur);
stadium	read_xml_stadium(xmlDocPtr doc, xmlNodePtr cur);
team 	read_xml_team	(xmlDocPtr doc, xmlNodePtr cur);
player 	read_xml_player	(xmlDocPtr doc, xmlNodePtr cur);

void 	read_xml_failure(xmlDocPtr doc);

gint 	read_xml_int	(xmlDocPtr doc, xmlNodePtr node);
gfloat 	read_xml_float	(xmlDocPtr doc, xmlNodePtr node);
void	read_xml_string	(xmlDocPtr doc, xmlNodePtr node, gchar * result);

#endif
