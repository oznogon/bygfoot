#include "xml_general.h"

enum
{
    TAG_START = 0,
    TAG_GENERAL,
    TAG_MYTEAM,
    TAG_SEASON,
    TAG_WEEK,
    TAG_SCOUT,
    TAG_PHYSIO,
    TAG_FONTNAME,
    TAG_COUNTRYFILENAME,
    TAG_GOALS,
    TAG_GOAL,
    TAG_GOAL_MINUTE,
    TAG_GOAL_TEAMID,
    TAG_GOAL_SCORER,
    TAG_GOAL_TIME,
    TAG_GOAL_TYPE,
    TAG_STAD_FACTS,
    TAG_STAD_FACT0,
    TAG_STAD_FACT1,
    TAG_STADS,
    TAG_STAD,
    TAG_STAD_CAP,
    TAG_STAD_AVATT,
    TAG_STAD_GAMES,
    TAG_STAD_SAF,
    TAG_OPTS,
    TAG_OPT,
    TAG_COUNTS,
    TAG_COUNT,
    TAG_RANKS,
    TAG_RANK,
    TAG_RANK_ID,
    TAG_FINS,
    TAG_FIN,
    TAG_LEAGUENAMES,
    TAG_LEAGUENAME,
    TAG_INJBOOKS,
    TAG_INJ0,
    TAG_INJ1,
    TAG_BOOK0,
    TAG_BOOK1,
    TAG_TRANSFERS,
    TAG_TRANSFER,
    TAG_TRANSFER_TEAMID,
    TAG_TRANSFER_PLAYERNR,
    TAG_TRANSFER_TIME,
    TAG_TRANSFER_ESTIMATEVALUE,
    TAG_TRANSFER_ESTIMATEWAGE,
    TAG_SEASON_STATS,
    TAG_SEASON_STAT,
    TAG_SEASON_STAT_SEASON,
    TAG_SEASON_STAT_MYLEAGUE,
    TAG_SEASON_STAT_MYRANK,
    TAG_SEASON_STAT_TEAMNAMES,
    TAG_SEASON_STAT_TEAMNAME,
    TAG_SEASON_STAT_PLAYER_STATS,
    TAG_SEASON_STAT_PLAYER_STAT,
    TAG_SEASON_STAT_PLAYER_NAME,
    TAG_SEASON_STAT_PLAYER_TEAMNAME,
    TAG_SEASON_STAT_PLAYER_GOALS,
    TAG_SEASON_STAT_PLAYER_GAMES,
    TAG_SEASON_OBJECTIVES,
    TAG_SEASON_OBJECTIVE,
    TAG_SEASON_OBJECTIVE_TYPE,
    TAG_SEASON_OBJECTIVE_EXTRA_DATA,
    TAG_END
};

gint idx[2], stat_idx;
gint state;
season_stat *seasonstat1, *seasonstat2;

void
xml_general_read_start_element (GMarkupParseContext *context,
				const gchar         *element_name,
				const gchar        **attribute_names,
				const gchar        **attribute_values,
				gpointer             user_data,
				GError             **error)
{
    gint i;
    gint tag = xml_get_tag_from_name(element_name);
    gboolean valid_tag = FALSE;

    for(i=TAG_START;i<TAG_END;i++)
	if(tag == i)
	{
	    state = i;
	    valid_tag = TRUE;
	}

    if(tag == TAG_GOALS ||
       tag == TAG_STADS ||
       tag == TAG_STAD_FACTS ||
       tag == TAG_OPTS ||
       tag == TAG_COUNTS ||
       tag == TAG_RANKS ||
       tag == TAG_FINS ||
       tag == TAG_LEAGUENAMES ||
       tag == TAG_INJBOOKS ||
       tag == TAG_TRANSFERS ||
       tag == TAG_SEASON_OBJECTIVES)
	idx[0] = idx[1] = 0;
    else if(tag == TAG_SEASON_STAT)
    {
	idx[0] = idx[1] = 0;
	if(history == NULL)
	{
	    history = (season_stat*)g_malloc(sizeof(season_stat));
	    history->next = NULL;
	    seasonstat1 = history;
	}
	else
	{
	    seasonstat2 = seasonstat1;
	    seasonstat1 = (season_stat*)g_malloc(sizeof(season_stat));
	    seasonstat1->next = NULL;
	    seasonstat2->next = seasonstat1;
	}
    }

    if(!valid_tag)
    {
	g_warning("xml_general_read_start_element: unknown tag: %s\n",
		  element_name);
	g_warning("I'm in state %d", state);
    }
}

