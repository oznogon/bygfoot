#include "bygfoot.h"
#include "graph.h"
#include "gui.h"
#include "history.h"
#include "maths.h"
#include "misc2_callbacks.h"
#include "support.h"
#include "window.h"


/* show the graph window with a skill graph or such */
void
show_graph(GtkWidget *graph_window, team *tm, player *pl, gint type)
{
    gint i;
    gfloat value;
    GArray *vector = g_array_new(FALSE, FALSE, sizeof(gint));
    GArray *float_vector = g_array_new(FALSE, FALSE, sizeof(gfloat));
    GtkWidget *curve_graph =
	lookup_widget(graph_window, "curve_graph");

    if(tm == NULL)
	get_player_history_values(*pl, vector, type);
    else
	get_team_history_values(*tm, vector, type);

    if(vector->len == 0)
    {
	show_popup_window("There is no history yet.", NULL);
	on_button_close_clicked(
	    GTK_BUTTON(lookup_widget(graph_window, "button_close")), NULL);
	return;
    }

    for(i=0;i<vector->len;i++)
    {
	value = (gfloat)g_array_index(vector, gint, i);
	if(tm == NULL && type == PLAYER_HISTORY_SKILL)
	    value /= 10;
	g_array_append_val(float_vector, value);
    }

    set_up_graph(graph_window, tm, pl, float_vector, type);

    gtk_curve_set_vector(GTK_CURVE(curve_graph), float_vector->len,
			 (gfloat*)float_vector->data);
    gtk_curve_set_curve_type(GTK_CURVE(curve_graph), GTK_CURVE_TYPE_LINEAR);

    g_array_free(vector, TRUE);
}

/* set up the x-y bounds for the graph and the write the labels */
void
set_up_graph(GtkWidget *graph_window, team *tm, player *pl,
	     GArray *vector, gint type)
{
    gfloat bounds[2][2];
    gint league_bounds[2];
    gint team_id = (tm == NULL) ? pl->team_id : tm->id;
    GtkWidget *curve_graph =
	lookup_widget(graph_window, "curve_graph");

    write_hruler_range(graph_window, tm, pl);
    
    bounds[0][0] = 0;
    bounds[0][1] = vector->len;
    bounds[1][0] = max_float_array((gfloat*)vector->data, vector->len, TRUE);
    bounds[1][1] = max_float_array((gfloat*)vector->data, vector->len, FALSE);
    
    gtk_curve_set_range(GTK_CURVE(curve_graph), bounds[0][0],
			bounds[0][1], bounds[1][0], bounds[1][1]);

    if(tm != NULL && type == TEAM_HISTORY_RANK)
    {
	get_league_bounds(get_league_from_id(team_id), league_bounds);
	bounds[1][0] = (league_bounds[1] - league_bounds[0]) - bounds[1][0];
	bounds[1][1] = (league_bounds[1] - league_bounds[0]) - bounds[1][1];
    }

    write_graph_labels(graph_window, pl, type, bounds);
}

void
write_graph_labels(GtkWidget *graph_window, player *pl, gint type, gfloat bounds[][2])
{
    gint i;
    gint precision = 0;
    gchar buf[SMALL];
    GtkWidget *labels[5];
    GtkWidget *label_title =
	lookup_widget(graph_window, "label_title");

    labels[0] =
	lookup_widget(graph_window, "label40");
    labels[1] =
	lookup_widget(graph_window, "label39");
    labels[2] =
	lookup_widget(graph_window, "label38");
    labels[3] =
	lookup_widget(graph_window, "label37");
    labels[4] =
	lookup_widget(graph_window, "label36");
    
    if(pl != NULL)
	switch(type)
	{
	    default:
		sprintf(buf, "Skill development for %s", pl->name);
		precision = 1;
		break;
	    case PLAYER_HISTORY_GOALS:
		sprintf(buf, "Goals development for %s", pl->name);
		break;
	    case PLAYER_HISTORY_WAGE:
		sprintf(buf, "Wage development for %s", pl->name);
	    case PLAYER_HISTORY_VALUE:
		sprintf(buf, "Value development for %s", pl->name);
		break;
	}
    else
	switch(type)
	{
	    default:
		sprintf(buf, "Rank development");
		break;
	    case TEAM_HISTORY_PTS:
		sprintf(buf, "Points development");
		break;
	    case TEAM_HISTORY_GD:
		sprintf(buf, "Goal difference development");
		break;
	    case TEAM_HISTORY_GF:
		sprintf(buf, "Goals for development");
		break;
	    case TEAM_HISTORY_GA:
		sprintf(buf, "Goals against development");
		break;
	    case TEAM_HISTORY_MONEY:
		sprintf(buf, "Money development");
		break;
	    case TEAM_HISTORY_AV_ATTENDANCE:
		sprintf(buf, "Average attendance development");
		break;
	}

    gtk_label_set_text(GTK_LABEL(label_title), buf);

    for(i=0;i<5;i++)
	label_set_text_from_float(GTK_LABEL(labels[i]), 
				  bounds[1][0] + ((bounds[1][1] - bounds[1][0]) * (gfloat)i / 4),
				  0, precision);
}

void
write_hruler_range(GtkWidget *graph_window, team *tm, player *pl)
{
    GtkWidget *hruler_week =
	lookup_widget(graph_window, "hruler_week");
    GtkWidget *hruler_season =
	lookup_widget(graph_window, "hruler_season");
    GArray *history = (tm == NULL) ? pl->history : tm->history;
    gint first_week;
    gint last_week;
    gint first_season;
    gint last_season;

    if(tm == NULL)
    {
	first_week = g_array_index(history, player_history, 0).values[PLAYER_HISTORY_WEEK] + 
	    (g_array_index(history, player_history, 0).values[PLAYER_HISTORY_SEASON] - 1) * 49;
	last_week = g_array_index(history, player_history, history->len - 1).values[PLAYER_HISTORY_WEEK] + 
	    (g_array_index(history, player_history, history->len - 1).values[PLAYER_HISTORY_SEASON] - 1) * 49;
	first_season = g_array_index(history, player_history, 0).values[PLAYER_HISTORY_SEASON];
	last_season = g_array_index(history, player_history, history->len - 1).values[PLAYER_HISTORY_SEASON];	
    }
    else
    {
	first_week = g_array_index(history, team_history, 0).values[TEAM_HISTORY_WEEK] + 
	    (g_array_index(history, team_history, 0).values[TEAM_HISTORY_SEASON] - 1) * 49;
	last_week = g_array_index(history, team_history, history->len - 1).values[TEAM_HISTORY_WEEK] + 
	    (g_array_index(history, team_history, history->len - 1).values[TEAM_HISTORY_SEASON] - 1) * 49;
	first_season = g_array_index(history, team_history, 0).values[TEAM_HISTORY_SEASON];
	last_season = g_array_index(history, team_history, history->len - 1).values[TEAM_HISTORY_SEASON];
    }
    
    gtk_ruler_set_range(GTK_RULER(hruler_week),
			first_week, last_week, first_week, last_week);

    gtk_ruler_set_range(GTK_RULER(hruler_season),
			first_season, last_season, first_season, last_season);
}
