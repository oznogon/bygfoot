#include "xml_teams.h"

enum
{
    TAG_START = 0,
    TAG_TEAMS,
    TAG_PLAYER,
    TAG_PLAYER_HISTORIES,
    TAG_PLAYER_HISTORY,
    TAG_PLAYER_HISTORY_VALUE,
    TAG_TEAM_HISTORIES,
    TAG_TEAM_HISTORY,
    TAG_TEAM_HISTORY_VALUE,
    TAG_PLAYER_NAME,
    TAG_PLAYER_POS,
    TAG_PLAYER_CPOS,
    TAG_PLAYER_HEALTH,
    TAG_PLAYER_GOALS,
    TAG_PLAYER_BOOKED,
    TAG_PLAYER_GAMES,
    TAG_PLAYER_VALUE,
    TAG_PLAYER_WAGE,
    TAG_PLAYER_TEAMID,
    TAG_PLAYER_LASTSKILL,
    TAG_PLAYER_AGE,
    TAG_PLAYER_PEAKAGE,
    TAG_PLAYER_SKILL,
    TAG_PLAYER_CSKILL,
    TAG_PLAYER_TALENT,
    TAG_PLAYER_ETAL,
    TAG_PLAYER_FITNESS,
    TAG_TEAM,
    TAG_TEAM_NAME,
    TAG_TEAM_ID,
    TAG_TEAM_STRUCTURE,
    TAG_TEAM_RESULTS,
    TAG_TEAM_RESULT,
    TAG_TEAM_STYLE,
    TAG_TEAM_PLAYERS,
    TAG_END
};

gint state;
gint team_idx, player_idx, team_res_idx, history_value_idx;
player_history pl_history;
team_history tm_history;

void
xml_teams_read_start_element (GMarkupParseContext *context,
			      const gchar         *element_name,
			      const gchar        **attribute_names,
			      const gchar        **attribute_values,
			      gpointer             user_data,
			      GError             **error)
{
    gint i;
    gint tag = xml_get_tag_from_name(element_name);
    gboolean valid_tag = FALSE;
    gchar progress_text[SMALL];

    for(i=TAG_START;i<TAG_END;i++)
	if(tag == i)
	{
	    state = i;
	    valid_tag = TRUE;
	}

    if(tag == TAG_TEAM)
    {
	team_res_idx = player_idx = 0;
	sprintf(progress_text, _("Loading team %d..."), team_idx);
	progress += PROGRESS_TEAM;	
	show_progress(progress / PROGRESS_MAX, progress_text);
    }
    else if(tag == TAG_PLAYER_HISTORY ||
	    tag == TAG_TEAM_HISTORY)
	history_value_idx = 0;

    if(!valid_tag)
	g_warning("xml_teams_read_start_element: unknown tag: %s; I'm in state %d\n",
		  element_name, state);
}

void
xml_teams_read_end_element    (GMarkupParseContext *context,
			       const gchar         *element_name,
			       gpointer             user_data,
			       GError             **error)
{
    gint tag = xml_get_tag_from_name(element_name);

    if(tag == TAG_TEAM_NAME ||
       tag == TAG_TEAM_ID ||
       tag == TAG_TEAM_STRUCTURE ||
       tag == TAG_TEAM_STYLE ||
       tag == TAG_TEAM_RESULTS ||
       tag == TAG_TEAM_PLAYERS ||
       tag == TAG_TEAM_HISTORIES)
	state = TAG_TEAM;
    else if(tag == TAG_TEAM)
    {
	team_idx++;
	state = TAG_TEAMS;
    }
    else if((tag >= TAG_PLAYER_NAME &&
	     tag <= TAG_PLAYER_FITNESS) ||
	    tag == TAG_PLAYER_HISTORIES)
	state = TAG_PLAYER;
    else if(tag == TAG_PLAYER)
    {
	player_idx++;
	state = TAG_TEAM_PLAYERS;
    }
    else if(tag == TAG_PLAYER_HISTORY)
    {
	g_array_append_val(teams[team_idx].players[player_idx].history,
			   pl_history);
	state = TAG_PLAYER_HISTORIES;
    }
    else if(tag == TAG_PLAYER_HISTORY_VALUE)
    {
	history_value_idx++;
	state = TAG_PLAYER_HISTORY;
    }
    else if(tag == TAG_TEAM_HISTORY)
    {
	g_array_append_val(teams[team_idx].history, tm_history);
	state = TAG_TEAM_HISTORIES;
    }
    else if(tag == TAG_TEAM_HISTORY_VALUE)
    {
	history_value_idx++;
	state = TAG_TEAM_HISTORY;
    }
    else if(tag == TAG_TEAM_RESULT)
    {
	team_res_idx++;
	state = TAG_TEAM_RESULTS;
    }
    else if(tag != TAG_TEAMS)
	g_warning("xml_teams_read_end_element: unknown tag: %s; I'm in state %d\n",
		  element_name, state);
}


