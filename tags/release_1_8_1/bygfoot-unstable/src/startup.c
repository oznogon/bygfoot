/**************************************************
 * Functions that are called at the beginning     *
 * of the game                                    *
 **************************************************/

#include "callbacks.h"
#include "fixture.h"
#include "generation.h"
#include "maths.h"
#include "misc.h"
#include "startup.h"
#include "support.h"
#include "team.h"
#include "files.h"
#include "treeview.h"
#include "window.h"

/* write default options and other
   global variables.
   meanings of the options see non_gui_defs.h */
void
write_global_variables(gint new_game)
{
    gint i;

    my_team = -1;

    /* default tables */
    for(i=0;i<130;i++)
    {
	if(i < 20)
	    rank[i] = i + 1;
	else if(i < 44)
	    rank[i] = i - 19;
	else if(i < 68)
	    rank[i] = i - 43;
	else if(i < 92)
	    rank[i] = i - 67;
	else if(i < 114)
	    rank[i] = i - 91;
	else
	    rank[i] = (i - 2) % 4 + 1;

	rank_ids[i] = i;
    }
    
    season = week = 1;

    history = NULL;
    
    for(i=0;i<20;i++)
	transferlist[i].time = -1;

    for(i=0;i<FIX_END;i++)
	fixtures[i].type = -1;

    if(new_game == 1)
	return;

    scout = physio = 3;

    /* default penalty shooter: noone */
    options[OPT_PENALTY_SHOOTER] = -1;
}

/* start a new game: generate teams etc. */
void
start_new_game(gint new_game)
{
    /* write default options and other
       global variables */
    write_global_variables(new_game);

    /* generate teams */
    generate_teams();

    write_season_fixtures();
}

/* show the initial team selection window */
void
start(gint new_game)
{
    start_new_game(new_game);
    
    show_team_selection();
}

void
set_my_team(gint team_id, gint league)
{    
    gint bound[2];
    gchar *name1 = teams[team_id].name;
    gchar *name2;

    get_league_bounds(league, bound);

    my_team = rndi(bound[0], bound[1] - 1);

    name2 = teams[my_team].name;

    swap_char(&name1, &name2);

    if(my_team > 114)
    {
	name1 = teams[team_id].name;
	name2 = teams[110].name;
	swap_char(&name1, &name2);
    }
}

/* get the id of the human player's team according
   to the list in the team selection window */
void
get_my_team(GtkWidget *widget)
{
    GtkWidget *list = lookup_widget(widget,
				    "team_selection_treeview");
    GtkWidget *team_selection_radio1 =
	lookup_widget(widget, "team_selection_radio1");
    GtkWidget *team_selection_radio3 =
	lookup_widget(widget, "team_selection_radio3");
    GtkWidget *radiobutton_cl = 
	lookup_widget(widget, "radiobutton_cl");
    GtkWidget *radiobutton_cwc = 
	lookup_widget(widget, "radiobutton_cwc");
    GtkWidget *radiobutton_uefa = 
	lookup_widget(widget, "radiobutton_uefa");

    my_team = get_index(GTK_TREE_VIEW(list)) - 1;

    if(gtk_toggle_button_get_active(
	   GTK_TOGGLE_BUTTON(team_selection_radio1)))
	set_my_team(my_team, 1);
    else if(gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(team_selection_radio3)))
	set_my_team(my_team, 5);
    else if(gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(radiobutton_cl)))
	set_my_team(my_team, 6);
    else if(gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(radiobutton_cwc)))
	set_my_team(my_team, 7);
    else if(gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(radiobutton_uefa)))
	set_my_team(my_team, 8);
}