void
xml_general_read_end_element    (GMarkupParseContext *context,
				 const gchar         *element_name,
				 gpointer             user_data,
				 GError             **error)
{
    gint tag = xml_get_tag_from_name(element_name);

    if((tag >= TAG_MYTEAM &&
	tag <= TAG_COUNTRYFILENAME) ||
       tag == TAG_GOALS ||
       tag == TAG_OPTS ||
       tag == TAG_COUNTS ||
       tag == TAG_RANKS ||
       tag == TAG_FINS ||
       tag == TAG_LEAGUENAMES ||
       tag == TAG_INJBOOKS ||
       tag == TAG_STADS ||
       tag == TAG_STAD_FACTS ||
       tag == TAG_TRANSFERS ||
       tag == TAG_SEASON_STATS ||
       tag == TAG_SEASON_OBJECTIVES)
	state = TAG_GENERAL;
    else if(tag == TAG_LEAGUENAME)
    {
	idx[0]++;
	state = TAG_LEAGUENAMES;
    }
    else if(tag == TAG_GOAL)
    {
	idx[0] = (idx[0] + 1) % 2;
	if(idx[0] == 0)
	    idx[1]++;
	state = TAG_GOALS;
    }
    else if(tag >= TAG_GOAL_MINUTE &&
	    tag <=  TAG_GOAL_TYPE)
	state = TAG_GOAL;
    else if(tag == TAG_SEASON_STAT)
	state = TAG_SEASON_STATS;
    else if((tag >= TAG_SEASON_STAT_SEASON &&
	     tag <= TAG_SEASON_STAT_TEAMNAMES) ||
	    tag == TAG_SEASON_STAT_PLAYER_STATS)
	state = TAG_SEASON_STAT;
    else if(tag == TAG_SEASON_STAT_TEAMNAME)
    {
	idx[0]++;
	state = TAG_SEASON_STAT_TEAMNAMES;
    }
    else if(tag == TAG_SEASON_STAT_PLAYER_STAT)
    {
	idx[1]++;
	state = TAG_SEASON_STAT_PLAYER_STATS;
    }
    else if(tag >= TAG_SEASON_STAT_PLAYER_NAME &&
	    tag <= TAG_SEASON_STAT_PLAYER_GAMES)
	state = TAG_SEASON_STAT_PLAYER_STAT;
    else if(tag == TAG_STAD)
    {
	idx[0]++;
	state = TAG_STADS;
    }
    else if(tag == TAG_STAD_FACT0)
    {
	idx[0]++;
	state = TAG_STAD_FACTS;
    }
    else if(tag == TAG_STAD_FACT1)
    {
	idx[1]++;
	state = TAG_STAD_FACTS;
    }
    else if(tag >= TAG_STAD_CAP &&
	    tag <= TAG_STAD_SAF)
	state = TAG_STAD;
    else if(tag == TAG_OPT)
    {
	idx[0]++;
	state = TAG_OPTS;
    }
    else if(tag == TAG_COUNT)
    {
	idx[0]++;
	state = TAG_COUNTS;
    }
    else if(tag == TAG_RANK ||
	    tag == TAG_RANK_ID)
    {
	if(tag == TAG_RANK_ID)
	    idx[0]++;
	state = TAG_RANKS;
    }
    else if(tag == TAG_FIN)
    {
	idx[0]++;
	state = TAG_FINS;
    }
    else if(tag >= TAG_INJ0 &&
	    tag <= TAG_BOOK1)
    {
	if(tag == TAG_INJ1)
	    idx[0]++;
	else if(tag == TAG_BOOK1)
	    idx[1]++;
	state = TAG_INJBOOKS;
    }
    else if(tag == TAG_TRANSFER)
    {
	idx[0]++;
	state = TAG_TRANSFERS;
    }
    else if(tag >= TAG_TRANSFER_TEAMID &&
	    tag <= TAG_TRANSFER_ESTIMATEWAGE)
	state = TAG_TRANSFER;
    else if(tag == TAG_SEASON_OBJECTIVE)
    {
	idx[0]++;
	state = TAG_SEASON_OBJECTIVES;	
    }
    else if(tag == TAG_SEASON_OBJECTIVE_TYPE ||
	    tag == TAG_SEASON_OBJECTIVE_EXTRA_DATA)
	state = TAG_SEASON_OBJECTIVE;
    else if(tag !=  TAG_GENERAL)
    {
	g_warning("xml_general_read_end_element: unknown tag: %s\n",
		  element_name);
	g_warning("I'm in state %d\n", state);
    }
}