void
xml_teams_read_text_team(team *tm, gchar *text)
{
    gint int_value = (gint)g_ascii_strtod(text, NULL);

    switch(state)
    {
	case TAG_TEAM_NAME:
	    strcpy(tm->name, text);
	    break;
	case TAG_TEAM_ID:
	    tm->id = int_value;
	    break;
	case TAG_TEAM_STRUCTURE:
	    tm->structure = int_value;
	    break;
	case TAG_TEAM_RESULT:
	    tm->results[team_res_idx] = int_value;
	    break;
	case TAG_TEAM_STYLE:
	    tm->style = int_value;
	    break;
    }
}

void
xml_teams_read_text_player(player *pl, gchar *text)
{
    gint int_value = (gint)g_ascii_strtod(text, NULL);
    gfloat float_value = (gfloat)g_ascii_strtod(text, NULL) / FLOAT_FACTOR;

    switch(state)
    {
	default:
	    strcpy(pl->name, text);
	    break;
	case TAG_PLAYER_POS:
	    pl->pos = int_value;
	    break;
	case TAG_PLAYER_CPOS:
	    pl->cpos = int_value;
	    break;
	case TAG_PLAYER_HEALTH:
	    pl->health = int_value;
	    break;
	case TAG_PLAYER_GOALS:
	    pl->goals = int_value;
	    break;
	case TAG_PLAYER_BOOKED:
	    pl->booked = int_value;
	    break;
	case TAG_PLAYER_GAMES:
	    pl->games = int_value;
	    break;
	case TAG_PLAYER_VALUE:
	    pl->value = int_value;
	    break;
	case TAG_PLAYER_WAGE:
	    pl->wage = int_value;
	    break;
	case TAG_PLAYER_TEAMID:
	    pl->team_id = int_value;
	    break;
	case TAG_PLAYER_LASTSKILL:
	    pl->last_skill_update = float_value;
	    break;
	case TAG_PLAYER_AGE:
	    pl->age = float_value;
	    break;
	case TAG_PLAYER_PEAKAGE:
	    pl->peak_age = float_value;
	    break;
	case TAG_PLAYER_SKILL:
	    pl->skill = float_value;
	    break;
	case TAG_PLAYER_CSKILL:
	    pl->cskill = float_value;
	    break;
	case TAG_PLAYER_TALENT:
	    pl->talent = float_value;
	    break;
	case TAG_PLAYER_ETAL:
	    pl->etal = float_value;
	    break;
	case TAG_PLAYER_FITNESS:
	    pl->fitness = float_value;
	    break;
    }
}

void
xml_teams_read_text         (GMarkupParseContext *context,
			     const gchar         *text,
			     gsize                text_len,  
			     gpointer             user_data,
			     GError             **error)
{
    gchar buf[SMALL];
    gint int_value = -1;
    gfloat float_value = -1;
    team *tm = NULL;
    player *pl = NULL;

    strncpy(buf, text, text_len);
    buf[text_len] = '\0';

    int_value = (gint)g_ascii_strtod(buf, NULL);
    float_value = (gfloat)g_ascii_strtod(buf, NULL) / FLOAT_FACTOR;

    if(team_idx < 178)
    {
	tm = &teams[team_idx];
	if(player_idx < 20)
	    pl = &tm->players[player_idx];
    }
    
    if(state == TAG_PLAYER_HISTORY_VALUE)
	pl_history.values[history_value_idx] =
	    int_value;
    else if(state == TAG_TEAM_HISTORY_VALUE)
	tm_history.values[history_value_idx] =
	    int_value;
    else if(state >= TAG_PLAYER_NAME &&
	    state <= TAG_PLAYER_FITNESS)
	xml_teams_read_text_player(pl, buf);
    else if(state >= TAG_TEAM_NAME &&
	    state <= TAG_TEAM_STYLE)
	xml_teams_read_text_team(tm, buf);
}

void
xml_teams_read(gchar *file_name)
{
    gint i, j;
    gchar file[SMALL];
    GMarkupParser parser = {xml_teams_read_start_element,
			    xml_teams_read_end_element,
			    xml_teams_read_text, NULL, NULL};
    GMarkupParseContext *context;
    gchar *file_contents;
    gint length;
    GError *error = NULL;

    team_idx = player_idx = team_res_idx = history_value_idx = 0;

    for(i=0;i<178;i++)
    {
	reset_team_history(&teams[i]);
	for(j=0;j<20;j++)
	    reset_player_history(&teams[i].players[j]);
    }
    
    sprintf(file, "%s_%s.xml", file_name, XML_FILE_EXT_TEAMS);

    context = 
	g_markup_parse_context_new(&parser, 0, NULL, NULL);

    if(!g_file_get_contents(file, &file_contents, &length, &error))
    {
	g_warning("xml_teams_read: error reading file %s\n", file);
	print_error(error, FALSE);
	return;
    }

    if(g_markup_parse_context_parse(context, file_contents, length, &error))
    {
	g_markup_parse_context_end_parse(context, NULL);	
	g_markup_parse_context_free(context);
	g_free(file_contents);
    }
    else
    {
	g_critical("xml_teams_read: error parsing file %s\n", file);
	print_error(error, TRUE);
    }
}

