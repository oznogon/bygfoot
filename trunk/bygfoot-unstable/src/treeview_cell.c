/*********************************************************
 * Functions working with the cells of the treeviews     *
 *********************************************************/

#include "fixture.h"
#include "game.h"
#include "gui.h"
#include "maths.h"
#include "misc.h"
#include "team.h"
#include "treeview_cell.h"

/* print some information about the injury of
   the player into 'buf' */
void
set_injury_info_into_cell(gint team_id, gint player_number,
			  gchar *buf)
{
    gint health[2] =
	{get_place(teams[team_id].players[player_number].health, 4) +
	 get_place(teams[team_id].players[player_number].health, 5) * 10,
	 get_place(teams[team_id].players[player_number].health, 22)};
    gchar *injury[11] =
	{"concussion",
	 "hamstring",
	 "pulled muscle",
	 "groin injury",
	 "fractured ankle",
	 "cracked rib",
	 "broken leg",
	 "broken ankle",
	 "broken arm",
	 "broken shoulder",
	 "torn crucial ligament"};

    sprintf(buf,
	    "Injured: %s\n(expected recovery in %d weeks)",
	    injury[health[0] - 1], health[1]);
}

/* print some information about why the player's banned into 'buf' */
void
set_ban_info_into_cell(gint team_id, gint player_number,
		       gchar *buf)
{
    gchar *ban[8] =
	{"repeated dissent",
	 "repeated delayed play",
	 "deliberate, goal-blocking foul",
	 "repeated bad tackle",
	 "dangerous tackle",
	 "violent conduct",
	 "spat at player",
	 "fifth yellow card"};

    sprintf(buf, "banned for %d weeks (%s)",
	    teams[team_id].players[player_number].booked % 10,
	    ban[get_place(teams[team_id].players[player_number].booked, 11) - 1]);
}

void
set_status_into_cell(GtkCellRenderer *renderer,
		     gint team_id, 
		     gint player_number,
		     gchar *buf,
		     gint type)
{
    gint health = teams[team_id].players[player_number].health;
    gint booked = teams[team_id].players[player_number].booked % 10;

    if(health + booked == 0)
	sprintf(buf, "OK");
    else if(health > 0)
    {
	g_object_set(renderer, "background", "Lightgreen", NULL);
	if(type < 5)
	    sprintf(buf, "INJ (%d)",
		    health % 100);
	else
	    set_injury_info_into_cell(team_id, player_number, buf);
    }
    else if(booked > 0)
    {
	g_object_set(renderer, "background", "Red", NULL);
	if(type < 5)
	    sprintf(buf, "BAN (%d)",
		    teams[team_id].players[player_number].
		    booked % 10);
	else
	    set_ban_info_into_cell(team_id, player_number, buf);
    }
}

void
set_position_into_cell(GtkCellRenderer *renderer,
		       gint team_id, 
		       gint player_number,
		       gint position,
		       gchar *buf)
{
    gint value = (position == 1) ?
	teams[team_id].players[player_number].pos :
	teams[team_id].players[player_number].cpos;

    g_object_set(renderer, "foreground",
		 "White", NULL);
    
    if(value == 0)
    {
	sprintf(buf, "G");
	g_object_set(renderer, "background", "Black",
		     NULL);
    }
    else
    {
	if(value == 1)
	{
	    sprintf(buf, "D");
	    g_object_set(renderer, "background",
			 "Darkgreen", NULL);
	}
	else if(value == 2)
	{
	    sprintf(buf, "M");
	    g_object_set(renderer, "background",
			 "Darkblue", NULL);
	}
	else
	{
	    sprintf(buf, "F");
	    g_object_set(renderer, "background",
			 "Darkred", NULL);
	}	
    }
}