void
xml_general_read_text         (GMarkupParseContext *context,
			       const gchar         *text,
			       gsize                text_len,  
			       gpointer             user_data,
			       GError             **error)
{
    gchar buf[SMALL];
    gint int_value = -1;
    gfloat float_value = -1;

    strncpy(buf, text, text_len);
    buf[text_len] = '\0';

    int_value = (gint)g_ascii_strtod(buf, NULL);
    float_value = (gfloat)g_ascii_strtod(buf, NULL) / FLOAT_FACTOR;
    
    if(state == TAG_MYTEAM)
	my_team = int_value;
    else if(state == TAG_SEASON)
	season = int_value;
    else if(state == TAG_WEEK)
	week = int_value;        
    else if(state == TAG_SCOUT)
	scout = int_value;        
    else if(state == TAG_PHYSIO)
	physio = int_value;
    else if(state == TAG_FONTNAME)
	strcpy(font_name, buf);
    else if(state == TAG_COUNTRYFILENAME)
	strcpy(country_file_name, buf);
    else if(state == TAG_GOAL_MINUTE)
	goals[idx[0]][idx[1]].minute = int_value;    
    else if(state == TAG_GOAL_TEAMID)
    	goals[idx[0]][idx[1]].team_id = int_value;
    else if(state == TAG_GOAL_SCORER)
    	goals[idx[0]][idx[1]].scorer = int_value;
    else if(state == TAG_GOAL_TIME)
    	goals[idx[0]][idx[1]].time = int_value;
    else if(state == TAG_GOAL_TYPE)
    	goals[idx[0]][idx[1]].type = int_value;
    else if(state == TAG_SEASON_STAT_SEASON)
	seasonstat1->season_number = int_value;
    else if(state == TAG_SEASON_STAT_MYLEAGUE)
	seasonstat1->my_league = int_value;
    else if(state == TAG_SEASON_STAT_MYRANK)
	seasonstat1->my_rank = int_value;
    else if(state == TAG_SEASON_STAT_TEAMNAME)
	strcpy(seasonstat1->team_names[idx[0]], buf);
    else if(state == TAG_SEASON_STAT_PLAYER_NAME)
	strcpy(seasonstat1->best_players[idx[1]].name, buf);
    else if(state == TAG_SEASON_STAT_PLAYER_TEAMNAME)
	strcpy(seasonstat1->best_players[idx[1]].team_name, buf);
    else if(state == TAG_SEASON_STAT_PLAYER_GOALS)
	seasonstat1->best_players[idx[1]].goals = int_value;
    else if(state == TAG_SEASON_STAT_PLAYER_GAMES)
	seasonstat1->best_players[idx[1]].games = int_value;
    else if(state == TAG_STAD_FACT0)
	stadium_facts[0][idx[0]] = int_value;
    else if(state == TAG_STAD_FACT1)
	stadium_facts[1][idx[1]] = int_value;
    else if(state == TAG_STAD_CAP)
	stadiums[idx[0]].capacity = int_value;
    else if(state == TAG_STAD_AVATT)
	stadiums[idx[0]].average_attendance = int_value;
    else if(state == TAG_STAD_GAMES)
	stadiums[idx[0]].games = int_value;
    else if(state == TAG_STAD_SAF)
	stadiums[idx[0]].safety = float_value;
    else if(state == TAG_OPT)
	options[idx[0]] = int_value;
    else if(state == TAG_COUNT)
	counters[idx[0]] = int_value;
    else if(state == TAG_RANK)
	rank[idx[0]] = int_value;
    else if(state == TAG_RANK_ID)
	rank_ids[idx[0]] = int_value;
    else if(state == TAG_FIN)
	finances[idx[0]] = int_value;
    else if(state == TAG_LEAGUENAME)
	strcpy(league_names[idx[0]], buf);
    else if(state == TAG_INJ0)
	injuries[0][idx[0]] = int_value;
    else if(state == TAG_INJ1)
	injuries[1][idx[0]] = int_value;
    else if(state == TAG_BOOK0)
	booked[0][idx[1]] = int_value;
    else if(state == TAG_BOOK1)
	booked[1][idx[1]] = int_value;
    else if(state == TAG_TRANSFER_TEAMID)
	transferlist[idx[0]].team_id = int_value;
    else if(state == TAG_TRANSFER_PLAYERNR)
	transferlist[idx[0]].player_number = int_value;
    else if(state == TAG_TRANSFER_TIME)
	transferlist[idx[0]].time = int_value;
    else if(state == TAG_TRANSFER_ESTIMATEVALUE)
	transferlist[idx[0]].estimates[0] = int_value;
    else if(state == TAG_TRANSFER_ESTIMATEWAGE)
	transferlist[idx[0]].estimates[1] = int_value;
    else if(state == TAG_SEASON_OBJECTIVE_TYPE)
	seasonObjective[idx[0]].type = int_value;
    else if(state == TAG_SEASON_OBJECTIVE_EXTRA_DATA)
	seasonObjective[idx[0]].extradata = int_value;
}

