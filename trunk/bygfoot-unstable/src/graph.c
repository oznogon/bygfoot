#include "graph.h"
#include "gui.h"
#include "maths.h"
#include "misc2_callbacks.h"
#include "support.h"
#include "window.h"


/* show the graph window with a skill graph or such */
void
show_graph(GtkWidget *graph_window, gint team_id,
	   gint player_number, gint type)
{
    gint veclen;
    GArray *vector = 
	g_array_new(FALSE, TRUE, sizeof(gfloat));
    GtkWidget *curve_graph =
	lookup_widget(graph_window, "curve_graph");

    if(type < HISTORY_MONEY)
	get_history_player(team_id, player_number, &veclen,
			   vector, type);

    if(vector->len == 0)
    {
	show_popup_window("There is no history yet.", NULL);
	on_button_close_clicked(
	    GTK_BUTTON(lookup_widget(graph_window, "button_close")), NULL);
	return;
    }

    set_up_graph(graph_window, team_id, player_number,
		 veclen, vector, type);

    gtk_curve_set_vector(GTK_CURVE(curve_graph), veclen, (gfloat*)vector->data);
    gtk_curve_set_curve_type(GTK_CURVE(curve_graph), GTK_CURVE_TYPE_LINEAR);

    g_array_free(vector, TRUE);
}

/* set up the x-y bounds for the graph and the write the labels accordingly */
void
set_up_graph(GtkWidget *graph_window, gint team_id, gint player_number,
	     gint veclen, GArray *array, gint type)
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
    GtkWidget *label_space =
	lookup_widget(graph_window, "label_space");
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
    
    switch(type)
    {
	default:
	    sprintf(buf, "Skill development for %s",
		    teams[team_id].players[player_number].name);
	    precision = 1;
	    break;
	case HISTORY_GOALS:
	    sprintf(buf, "Goals development for %s",
		    teams[team_id].players[player_number].name);
	    break;
	case HISTORY_WAGE:
	    sprintf(buf, "Wage development for %s",
		    teams[team_id].players[player_number].name);
	case HISTORY_VALUE:
	    sprintf(buf, "Value development for %s",
		    teams[team_id].players[player_number].name);
	    break;
	case HISTORY_MONEY:
	    sprintf(buf, "Your money development");
	    break;
    }

    gtk_label_set_text(GTK_LABEL(label_title), buf);

    gtk_ruler_set_range(GTK_RULER(hruler_graph),
			0, veclen, 0, veclen);

    bounds[0][0] = 0;
    bounds[0][1] = veclen;
    bounds[1][0] = max_float_array((gfloat*)array->data, veclen, TRUE) * 0.9;
    bounds[1][1] = max_float_array((gfloat*)array->data, veclen, FALSE) * 1.1;

    gtk_curve_set_range(GTK_CURVE(curve_graph), bounds[0][0],
			bounds[0][1], bounds[1][0], bounds[1][1]);

    for(i=0;i<5;i++)
	label_set_text_from_float(GTK_LABEL(labels[i]), 
				  bounds[1][0] + ((bounds[1][1] - bounds[1][0]) * (gfloat)i / 4),
				  0, precision);

    strcpy(buf, "----");
    for(i=0;i<strlen(gtk_label_get_text(GTK_LABEL(labels[0])));i++)
	strcat(buf, "-");

    gtk_label_set_text(GTK_LABEL(label_space), buf);
}