void
cdf_player_column(GtkTreeViewColumn *col,
		  GtkCellRenderer   *renderer,
		  GtkTreeModel      *model,
		  GtkTreeIter       *iter,
		  gpointer           user_data)
{
    gchar buf[SMALL];
    gint team_id, player_number, attribute;
    gint value;
    gint type;
    gint column = GPOINTER_TO_INT(user_data);
    gfloat fitness_percent;
    gchar fitness_color[SMALL];

    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    gtk_tree_model_get(model, iter, column, &value, -1);

    team_id = get_place(value, 5) + 
	get_place(value, 6) * 10 + get_place(value, 7) * 100;
    player_number = get_place(value, 3) + get_place(value, 4) * 10;
    attribute = get_place(value, 22);
    type = get_place(value, 8);

    if(type == 1 && team_id == my_team)
	g_object_set(renderer, "background",
		     "Darkblue", "foreground", "White", NULL);
    else if(my_team == 114 && (player_number == 12 ||
			       player_number == 15 ||
			       player_number == 18) )
	g_object_set(renderer, "background",
		     "Lightgrey", "foreground", "Black", NULL);
    else
	g_object_set(renderer, "background",
		     "White", "foreground", "Black", NULL);

    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);
    
    if(attribute < 0)
	strcpy(buf, "");
    else if(attribute == ATT_NAME)
    {
	sprintf(buf, "%s", 
		teams[team_id].players[player_number].name);

	if(team_id == my_team && 
	   player_number == options[OPT_PENALTY_SHOOTER])
	    strcat(buf, " (P)");
    }
    else if(attribute == ATT_CPOS)
	set_position_into_cell(renderer, team_id, player_number, 0,
			       buf);
    else if(attribute == ATT_POS)
	set_position_into_cell(renderer, team_id, player_number, 1,
			       buf);
    else if(attribute == ATT_SKILL)
	sprintf(buf, "%.1f",
		teams[team_id].players[player_number].
		skill);
    else if(attribute == ATT_TALENT)
	sprintf(buf, "%.1f",
		teams[team_id].players[player_number].
		talent);
    else if(attribute == ATT_CSKILL)
    {
	sprintf(buf, "%.1f",
		teams[team_id].players[player_number].
		cskill *
		powf(teams[team_id].players[player_number].
		     fitness, 0.25));

	if(teams[team_id].players[player_number].cskill <
	   teams[team_id].players[player_number].skill)
	    g_object_set(renderer,
			 "background", "Lightblue",
			 "foreground", "Red", NULL);
    }
    else if(attribute == ATT_FIT)
    {
	fitness_percent = teams[team_id].players[player_number].fitness * 100;
	sprintf(buf, "%.0f%%",
		teams[team_id].players[player_number].
		fitness * 100);

	if (fitness_percent < 60)
	    strcpy(fitness_color, "Red");
	else if(fitness_percent < 70)
	    strcpy(fitness_color, "OrangeRed");
	else if(fitness_percent < 85)
	    strcpy(fitness_color, "DarkOrange");
	else
	    strcpy(fitness_color, "DarkGreen");

	if(options[OPT_FIT_COLORS] == 0)
	    strcpy(fitness_color, "Black");

	g_object_set(renderer,
		     "background", "White",
		     "foreground", fitness_color, NULL);	
    }
    else if(attribute == ATT_GAMES)
	sprintf(buf, "%d",
		teams[team_id].players[player_number].
		games);
    else if(attribute == ATT_GOALS)
	sprintf(buf, "%d",
		teams[team_id].players[player_number].
		goals);
    else if(attribute == ATT_STATUS)
	set_status_into_cell(renderer, team_id, player_number,
			     buf, type);
    else if(attribute == ATT_AGE)
	sprintf(buf, "%.0f",
		teams[team_id].players[player_number].
		age);
    else if(attribute == ATT_ETAL)
	sprintf(buf, "%.1f",
		teams[team_id].players[player_number].
		etal);
    else if(attribute == ATT_BOOKED)
	sprintf(buf, "%d",
		get_place(teams[team_id].players[player_number].
			  booked, 2));
    else if(attribute == ATT_VALUE)
	print_grouped_int(teams[team_id].players[player_number].
			  value, buf, 0);
    else if(attribute == ATT_WAGE)
	print_grouped_int(teams[team_id].players[player_number].
			  wage, buf, 0);
    else if(attribute == ATT_TEAM)
	sprintf(buf, "%s",
		teams[team_id].name);
    else if(attribute == ATT_LEAGUE)
	get_league_name_from_id(team_id, buf);
    else if(attribute == ATT_TALENT)
	sprintf(buf, "%.1f",
		teams[team_id].players[player_number].
		talent);
    else
	g_print("cdf_player_column: unknown attribute: %d",
		attribute);
    
    g_object_set(renderer, "text", buf, NULL);
}

