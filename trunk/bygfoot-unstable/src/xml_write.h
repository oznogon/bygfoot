#ifndef XML_WRITE_H
#define XML_WRITE_H

#include <stdio.h>
#include <gtk/gtk.h>
#include "bygfoot.h"
#include "variables.h"
#include "history.h"

void write_xml_save(char * file_name);
void write_xml_init(FILE * xml_file);
void write_xml_end(FILE * xml_file);

void write_xml_player(player * pl, FILE * xml_file);
void
write_xml_player_history(player *pl, FILE *xml_file);
void
write_xml_player_history_element(player *pl, FILE *xml_file, gint idx);

void write_xml_stadiums(FILE * xml_file);
void write_xml_stadium(stadium * st, FILE * xml_file);

void write_xml_teams(FILE * xml_file);
void write_xml_team(team * tm, FILE * xml_file);
void
write_xml_team_history(team *tm, FILE *xml_file);
void
write_xml_team_history_element(team *tm, FILE *xml_file, gint idx);

void write_xml_transfers(FILE * xml_file);
void write_xml_transfer(transfer * tf, FILE * xml_file);

void write_xml_options(FILE * xml_file);
void write_xml_goal(goal * gl, FILE * xml_file);

void write_xml_history(FILE * xml_file);
void write_xml_history_element(season_stat * stat, FILE * xml_file);
void write_xml_history_best_player(player_stat * pl_stat, FILE * xml_file);

void write_xml_fixtures(FILE * xml_file);

#endif
