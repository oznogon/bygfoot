#ifndef XML_GENERAL_H
#define XML_GENERAL_H

#include "bygfoot.h"
#include "free.h"
#include "variables.h"
#include "xml.h"

void
xml_general_read_start_element (GMarkupParseContext *context,
				const gchar         *element_name,
				const gchar        **attribute_names,
				const gchar        **attribute_values,
				gpointer             user_data,
				GError             **error);

void
xml_general_read_end_element    (GMarkupParseContext *context,
				 const gchar         *element_name,
				 gpointer             user_data,
				 GError             **error);

void
xml_general_read_text         (GMarkupParseContext *context,
			       const gchar         *text,
			       gsize                text_len,  
			       gpointer             user_data,
			       GError             **error);

void
xml_general_read(gchar *file_name);

void
xml_general_write(gchar *file_name);

void
xml_general_write_single(FILE *xml_file);

void
xml_general_write_goals(FILE *xml_file);

void
xml_general_write_goal(goal * gl, FILE * xml_file);

void
xml_general_write_stadiums(FILE *xml_file);

void
xml_general_write_transfers(FILE *xml_file);

void
xml_general_write_season_stats(FILE *xml_file);

void
xml_general_write_season_stat(FILE *xml_file, season_stat stat);

void
xml_general_write_season_stat_player(FILE *xml_file, player_stat plst);

#endif