void
league_to_string(gint league, gchar *buf)
{
    if(league == -50)
	strcpy(buf, "");
    else if(league > 0 && league < 10)
	sprintf(buf, "%s", league_names[LEAGUE_NAMES_LEAGUE1 + (league - 1)]);
    else if(league == 11)
	sprintf(buf, "%s", league_names[LEAGUE_NAMES_LEAGUE]);
    else if(league == 25)
	sprintf(buf, "%s", league_names[LEAGUE_NAMES_CHARITY]);
    else if(league == 35)
	sprintf(buf, "%s", league_names[LEAGUE_NAMES_UEFA_SUPER]);
    else
	sprintf(buf, "%s", league_names[LEAGUE_NAMES_PROMOTION]);
}

/* a cell data function (cdf) that converts the gint league value
   (of teams) into a string like "Premier Division" or "Division 2" */
void
cdf_league_to_cell (GtkTreeViewColumn *col,
		    GtkCellRenderer   *renderer,
		    GtkTreeModel      *model,
		    GtkTreeIter       *iter,
		    gpointer           user_data)
{
    gint column = GPOINTER_TO_INT(user_data);
    gint league = -1;
    gchar  buf[SMALL];

    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    gtk_tree_model_get(model, iter, column, &league, -1);

    league_to_string(league, buf);

    g_object_set(renderer, "text", buf, NULL);
}

/* display an int value  */
void
cdf_int_to_cell (GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data)
{
    gint column = GPOINTER_TO_INT(user_data);
    gint value;
    gchar  buf[SMALL];

    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    gtk_tree_model_get(model, iter, column, &value, -1);
    
    if(value == -5)
	strcpy(buf, "");
    else
	sprintf(buf, "%d", value);

    g_object_set(renderer, "text", buf, NULL);
}

/* print something like 'Champions' League Semi-finals'.
   probably the ugliest part of the whole program */
void
fixture_type_to_string(gint type, gint column, gchar *buf)
{
    if(column == 0)
    {
	if(get_place(type, 12) != 11 &&
	   type < 25000)
	    league_to_string(get_place(type, 11), buf);
	else
	    league_to_string(get_place(type, 12), buf);

	return;
    }

    if(type == 9600 || type == 11600)
	sprintf(buf, "1st Round");
    else if(type == 9500 || type == 11500)
	sprintf(buf, "2nd Round");
    else if(get_place(type, 12) == 80 ||
	    get_place(type, 12) == 81 ||
	    type == 9400 || type == 11400)
	sprintf(buf, "1/16 Final");
    else if( get_place(type, 12) == 78 ||
	     get_place(type, 12) == 82 ||
	     type == 9300 || type == 11300)
	sprintf(buf, "1/8 Final");
    else if( get_place(type, 12) == 64 ||
	     get_place(type, 12) == 74 ||
	     get_place(type, 12) == 84 ||
	     type == 9200 || type == 11200)
	sprintf(buf, "1/4 Final");
    else if( get_place(type, 12) == 62 ||
	     get_place(type, 12) == 72 ||
	     get_place(type, 12) == 86 ||
	     type == 9100 || type == 11100)
	sprintf(buf, "1/2 Final");
    else if( get_place(type, 12) == 60 ||
	     get_place(type, 12) == 70 ||
	     get_place(type, 12) == 88 ||
	     type == 9000 || type == 11000)
	sprintf(buf, "Final");
    else if(type >= 50000)
    {
	if(type < 55000)
	    sprintf(buf, "1st Round");
	else
	    sprintf(buf, "2nd Round");
    }

    if( (get_place(type, 11) >= 6 &&
	 get_place(type, 11) <= 8 &&
	 type != 6500 && type % 1000 != 0 ) )
    {
	if(get_place(type, 12) == 65)
	    strcat(buf, "Round robin");
	else if(type % 10 == 0)
	    strcat(buf, " 1st Leg");
	else
	    strcat(buf, " 2nd Leg");
    }

    if(type > 9000 && type < 12000)
    {
	if(type % 1000 == 6000)
	    sprintf(buf, "1st Round");
	else if(type % 1000 == 5000)
	    sprintf(buf, "2nd Round");
    }

}

