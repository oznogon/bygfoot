#ifndef GRAPH_H
#define GRAPH_H

#include "bygfoot.h"

/* show the graph window with a skill graph or such */
void
show_graph(GtkWidget *graph_window, gint team_id, gint player_number,
	   gboolean player_history, gint type);

/* set up the x-y bounds for the graph and the write the labels */
void
set_up_graph(GtkWidget *graph_window, player *pl,
	     GArray *vector, gint type);

#endif
