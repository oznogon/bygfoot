#ifndef XML_WRITE_H
#define XML_WRITE_H

#include "bygfoot.h"

void write_xml_save(char * file_name);
void write_xml_init(FILE * xml_file);
void write_xml_end(FILE * xml_file);

void write_xml_player(player * pl, FILE * xml_file);

void write_xml_stadiums(FILE * xml_file);
void write_xml_stadium(stadium * st, FILE * xml_file);

void write_xml_teams(FILE * xml_file);
void write_xml_team(team * tm, FILE * xml_file);

void write_xml_transfers(FILE * xml_file);
void write_xml_transfer(transfer * tf, FILE * xml_file);

void write_xml_options(FILE * xml_file);

#endif