/* print the result of a game into a string */
void
fixture_result_to_string(gint fix_idx, gchar *buf)
{
    gint idx = (fix_idx < 0 ) ?
	fix_idx + 1000000 : fix_idx;
    gint res_idx0 = (fix_idx < 0 ) ? 1 : 0;
    gint res_idx1 = (fix_idx < 0 ) ? 0 : 1;

    if(fixtures[idx].week_number >= week)
    {
	sprintf(buf, "-- : --");
	return;
    }

    /* no extra time */
    if(fixtures[idx].result[res_idx0][1] + 
       fixtures[idx].result[res_idx1][1] +
       fixtures[idx].result[res_idx0][2] + 
       fixtures[idx].result[res_idx1][2] == 0)
	sprintf(buf, "%d - %d", fixtures[idx].result[res_idx0][0],
		fixtures[idx].result[res_idx1][0]);
    /* extra time, no penalties */
    else if(fixtures[idx].result[res_idx0][2] + 
	    fixtures[idx].result[res_idx1][2] == 0)
	sprintf(buf, "%d - %d e.t.", fixtures[idx].result[res_idx0][0] +
		fixtures[idx].result[res_idx0][1],
		fixtures[idx].result[res_idx1][0] +
		fixtures[idx].result[res_idx1][1]);
    else
	sprintf(buf, "%d - %d\n%d - %d\npen.",
		fixtures[idx].result[res_idx0][0] +
		fixtures[idx].result[res_idx0][1],
		fixtures[idx].result[res_idx1][0] +
		fixtures[idx].result[res_idx1][1],
		fixtures[idx].result[res_idx0][2],
		fixtures[idx].result[res_idx1][2]);
}