void
xml_general_read(gchar *file_name)
{
    gchar file[SMALL];
    GMarkupParser parser = {xml_general_read_start_element,
			    xml_general_read_end_element,
			    xml_general_read_text, NULL, NULL};
    GMarkupParseContext *context;
    gchar *file_contents;
    gint length;
    GError *error = NULL;

    sprintf(file, "%s_%s.xml", file_name, XML_FILE_EXT_GENERAL);

    context = 
	g_markup_parse_context_new(&parser, 0, NULL, NULL);

    if(!g_file_get_contents(file, &file_contents, &length, &error))
    {
	g_critical("xml_general_read: error reading file %s\n", file);
	print_error(error, TRUE);
	return;
    }

    free_history();

    progress += PROGRESS_GENERAL;
    show_progress(progress / PROGRESS_MAX, _("Loading the rest..."));

    if(g_markup_parse_context_parse(context, file_contents, length, &error))
    {
	g_markup_parse_context_end_parse(context, NULL);	
	g_markup_parse_context_free(context);
	g_free(file_contents);
    }
    else
    {
	g_critical("xml_general_read: error parsing file %s\n", file);
	print_error(error, TRUE);
    }
}

void
xml_general_write(gchar *file_name)
{
    gint i;
    gchar file[SMALL];
    FILE *xml_file;

    sprintf(file, "%s_%s.xml", file_name, XML_FILE_EXT_GENERAL);

    progress += PROGRESS_GENERAL;
    show_progress(progress / PROGRESS_MAX, _("Saving the rest..."));

    my_fopen(file, "w", &xml_file, FALSE);

    xml_write_init(xml_file, TAG_GENERAL);

    xml_general_write_single(xml_file);
    xml_general_write_goals(xml_file);
    xml_general_write_stadiums(xml_file);
    xml_general_write_transfers(xml_file);
    xml_general_write_season_stats(xml_file);
    xml_general_write_season_objectives(xml_file);

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_OPTS);
    for(i=0;i<OPT_END;i++)
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_OPT, options[i], TAG_OPT);
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_OPTS);

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_COUNTS);
    for(i=0;i<COUNT_END;i++)
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_COUNT, counters[i], TAG_COUNT);
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_COUNTS);

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_RANKS);
    for(i=0;i<130;i++)
    {
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_RANK, rank[i], TAG_RANK);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_RANK_ID, rank_ids[i], TAG_RANK_ID);
    }
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_RANKS);

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_FINS);
    for(i=0;i<FIN_END;i++)
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, TAG_FIN, finances[i], TAG_FIN);
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_FINS);
    
    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_LEAGUENAMES);
    for(i=0;i<LEAGUE_NAMES_END;i++)
	fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT1, TAG_LEAGUENAME,
		league_names[i], TAG_LEAGUENAME);
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_LEAGUENAMES);
    
    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_INJBOOKS);
    for(i=0;i<12;i++)
    {
	fprintf(xml_file, "%s<_%d>%d</_%d>\n%s<_%d>%d</_%d>\n",
		INDENT1, TAG_INJ0, injuries[0][i], TAG_INJ0,
		INDENT1, TAG_INJ1, injuries[1][i], TAG_INJ1);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n%s<_%d>%d</_%d>\n",
		INDENT1, TAG_BOOK0, booked[0][i], TAG_BOOK0,
		INDENT1, TAG_BOOK1, booked[1][i], TAG_BOOK1);
    }
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_INJBOOKS);

    xml_write_end(xml_file, TAG_GENERAL);

    fclose(xml_file);
}

