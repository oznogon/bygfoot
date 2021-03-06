#ifndef XML_LOADSAVE_TEAMS_H
#define XML_LOADSAVE_TEAMS_H

#include "bygfoot.h"
#include "player_struct.h"
#include "team_struct.h"

void
xml_loadsave_teams_start_element (GMarkupParseContext *context,
				   const gchar         *element_name,
				   const gchar        **attribute_names,
				   const gchar        **attribute_values,
				   gpointer             user_data,
				   GError             **error);

void
xml_loadsave_teams_end_element    (GMarkupParseContext *context,
				    const gchar         *element_name,
				    gpointer             user_data,
				    GError             **error);

void
xml_loadsave_teams_text         (GMarkupParseContext *context,
				  const gchar         *text,
				  gsize                text_len,  
				  gpointer             user_data,
				  GError             **error);

void
xml_loadsave_teams_read(const gchar *filename, GArray *teams);

void
xml_loadsave_teams_write(const gchar *filename, const GArray *teams);

void
xml_loadsave_teams_write_team(FILE *fil, const Team* team);

void
xml_loadsave_teams_write_player(FILE *fil, const Player *pl);

#endif