/* render a cell in a fixture list */
void
cdf_fixture_column(GtkTreeViewColumn *col,
		   GtkCellRenderer   *renderer,
		   GtkTreeModel      *model,
		   GtkTreeIter       *iter,
		   gpointer           user_data)
{
    gint column = GPOINTER_TO_INT(user_data);
    gint value;
    gint id;
    gint goal_ids[2];
    gchar  buf[SMALL];

    gtk_tree_model_get(model, iter, column, &value, -1);

    g_object_set(renderer, "background",
		 "White", "foreground", "Black", NULL);
    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    if(column == 0)
    {
	if(((value < 3000000 && value > 1000000) ||
	    value == -100 || value == -400 ||
	    value == -410))
	    g_object_set(renderer,
			 "background", "Lightgrey",
			 "foreground", "Black", NULL);

	else if(value < -510 && value > -514)
	    g_object_set(renderer,
			 "background", "Orangered",
			 "foreground", "Black", NULL);
    }
    
    /* a goal with scorer name and minute */
    if(value >= 3000000)
    {
	value -= 3000000;
	goal_ids[0] = get_place(value, 3);
	goal_ids[1] = get_place(value, 22);
	sprintf(buf, "%s %d.",
		teams[goals[goal_ids[0]][goal_ids[1]].team_id].
		players[goals[goal_ids[0]][goal_ids[1]].scorer].name,
		goals[goal_ids[0]][goal_ids[1]].minute);
	if(goals[goal_ids[0]][goal_ids[1]].type == GOAL_TYPE_OWN)
	    strcat(buf, " (own goal)");
	else if(goals[goal_ids[0]][goal_ids[1]].type == GOAL_TYPE_PEN)
	    strcat(buf, " (penalty)");
	else if(goals[goal_ids[0]][goal_ids[1]].type >= GOAL_TYPE_PEN_MISSED &&
		goals[goal_ids[0]][goal_ids[1]].type <= GOAL_TYPE_PEN_CROSS)
	{
	    g_object_set(renderer, "background", "tomato", NULL);

	    if(goals[goal_ids[0]][goal_ids[1]].type == GOAL_TYPE_PEN_MISSED)
		strcat(buf, " (penalty; missed)");
	    else if(goals[goal_ids[0]][goal_ids[1]].type == GOAL_TYPE_PEN_SAVE)
		strcat(buf, " (penalty; save)");
	    else if(goals[goal_ids[0]][goal_ids[1]].type == GOAL_TYPE_PEN_POST)
		strcat(buf, " (penalty; post)");
	    else if(goals[goal_ids[0]][goal_ids[1]].type == GOAL_TYPE_PEN_CROSS)
		strcat(buf, " (penalty; cross-bar)");
	}
    }
    /* type of league */
    else if(value > 2000000)
	fixture_type_to_string(value - 2000000, column, buf);
    /* week number */
    else if(value > 1000000)
	sprintf(buf, "Week %d", value - 1000000);
    else if(value == -100)
	sprintf(buf, "Injuries / Cards");
    else if(value == -400)
	sprintf(buf, "Attendance:");
    else if(value == -410)
	sprintf(buf, "Attendance (neutral stadium):");
    else if(value == -501 || value == -500)
	sprintf(buf, "%d", stadium_facts[(value + 500) * -1][0]);
    else if(value == -511)
	sprintf(buf, "There were technical\nproblems in your stadium.");
    else if(value == -512)
	sprintf(buf, "There were riots\nin your stadium.");
    else if(value == -513)
	sprintf(buf, "There was a fire\nin your stadium.");
    else if(value <= -1000 && value >= -1100)
    {
	value = (value + 1000) * -1;
	if(teams[injuries[(column !=0)][11]].players[value].booked % 10 == 0)
	    g_object_set(renderer, "background",
			 "Yellow", "foreground", "Black", NULL);
	else
	    g_object_set(renderer, "background",
			 "Red", "foreground", "Black", NULL);

	sprintf(buf, "%s", teams[injuries[(column !=0)][11]].players[value].name);
    }
    else if(value <= -800 && value >= -900)
    {
	value = (value + 800) * -1;
	g_object_set(renderer, "background",
		     "Lightgreen", "foreground", "Black", NULL);
	sprintf(buf, "%s", teams[injuries[(column != 0)][11]].players[value].name);
    }
    else if(value == -5)
	strcpy(buf, "");
    else
    {
	if(my_team_involved(fixtures[value]))
	    g_object_set(renderer, "background",
			 "Darkblue", "foreground", "White", NULL);
	
	if(column == 0 || column == 2)
	{
	    id = fixtures[value].team_id[column / 2];
	    if(fixtures[value].type < 6000 ||
	       fixtures[value].type > 50000)
		sprintf(buf, "%s [%d]",
			teams[teams[id].id].name,
			rank[id]);
	    else
		sprintf(buf, "%s (%d)",
			teams[teams[id].id].name,
			get_league_from_id(id));
	}
	else
	    fixture_result_to_string(value, buf);
    }
    
    g_object_set(renderer, "text", buf, NULL);
}

void
team_id_to_table_title(gint team_id, gchar *buf)
{
    gint i;

    if(team_id < 114)
	get_league_name_from_id(team_id, buf);
    else
    {
	for(i=0;i<4;i++)
	    if(team_id > 113 + i * 4 &&
	       team_id < 118 + i * 4)
		sprintf(buf, "CL Group %d", i + 1);
    }
}

