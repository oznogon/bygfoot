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
show_graph(GtkWidget *graph_window, gint team_id,
	   gint player_number, gboolean player_history,
	   gint type)
{
    gint i;
    gfloat value;
    player *pl = NULL;
    GArray *vector = g_array_new(FALSE, FALSE, sizeof(gint));
    GArray *float_vector = g_array_new(FALSE, FALSE, sizeof(gfloat));
    GtkWidget *curve_graph =
	lookup_widget(graph_window, "curve_graph");

    if(player_history)
    {
	pl = &teams[team_id].players[player_number];
	get_history_values(pl->history, vector, type, PLAYER_HISTORY_END);
    }
    else
	get_history_values(teams[team_id].history,
			   vector, type, TEAM_HISTORY_END);

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
	if(player_history && type == PLAYER_HISTORY_SKILL)
	    value /= 10;
	g_array_append_val(float_vector, value);
    }

    set_up_graph(graph_window, pl, float_vector, type);

    gtk_curve_set_vector(GTK_CURVE(curve_graph), float_vector->len,
			 (gfloat*)float_vector->data);
    gtk_curve_set_curve_type(GTK_CURVE(curve_graph), GTK_CURVE_TYPE_LINEAR);

    g_array_free(vector, TRUE);
}

/* set up the x-y bounds for the graph and the write the labels */
void
set_up_graph(GtkWidget *graph_window, player *pl,
	     GArray *vector, gint type)
{
    gint i;
    gint precision = 0;
    gfloat bounds[2][2];
    gchar buf[SMALL];
    GtkWidget *curve_graph =
	lookup_widget(graph_window, "curve_graph");
    GtkWidget *labels[5];
    GtkWidget *label_title =
	lookup_widget(graph_window, "label_title");
    GtkWidget *hruler_graph =
	lookup_widget(graph_window, "hruler_graph");

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
		precision = 1;
		break;
	    case TEAM_HISTORY_PTS:
		sprintf(buf, "Points development");
		break;
	    case TEAM_HISTORY_GD:
		sprintf(buf, "Goal difference development");
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

    gtk_ruler_set_range(GTK_RULER(hruler_graph),
			0, vector->len, 0, vector->len);

    bounds[0][0] = 0;
    bounds[0][1] = vector->len;
    bounds[1][0] = max_float_array((gfloat*)vector->data, vector->len, TRUE) * 0.9;
    bounds[1][1] = max_float_array((gfloat*)vector->data, vector->len, FALSE) * 1.1;

    gtk_curve_set_range(GTK_CURVE(curve_graph), bounds[0][0],
			bounds[0][1], bounds[1][0], bounds[1][1]);

    for(i=0;i<5;i++)
	label_set_text_from_float(GTK_LABEL(labels[i]), 
				  bounds[1][0] + ((bounds[1][1] - bounds[1][0]) * (gfloat)i / 4),
				  0, precision);
}