void
xml_general_write_single(FILE *xml_file)
{
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT0,
	    TAG_MYTEAM, my_team, TAG_MYTEAM);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT0,
	    TAG_SEASON, season, TAG_SEASON);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT0,
	    TAG_WEEK, week, TAG_WEEK);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT0,
	    TAG_SCOUT, scout, TAG_SCOUT);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT0,
	    TAG_PHYSIO, physio, TAG_PHYSIO);
    fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT0,
	    TAG_FONTNAME, font_name, TAG_FONTNAME);
    fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT0,
	    TAG_COUNTRYFILENAME, country_file_name, TAG_COUNTRYFILENAME);

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_STAD_FACTS);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n%s<_%d>%d</_%d>\n",
	    INDENT0, TAG_STAD_FACT0, stadium_facts[0][0], TAG_STAD_FACT0,
	    INDENT0, TAG_STAD_FACT0, stadium_facts[0][1], TAG_STAD_FACT0);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n%s<_%d>%d</_%d>\n",
	    INDENT0, TAG_STAD_FACT1, stadium_facts[1][0], TAG_STAD_FACT1,
	    INDENT0, TAG_STAD_FACT1, stadium_facts[1][1], TAG_STAD_FACT1);
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_STAD_FACTS);
}

void
xml_general_write_goals(FILE *xml_file)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_GOALS);
    for(i=0;i<50;i++)
    {
	xml_general_write_goal(&goals[0][i], xml_file);
	xml_general_write_goal(&goals[1][i], xml_file);
    }
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_GOALS);
}

void
xml_general_write_goal(goal * gl, FILE * xml_file)
{
    fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_GOAL);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_GOAL_MINUTE, gl->minute, TAG_GOAL_MINUTE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_GOAL_TEAMID, gl->team_id, TAG_GOAL_TEAMID);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_GOAL_SCORER, gl->scorer, TAG_GOAL_SCORER);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_GOAL_TIME, gl->time, TAG_GOAL_TIME);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_GOAL_TYPE, gl->type, TAG_GOAL_TYPE);
    fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_GOAL);
}

void
xml_general_write_stadiums(FILE *xml_file)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_STADS);
    
    for(i=0;i<178;i++)
    {
	fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_STAD);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_STAD_CAP, stadiums[i].capacity, TAG_STAD_CAP);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_STAD_AVATT, stadiums[i].average_attendance, TAG_STAD_AVATT);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_STAD_GAMES, stadiums[i].games, TAG_STAD_GAMES);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_STAD_SAF, (gint)(stadiums[i].safety * FLOAT_FACTOR), TAG_STAD_SAF);
	fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_STAD);
    }

    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_STADS);
}