void
team_id_to_table_string(gint team_id, gchar *buf,
			gint column, GtkCellRenderer *renderer)
{
    gint i;
    gchar background_color[50],
	foreground_color[50];

    if(teams[team_id].id == my_team &&
       column == 1)
	g_object_set(renderer, "background",
		     "Darkblue", "foreground", "White", NULL);
    else
    {
	get_table_colors(team_id, background_color, foreground_color);
	g_object_set(renderer, "background", background_color,
		     "foreground", foreground_color, NULL);
    }
    
    if(column == 1)
	sprintf(buf, "%s", teams[teams[team_id].id].name);
    for(i=2;i<10;i++)
	if(column == i)
	{
	    if(i < 8)
		sprintf(buf, "%d", teams[team_id].results[i - 2]);
	    else if(i == 8)
		sprintf(buf, "%+d", 
			teams[team_id].results[RES_GF] - 
			teams[team_id].results[RES_GA]);
	    else
		sprintf(buf, "%d", teams[team_id].results[RES_PTS]);
	}
}

void
cdf_table_column(GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data)
{
    gchar buf[SMALL];
    gint value;
    gint column = GPOINTER_TO_INT(user_data);

    gtk_tree_model_get(model, iter, column, &value, -1);

    g_object_set(renderer, "background",
		 "White", "foreground", "Black", NULL);
    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    if(value  >= 1000000)
	team_id_to_table_title(value - 1000000, buf);
    else if(value == -5)
	strcpy(buf, "");
    else if(column == 0)
	sprintf(buf, "%d", value);
    else
	team_id_to_table_string(value, buf, column, renderer);

    g_object_set(renderer, "text", buf, NULL);
}

void
set_result_to_string(gint team_id, gchar *buf,
		     gint away)
{
    gint i;

    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type != -1 &&
	   fixtures[i].type < 6000 &&
	   fixtures[i].team_id[away] == my_team &&
	   fixtures[i].team_id[(away + 1) % 2] == team_id)
	    break;
    
    fixture_result_to_string(i - away * 1000000, buf);
}

void
cdf_league_result_column(GtkTreeViewColumn *col,
			 GtkCellRenderer   *renderer,
			 GtkTreeModel      *model,
			 GtkTreeIter       *iter,
			 gpointer           user_data)
{
    gchar buf[SMALL];
    gint value;
    gint column = GPOINTER_TO_INT(user_data);
    
    gtk_tree_model_get(model, iter, column, &value, -1);

    if(column != 1)
	g_object_set(renderer, "background",
		     "White", NULL);
    else
	g_object_set(renderer, "background",
		     "Lightgrey", NULL);

    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    if(column == 0)
	sprintf(buf, "%s", teams[value].name);
    else
	set_result_to_string(value, buf, column % 2);

    g_object_set(renderer, "text", buf, NULL);
}

void
cdf_best_players(GtkTreeViewColumn *col,
		 GtkCellRenderer   *renderer,
		 GtkTreeModel      *model,
		 GtkTreeIter       *iter,
		 gpointer           user_data)
{
    gchar buf[SMALL];
    gint value;
    gint ids[2];
    gint column = GPOINTER_TO_INT(user_data);
    
    gtk_tree_model_get(model, iter, column, &value, -1);

    g_object_set(renderer, "background",
		 "White", "foreground", "Black", NULL);
    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);
        
    if(value == -5)
	strcpy(buf, "");
    else if(value == -10)
	sprintf(buf, "Best field players");
    else if(value == -20)
	sprintf(buf, "Best goalies");
    else if(value < -20)
    {
	value += 100;
	get_league_name_from_id(value * 19, buf);
    }
    else
    {
	ids[0] = get_place(value, 3) + get_place(value, 4) * 10 +
	    get_place(value, 5) * 100;
	ids[1] = get_place(value, 22);

	if(ids[0] == my_team)
	    g_object_set(renderer, "background",
			 "Darkblue", "foreground",
			 "White", NULL);

	if(column == 1)
	    sprintf(buf, "%s",
		    teams[ids[0]].players[ids[1]].name);
	else if(column == 2)
	    sprintf(buf, "%s",
		    teams[ids[0]].name);
	else if(column == 3)
	    sprintf(buf, "%d",
		    teams[ids[0]].players[ids[1]].goals);
	else if(column == 4)
	{
	    if(teams[ids[0]].players[ids[1]].games > 0)
		sprintf(buf, "%.1f",
			(gfloat)teams[ids[0]].players[ids[1]].goals /
			(gfloat)teams[ids[0]].players[ids[1]].games);
	    else
		sprintf(buf, "0");
	}
    }

    g_object_set(renderer, "text", buf, NULL);
}

