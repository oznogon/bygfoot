#ifndef XML_TEAMS_H
#define XML_TEAMS_H

#include "bygfoot.h"
#include "history.h"
#include "player.h"
#include "team.h"
#include "variables.h"
#include "xml.h"

void
xml_teams_read_start_element (GMarkupParseContext *context,
			      const gchar         *element_name,
			      const gchar        **attribute_names,
			      const gchar        **attribute_values,
			      gpointer             user_data,
			      GError             **error);

void
xml_teams_read_end_element    (GMarkupParseContext *context,
			       const gchar         *element_name,
			       gpointer             user_data,
			       GError             **error);


void
xml_teams_read_text_team(team *tm, gchar *text);

void
xml_teams_read_text_player(player *pl, gchar *text);

void
xml_teams_read_text         (GMarkupParseContext *context,
			     const gchar         *text,
			     gsize                text_len,  
			     gpointer             user_data,
			     GError             **error);

void
xml_teams_read(gchar *file_name);

void
xml_teams_write(gchar *file_name);

void
xml_teams_write_team(FILE *xml_file, team tm);

void
xml_teams_write_player(FILE *xml_file, player pl);

void
xml_teams_write_player_history_element(FILE *xml_file, player_history history);

void
xml_teams_write_team_history_element(FILE *xml_file, team_history history);

#endif