void
xml_general_write_transfers(FILE *xml_file)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_TRANSFERS);
    
    for(i=0;i<20;i++)
    {
	fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_TRANSFER);
	
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_TRANSFER_TEAMID, transferlist[i].team_id, TAG_TRANSFER_TEAMID);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_TRANSFER_PLAYERNR, transferlist[i].player_number, TAG_TRANSFER_PLAYERNR);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_TRANSFER_TIME, transferlist[i].time, TAG_TRANSFER_TIME);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_TRANSFER_ESTIMATEVALUE, transferlist[i].estimates[0], TAG_TRANSFER_ESTIMATEVALUE);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
		TAG_TRANSFER_ESTIMATEWAGE, transferlist[i].estimates[1], TAG_TRANSFER_ESTIMATEWAGE);

	fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_TRANSFER);
    }

    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_TRANSFERS);
}

void
xml_general_write_season_stats(FILE *xml_file)
{
    season_stat *stat = history;

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_SEASON_STATS);
    while(stat != NULL)
    {
	xml_general_write_season_stat(xml_file, *stat);
	stat = stat->next;
    }
    
    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_SEASON_STATS);
}

void
xml_general_write_season_stat(FILE *xml_file, season_stat stat)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_SEASON_STAT);
    
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_SEASON_STAT_SEASON, stat.season_number, TAG_SEASON_STAT_SEASON);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_SEASON_STAT_MYLEAGUE, stat.my_league, TAG_SEASON_STAT_MYLEAGUE);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2,
	    TAG_SEASON_STAT_MYRANK, stat.my_rank, TAG_SEASON_STAT_MYRANK);

    fprintf(xml_file, "%s<_%d>\n", INDENT2, TAG_SEASON_STAT_TEAMNAMES);
    for(i=0;i<STAT_END;i++)
	fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT3, 
		TAG_SEASON_STAT_TEAMNAME, stat.team_names[i], TAG_SEASON_STAT_TEAMNAME);
    fprintf(xml_file, "%s</_%d>\n", INDENT2, TAG_SEASON_STAT_TEAMNAMES);

    fprintf(xml_file, "%s<_%d>\n", INDENT2, TAG_SEASON_STAT_PLAYER_STATS);
    for(i=0;i<6;i++)
	xml_general_write_season_stat_player(xml_file, stat.best_players[i]);
    fprintf(xml_file, "%s</_%d>\n", INDENT2, TAG_SEASON_STAT_PLAYER_STATS);

    fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_SEASON_STAT);
}

void
xml_general_write_season_stat_player(FILE *xml_file, player_stat plst)
{
    fprintf(xml_file, "%s<_%d>\n", INDENT3, TAG_SEASON_STAT_PLAYER_STAT);

    fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT4, 
	    TAG_SEASON_STAT_PLAYER_NAME, plst.name, TAG_SEASON_STAT_PLAYER_NAME);
    fprintf(xml_file, "%s<_%d>%s</_%d>\n", INDENT4, 
	    TAG_SEASON_STAT_PLAYER_TEAMNAME, plst.team_name, TAG_SEASON_STAT_PLAYER_TEAMNAME);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT4, 
	    TAG_SEASON_STAT_PLAYER_GOALS, plst.goals, TAG_SEASON_STAT_PLAYER_GOALS);
    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT4, 
	    TAG_SEASON_STAT_PLAYER_GAMES, plst.games, TAG_SEASON_STAT_PLAYER_GAMES);

    fprintf(xml_file, "%s</_%d>\n", INDENT3, TAG_SEASON_STAT_PLAYER_STAT);
}

void
xml_general_write_season_objectives(FILE *xml_file)
{
    gint i;

    fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_SEASON_OBJECTIVES);

    for(i=0;i<MAX_OBJECTIVE;i++)
    {
	fprintf(xml_file, "%s<_%d>\n", INDENT1, TAG_SEASON_OBJECTIVE);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2, TAG_SEASON_OBJECTIVE_TYPE,
		seasonObjective[i].type, TAG_SEASON_OBJECTIVE_TYPE);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT2, TAG_SEASON_OBJECTIVE_EXTRA_DATA,
		seasonObjective[i].extradata, TAG_SEASON_OBJECTIVE_EXTRA_DATA);	
	fprintf(xml_file, "%s</_%d>\n", INDENT1, TAG_SEASON_OBJECTIVE);
    }

    fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_SEASON_OBJECTIVES);
}