void
cdf_opponents (GtkTreeViewColumn *col,
	       GtkCellRenderer   *renderer,
	       GtkTreeModel      *model,
	       GtkTreeIter       *iter,
	       gpointer           user_data)
{
    g_object_set(renderer, "text", "", NULL);
}

/* set the result of previous games against one team
   into a string */
void
my_result_to_string(gint fix_idx, gchar *buf)
{
    gchar buf2[SMALL];

    winner_of_regular(fixtures[fix_idx], buf2,
		      (fixtures[fix_idx].team_id[0] != my_team));
    strcat(buf, buf2);
    
    fixture_result_to_string(fix_idx, buf2);

    strcat(buf, buf2);

    if(fixtures[fix_idx].type % 1000 == 0 && 
       fixtures[fix_idx].type > 5000)
	strcat(buf, " (N)  ");
    else if(fixtures[fix_idx].team_id[0] == my_team)
	strcat(buf, " (H)  ");
    else
	strcat(buf, " (A)  ");
}

/* write the previous results of the human player's team
   against a certain cpu team into a row */
void
write_my_results(GtkListStore *liststore, gint team_id)
{
    gint i;
    gchar buf[SMALL];
    GtkTreeIter iter;
    
    strcpy(buf, "");
    for(i=0;i<FIX_END;i++)
	if(fixtures[i].type != -1 &&
	   my_team_involved(fixtures[i]) &&
	   (fixtures[i].team_id[0] == team_id ||
	    fixtures[i].team_id[1] == team_id) &&
	   fixtures[i].week_number < week)
	    my_result_to_string(i, buf);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Your results", 2, buf, -1);
}

/* write the outcomes of the last games of a team in
   'WWLLDW...' form into a row */
void
write_last_games(GtkListStore *liststore, gint team_id)
{
    gint nr_games = 7;
    gint i, k;
    gint week_number = week - 1;
    gint goals[2] = {0, 0};
    gint goals_for_idx;
    gchar buf[SMALL];
    gchar results[nr_games][SMALL];
    GtkTreeIter iter;
    
    strcpy(buf, "");
    for(i=0;i<nr_games;i++)
	strcpy(results[i], "");

    k = 0;
    while(week_number >= 0 && k < nr_games)
    {
	for(i = FIX_END - 1; i >= 0; i--)
	    if( fixtures[i].type != -1 &&
		(fixtures[i].team_id[0] == team_id ||
		 fixtures[i].team_id[1] == team_id) &&
		fixtures[i].week_number == week_number )
	    {
		goals_for_idx = (fixtures[i].team_id[0] != team_id);
		goals[0] +=
		    fixtures[i].result[goals_for_idx][0];
		goals[1] +=
		    fixtures[i].result[(goals_for_idx + 1 ) % 2][0];
		winner_of_regular(fixtures[i], results[k], 
				  (fixtures[i].team_id[0] != team_id));
		k++;
	    }
	week_number--;
    }

    for(i = nr_games - 1; i >= 0; i--)
	if(strlen(results[i]) > 0)
	    strcat(buf, results[i]);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Latest results", 2, buf, -1);

    sprintf(buf, "%d : %d", goals[0], goals[1]);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Goals", 2, buf, -1);
}