void
xml_teams_write(gchar *file_name)
{
    gint i;
    gchar file[SMALL];
    gchar progress_text[SMALL];
    FILE *xml_file;

    sprintf(file, "%s_%s.xml", file_name, XML_FILE_EXT_TEAMS);

    my_fopen(file, "w", &xml_file, FALSE);

    xml_write_init(xml_file, TAG_TEAMS);

    for(i=0;i<178;i++)
    {
	sprintf(progress_text, _("Saving team %d..."), i);
	progress += PROGRESS_TEAM;
	show_progress(progress / PROGRESS_MAX, progress_text);
	xml_teams_write_team(xml_file, teams[i]);
    }

    xml_write_end(xml_file, TAG_TEAMS);

    fclose(xml_file);
}

void
xml_teams_write_team(FILE *xml_file, team tm)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_TEAM);

    fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT1, TAG_TEAM_NAME, tm.name, TAG_TEAM_NAME);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_TEAM_ID, tm.id, TAG_TEAM_ID);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_TEAM_STRUCTURE, tm.structure, TAG_TEAM_STRUCTURE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_TEAM_STYLE, tm.style, TAG_TEAM_STYLE);

    fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_TEAM_RESULTS);
    for(i=0;i<RES_END;i++)
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2, 
		TAG_TEAM_RESULT, tm.results[i], TAG_TEAM_RESULT);
    fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_TEAM_RESULTS);

    fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_TEAM_PLAYERS);
    for(i=0;i<20;i++)
	xml_teams_write_player(xml_file, tm.players[i]);
    fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_TEAM_PLAYERS);

    fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_TEAM_HISTORIES);
    if(tm.history != NULL)
	for(i=0;i<tm.history->len;i++)
	    xml_teams_write_team_history_element(xml_file, 
						 g_array_index(tm.history, team_history, i));
    fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_TEAM_HISTORIES);

    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_TEAM);
}

void
xml_teams_write_player(FILE *xml_file, player pl)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT2, TAG_PLAYER);
    
    fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT3, TAG_PLAYER_NAME, pl.name, TAG_PLAYER_NAME);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_POS, pl.pos, TAG_PLAYER_POS);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_CPOS, pl.cpos, TAG_PLAYER_CPOS);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_HEALTH, pl.health, TAG_PLAYER_HEALTH);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_GOALS, pl.goals, TAG_PLAYER_GOALS);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_BOOKED, pl.booked, TAG_PLAYER_BOOKED);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_GAMES, pl.games, TAG_PLAYER_GAMES);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_VALUE, pl.value, TAG_PLAYER_VALUE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_WAGE, pl.wage, TAG_PLAYER_WAGE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, TAG_PLAYER_TEAMID, pl.team_id, TAG_PLAYER_TEAMID);

    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_LASTSKILL, (gint)(pl.last_skill_update * FLOAT_FACTOR), TAG_PLAYER_LASTSKILL);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_AGE, (gint)(pl.age * FLOAT_FACTOR), TAG_PLAYER_AGE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_PEAKAGE, (gint)(pl.peak_age * FLOAT_FACTOR), TAG_PLAYER_PEAKAGE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_SKILL, (gint)(pl.skill * FLOAT_FACTOR), TAG_PLAYER_SKILL);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_CSKILL, (gint)(pl.cskill * FLOAT_FACTOR), TAG_PLAYER_CSKILL);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_TALENT, (gint)(pl.talent * FLOAT_FACTOR), TAG_PLAYER_TALENT);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_ETAL, (gint)(pl.etal * FLOAT_FACTOR), TAG_PLAYER_ETAL);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
	    TAG_PLAYER_FITNESS, (gint)(pl.fitness * FLOAT_FACTOR), TAG_PLAYER_FITNESS);

    fprintf(xml_file, "%s<_%d>\n", INDENT3, TAG_PLAYER_HISTORIES);
    if(pl.history != NULL)
	for(i=0;i<pl.history->len;i++)
	    xml_teams_write_player_history_element(xml_file, 
						   g_array_index(pl.history, player_history, i));
    fprintf(xml_file, "%s</_%d>\n", INDENT3, TAG_PLAYER_HISTORIES);

    fprintf(xml_file, "%s</_%d>\n", INDENT2, TAG_PLAYER);
}

void
xml_teams_write_player_history_element(FILE *xml_file, player_history history)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT4, TAG_PLAYER_HISTORY);

    for(i=0;i<PLAYER_HISTORY_END;i++)
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT5, 
		TAG_PLAYER_HISTORY_VALUE, history.values[i], TAG_PLAYER_HISTORY_VALUE);

    fprintf(xml_file, "%s</_%d>\n", INDENT4, TAG_PLAYER_HISTORY);
}

void
xml_teams_write_team_history_element(FILE *xml_file, team_history history)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT2, TAG_TEAM_HISTORY);

    for(i=0;i<TEAM_HISTORY_END;i++)
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT3, 
		TAG_TEAM_HISTORY_VALUE, history.values[i], TAG_TEAM_HISTORY_VALUE);

    fprintf(xml_file, "%s</_%d>\n", INDENT2, TAG_TEAM_HISTORY);
}
