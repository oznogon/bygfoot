#ifndef GRAPH_H
#define GRAPH_H

#include "bygfoot.h"
#include "variables.h"

/* show the graph window with a skill graph or such */
void
show_graph(GtkWidget *graph_window, team *tm, player *pl, gint type);

/* set up the x-y bounds for the graph and the write the labels */
void
set_up_graph(GtkWidget *graph_window, team *tm, player *pl,
	     GArray *vector, gint type);

void
write_graph_labels(GtkWidget *graph_window, player *pl, gint type, gfloat bounds[][2]);

#endif