/* simulate a game and write the result as a bookmaker tip into the
   bookmaker variables */
void
write_bookmaker(GtkListStore *liststore, gint *opponent_ids, gint idx)
{
    gint i, j;
    gfloat home_advantage[2] = {1, 1};
    gchar buf[SMALL];
    gchar buf2[SMALL];
    fixture local_fixture;
    GtkTreeIter iter;

    local_fixture.type = fixtures[opponent_ids[idx + 4]].type;
    local_fixture.team_id[0] = fixtures[opponent_ids[idx + 4]].team_id[0];
    local_fixture.team_id[1] = fixtures[opponent_ids[idx + 4]].team_id[1];
    
    for(i=0;i<2;i++)
	for(j=0;j<3;j++)
	    local_fixture.result[i][j] = 0;

    if(local_fixture.type % 1000 != 0 || 
       local_fixture.type < 6000)
	home_advantage[0] =
	    gauss_dist(1.05, 1.05, 1.12, 1.12);
    
    /* tip not yet written */
    if(bookmaker_tips[idx][0] == -1)
    {
	process_result_game(&local_fixture, home_advantage,
			    0, TRUE);
	while(is_draw(local_fixture, 0))
	    process_result_game(&local_fixture, home_advantage,
				0, TRUE);
	
	bookmaker_tips[idx][0] = local_fixture.result[0][0];
	bookmaker_tips[idx][1] = local_fixture.result[1][0];
    } 
    else
    {
	local_fixture.result[0][0] = bookmaker_tips[idx][0];
	local_fixture.result[1][0]= bookmaker_tips[idx][1];
    }
        
    winner_of_regular(local_fixture, buf2,
		      (local_fixture.team_id[0] != my_team));
    sprintf(buf, "%d : %d", bookmaker_tips[idx][0],
	    bookmaker_tips[idx][1]);
    strcat(buf2, buf);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, opponent_ids[idx], 1, "Bookmaker's tip", 2, buf2, -1);
}

void
style_to_string(gint style, gchar *buf)
{
    if(style == -2)
	strcpy(buf, "All Out Defend");
    else if(style == -1)
	strcpy(buf, "Defend");
    else if(style == 0)
	strcpy(buf, "Balanced");
    else if(style == 1)
	strcpy(buf, "Attack");
    else if(style == 2)
	strcpy(buf, "All Out Attack");    
}

/* create the rows for the info on a certain team */
void
create_opponent_info(GtkListStore *liststore, gint *opponent_ids, gint idx)
{
    gint team_id = opponent_ids[idx];
    gchar buf[SMALL];
    gchar buf2[SMALL];
    GtkTreeIter iter;

    if(team_id == -1)
	return;

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Team", 2, teams[team_id].name, -1);

    if(team_id < 130)
    {
	league_to_string(get_league_from_id(team_id), buf2);
	sprintf(buf, "%d (%s)", rank[team_id], buf2);
    }
    else
    {
	league_to_string(get_league_from_id(team_id), buf2);
	sprintf(buf, "none (%s)", buf2);
    }

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Rank", 2, buf, -1);

    sprintf(buf, "%.2f (%+.2f)   %.2f (%+.2f)", average_skill(team_id, 11, TRUE), 
	    average_skill(team_id, 11, TRUE) - average_skill(my_team, 11, TRUE),
	    average_skill(team_id, 20, FALSE),
	    average_skill(team_id, 20, FALSE) - average_skill(my_team, 20, FALSE));
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Average cskills", 2, buf, -1);

    style_to_string(teams[team_id].style, buf);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Playing style", 2, buf, -1);

    sprintf(buf, "%d", teams[team_id].structure);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "Team structure", 2, buf, -1);

    write_last_games(liststore, team_id);
    write_my_results(liststore, team_id);
    write_bookmaker(liststore, opponent_ids, idx);
    
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, team_id, 1, "", 2, "", -1);
}

