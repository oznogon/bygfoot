/*********************************************************
 * Functions working with the GtkTreeViews of the GUI    *
 *********************************************************/

#include "finance.h"
#include "fixture.h"
#include "maths.h"
#include "misc.h"
#include "support.h"
#include "team.h"
#include "treeview_cell.h"
#include "treeview.h"

/* return the number in the first column of the currently
   selected row of the treeview */
gint
get_index(GtkTreeView *treeview)
{
    gint value;
    GtkTreeSelection *selection	= 
	gtk_tree_view_get_selection(treeview);
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    
    gtk_tree_selection_get_selected(selection, &model, &iter);

    gtk_tree_model_get(model, &iter, 0,
		       &value, -1);

    return value;
}

/* remove all columns in the treeview */
void
clear_treeview(GtkWidget *treeview)
{
    gint i;
    gint number_of_columns;
    GtkWidget *list = (treeview == NULL) ?
	lookup_widget(main_window, "player_info") :
	treeview;
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(list),
			    NULL);

    number_of_columns = 0;
    while(gtk_tree_view_get_column(GTK_TREE_VIEW(list),
				   number_of_columns) != NULL)
	number_of_columns++;

    for(i = number_of_columns - 1; i >= 0; i--)
	gtk_tree_view_remove_column(
	    GTK_TREE_VIEW(list),
	    gtk_tree_view_get_column(GTK_TREE_VIEW(list),
				     i));
}

/* create the model for the treeview in the team selection window.
   it contains the names and the divisions of all 114 english teams
   appearing in the game */
GtkTreeModel*
create_team_selection_list(gint european)
{
    gint i;
    GtkListStore  *liststore;
    GtkTreeIter iter;

    liststore = gtk_list_store_new(3,
				   G_TYPE_INT,
				   G_TYPE_STRING,
				   G_TYPE_INT);   
    
    /* fill the model with the team names and leagues */
    for(i=0;i<114 + european * 61;i++)
    {
	if(i != 114 && i != 130 && i != my_team)
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, i + 1,
			       1, teams[i].name,
			       2, get_league_from_id(i),
			       -1);
	}
    }

    return GTK_TREE_MODEL(liststore);
}

/* set up the tree view for the team selection window */
void
set_up_team_selection_treeview (GtkWidget *treeview)
{
    GtkTreeViewColumn   *col;
    GtkCellRenderer     *renderer;

    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_BROWSE);
    
    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview),
				 TRUE);

    /* number the teams */
    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer,
				       "text", 0);
    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    /* set up team name column */
    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "Team");
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer,
				       "text", 1);
    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);

    /* league column */
    col = gtk_tree_view_column_new();
    gtk_tree_view_column_set_title(col, "League");
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);

    /* connect the renderer to our int_to_cell league conversion
       function */
    gtk_tree_view_column_set_cell_data_func(col, renderer,
					    cdf_league_to_cell,
					    GINT_TO_POINTER(2), NULL);
}

/* show a list of teams in the treeview; if 'european'
   is 1, show also the european teams */
void
show_team_list(GtkWidget *treeview, gint european)
{
    GtkTreeModel *team_list = 
	create_team_selection_list(european);
    GtkTreeSelection *selection;
     
    clear_treeview(treeview);

    set_up_team_selection_treeview(treeview);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),
			    team_list);
    
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    gtk_tree_selection_select_path(selection,
				   gtk_tree_path_new_from_string("0"));
    
    g_object_unref(team_list);
}

/* the sort function for the transfer list */
gint
sort_column(GtkTreeModel *model,
		 GtkTreeIter *a,
		 GtkTreeIter *b,
		 gpointer user_data)
{
    gint i;
    GValue value = {0, };
    gfloat compare_values[2] = {0, 0};
    gint team_id[2], player_number[2];
    GtkTreeIter *iters[2] = {a, b};
    gint attrib = GPOINTER_TO_INT(user_data);

    for(i=0;i<2;i++)
    {
	gtk_tree_model_get_value(model, iters[i], 1, &value);

	team_id[i] = get_place(g_value_get_int(&value), 5) + 
	    get_place(g_value_get_int(&value), 6) * 10 +
	    get_place(g_value_get_int(&value), 7) * 100;
	player_number[i] = get_place(g_value_get_int(&value), 3) +
	    get_place(g_value_get_int(&value), 4) * 10;
	
	g_value_unset(&value);
    }

    if(attrib == ATT_POS)
	for(i=0;i<2;i++)
	    compare_values[i] = (gfloat)teams[team_id[i]].players[player_number[i]].pos;
    else if(attrib == ATT_SKILL)
	for(i=0;i<2;i++)
	    compare_values[i] = teams[team_id[i]].players[player_number[i]].skill;
    else if(attrib == ATT_GAMES)
	for(i=0;i<2;i++)
	    compare_values[i] = (gfloat)teams[team_id[i]].players[player_number[i]].games;
    else if(attrib == ATT_GOALS)
	for(i=0;i<2;i++)
	    compare_values[i] = (gfloat)teams[team_id[i]].players[player_number[i]].goals;
    else if(attrib == ATT_AGE)
	for(i=0;i<2;i++)
	    compare_values[i] = teams[team_id[i]].players[player_number[i]].age;
    else if(attrib == ATT_ETAL)
	for(i=0;i<2;i++)
	    compare_values[i] = teams[team_id[i]].players[player_number[i]].etal;
    else if(attrib == ATT_LEAGUE)
	for(i=0;i<2;i++)
	    compare_values[i] = (gfloat)get_league_from_id(team_id[i]);	

    if(compare_values[0] > compare_values[1])
	return 1;
    else if(compare_values[0] < compare_values[1])
	return -1;

    return 0;
}

/* create a player liststore of the players with
 team_ids and player_numbers in ids[][];
 'attrib' determines which columns are shown */
GtkTreeModel*
create_player_list(gint ids[][2], gint max,
				 gint separator,
				 gint *attrib, gint type)
{
    gint i, j, k;
    gint number_of_cols = 1;
    gint add;
    GtkListStore  *liststore;
    GtkTreeIter iter;
    GType types[ATT_DUMMY1 + 1];

    add = 0;

    /* transferlist */
    if(type == 10)
	add = 10000000;

    types[0] = G_TYPE_INT;

    k = 1;
    for(i=0;i<ATT_DUMMY1;i++)
	if(attrib[i] == 1)
	{
	    types[k] = G_TYPE_INT;
	    k++;
	    number_of_cols++;
	}

    liststore = gtk_list_store_newv(number_of_cols, types);
    
    /* fill the model */
    for(i=0;i<max;i++)
    {
	gtk_list_store_append(liststore, &iter);
	
	if(i == separator)
	{
	    gtk_list_store_set(liststore, &iter, 0, -5, -1);
	    for(j=1;j<number_of_cols;j++)
		gtk_list_store_set(liststore, &iter, j,
				   -5, -1);
	    
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter, 0, i + 1, -1);
	}
	else
	    gtk_list_store_set(liststore, &iter, 0, i + 1, -1);
	
	k = 1;
	for(j=0;j<ATT_DUMMY1;j++)
	    if(attrib[j] == 1)
	    {
		if(type == 10 && j != ATT_NAME && j != ATT_STATUS && j != ATT_TEAM)
		    gtk_tree_sortable_set_sort_func (GTK_TREE_SORTABLE(liststore),
						     k,
						     sort_column,
						     GINT_TO_POINTER(j), NULL);
		gtk_list_store_set(liststore, &iter, k,
				   add +
				   ids[i][0] * 10000 +
				   ids[i][1] * 100 +
				   j,
				   -1);
		k++;
	    }
    }


    return GTK_TREE_MODEL(liststore);
}

void
set_up_player_list_column(GtkWidget *treeview, gint column,
			       gint attribute, gint type)
{
    GtkTreeViewColumn   *col = gtk_tree_view_column_new();
    GtkCellRenderer     *renderer = gtk_cell_renderer_text_new();
    gchar *titles[ATT_DUMMY1] =
	{"Name",
	 "CPos", 
	 "Pos",
	 "CSk", 
	 "Sk",
	 "Talent",
	 "Fit",
	 "Ga", 
	 "Go",
	 "Status", 
	 "Age",
	 "ETal", 
	 "YC",
	 "Value",
	 "Wage", 
	 "Team",
	 "League"};
    
    if(attribute >= 0 && attribute < ATT_DUMMY1)
	gtk_tree_view_column_set_title(col, titles[attribute]);
    else
	g_print("set_up_player_list_column: unknown attribute: %d\n",
		attribute);

    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

    gtk_tree_view_column_pack_start(col, renderer, TRUE);

    gtk_tree_view_column_set_cell_data_func(col, renderer,
					    cdf_player_column,
					    GINT_TO_POINTER(
						column),
					    NULL);
    /* alignment */
    if( attribute != ATT_LEAGUE && 
	attribute != ATT_NAME &&
	attribute != ATT_TEAM &&
	(my_team != 114 || attribute != ATT_AGE))
    {
	gtk_tree_view_column_set_alignment(col, 0.5);
	g_object_set(renderer, "xalign", 0.5,
		     NULL);
    }    

    if(type == 10 && attribute != ATT_NAME && attribute != ATT_STATUS &&
       attribute != ATT_TEAM)
	gtk_tree_view_column_set_sort_column_id(col, column);
}

/* set up the tree view for a player list */
void
set_up_player_list (GtkWidget *player_list, gint *attrib, gint type)
{
    gint i, column;
    GtkTreeViewColumn   *col;
    GtkCellRenderer     *renderer;

    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(player_list)),
	GTK_SELECTION_SINGLE);
    
    /* number the players */
    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(player_list), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_cell_data_func(col, renderer,
					    cdf_int_to_cell,
					    GINT_TO_POINTER(0),
					    NULL);
    
    /* set up the columns for the attributes */
    column = 1;
    for(i=0;i<ATT_DUMMY1;i++)
	if(attrib[i] == 1)
	{	    
	    set_up_player_list_column(player_list, column, i, type);
	    column++;
	}
}

/* show the players with team_ids and player_numbers stored in
   'ids' from 0 to 'max' in 'treeview'; the attributes set to 1
   in 'attrib' are shown, the rest omitted.
   type = 0: a team
   type = 10: the transferlist */
void
show_players(GtkWidget *treeview, gint ids[][2], gint max,
		  gint *attrib, gint type)
{
    gint i;
    gint separator = (type == 10) ? -1 : 11;
    gint ids_local[20][2];
    GtkWidget *player_list = (treeview == NULL) ?
	lookup_widget(main_window, "player_list") :
	treeview;
    GtkTreeModel *player_list_model;
    gint local_attributes[ATT_DUMMY1];

    clear_treeview(player_list);

    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(player_list),
				      TRUE);
    if(ids == NULL)
    {
	max = 0;
	for(i=0;i<20;i++)
	{
	    if(teams[my_team].players[i].pos >= 0)
	    {
		ids_local[max][0] = my_team;
		ids_local[max][1] = i;
		max++;
	    }
	}
    }
    else
	for(i=0;i<20;i++)
	{
	    ids_local[i][0] = ids[i][0];
	    ids_local[i][1] = ids[i][1];
	}
    
    if(attrib == NULL)
	for(i=0;i<ATT_DUMMY1;i++)
	    local_attributes[i] = options[OPT_ATT_NAME + i];
    else
	for(i=0;i<ATT_DUMMY1;i++)
	    local_attributes[i] = attrib[i];

    set_up_player_list(player_list, local_attributes, type);
    player_list_model = create_player_list(ids_local, max, 
					   separator, local_attributes, type);

    gtk_tree_view_set_model(GTK_TREE_VIEW(player_list),
			    GTK_TREE_MODEL(player_list_model));
    
    g_object_unref(player_list_model);
}

/* set up the tree view for detailed info on a player */
void
set_up_player_info (GtkWidget *player_info)
{
    gint column;
    GtkTreeViewColumn   *col;
    GtkCellRenderer     *renderer;
    
    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(player_info)),
	GTK_SELECTION_NONE);

    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(player_info),
				      FALSE);
    
    /* first column: 'name', 'position' etc. */
    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(player_info), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_add_attribute(col, renderer,
				       "text", 0);
    if(strcmp(font_name, "0") != 0)
	g_object_set(renderer, "font", font_name, NULL);
    
    /* second column: values */
    column = 1;
    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(player_info), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_cell_data_func(col, renderer,
					    cdf_player_column,
					    GINT_TO_POINTER(1),
					    NULL);    
}

/* create the model for the detailed player info.
   in the first column we write the categories,
   in the second only the encoded team_id, player_number
   and attribute (just like in the create_player_list
   function) */
GtkTreeModel*
create_player_info(gint team_id, gint player_number)
{
    gint i;
    GtkListStore  *liststore;
    GtkTreeIter iter;
    gchar categories[ATT_DUMMY1][SMALL];

    strcpy(categories[ATT_NAME], "Name");
    strcpy(categories[ATT_CPOS], "Current position"); 
    strcpy(categories[ATT_POS], "(Normal) Position");
    strcpy(categories[ATT_CSKILL], "Current skill"); 
    strcpy(categories[ATT_SKILL], "(Normal) Skill");
    strcpy(categories[ATT_TALENT], "Talent");
    strcpy(categories[ATT_FIT], "Fitness");
    strcpy(categories[ATT_GAMES], "Games"); 
    strcpy(categories[ATT_GOALS], "Goals");
    strcpy(categories[ATT_STATUS], "Status"); 
    strcpy(categories[ATT_AGE], "Age");
    strcpy(categories[ATT_ETAL], "Estimated talent"); 
    strcpy(categories[ATT_BOOKED], "Yellow cards");
    strcpy(categories[ATT_VALUE], "Value");
    strcpy(categories[ATT_WAGE], "Wage"); 
    strcpy(categories[ATT_TEAM], "Team");
    strcpy(categories[ATT_LEAGUE], "League");

    liststore = gtk_list_store_new(2, G_TYPE_STRING,
				   G_TYPE_INT);
    
    /* fill the model */
    for(i=0;i<ATT_DUMMY1;i++)
    {
	if(i != ATT_TALENT || options[OPT_ATT_TALENT] == 1)
	{
	    gtk_list_store_append(liststore, &iter);	
	    gtk_list_store_set(liststore, &iter,
			       0, categories[i],
			       1, 50000000 + team_id * 10000 +
			       player_number * 100 + i,
			       -1);
	}
    }
    
    return GTK_TREE_MODEL(liststore);
}

/* show the players with team_ids and player_numbers stored in
   'ids' from 0 to 'max' in 'treeview'; the attributes set to 1
   in 'attrib' are shown, the rest omitted */
void
show_player_info(GtkWidget *treeview, gint team_id,
		      gint player_number)
{
    GtkWidget *player_info = (treeview == NULL) ?
	lookup_widget(main_window, "player_info") :
	treeview;
    GtkTreeModel *player_info_model;

    clear_treeview(NULL);

    set_up_player_info(player_info);
    
    player_info_model =
	create_player_info(team_id, player_number);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(player_info_model));
    
    g_object_unref(player_info_model);
}

void
set_attribs_from_scout(gint *attrib)
{
    gint i, local_scout = scout % 10;

    for(i=0;i<ATT_DUMMY1;i++)
	attrib[i] = 0;

    /* all scouts */
    attrib[ATT_NAME] = attrib[ATT_POS] = 
	attrib[ATT_SKILL] = attrib[ATT_TEAM] =
	attrib[ATT_LEAGUE] = 1;

    if(local_scout < 4)
	attrib[ATT_ETAL] = 1;
    
    if(local_scout < 3)
	attrib[ATT_AGE] = attrib[ATT_GAMES] = 1;

    if(local_scout < 2)
	attrib[ATT_STATUS] = attrib[ATT_GOALS] = 1;
}

/* show the transferlist */
void
show_transfers(void)
{
    gint i, max;
    gint ids[20][2];    
    gint attrib[ATT_DUMMY1];
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkWidget *player_info_separator =
	lookup_widget(main_window, "player_info_separator");
    GtkWidget *button_browse =
	lookup_widget(main_window, "button_browse");

    gtk_widget_hide(player_info_separator);
    gtk_widget_show(button_browse);

    max = 0;
    for(i=0;i<20;i++)
    {
	if(transferlist[i].time > 0)
	{
	    ids[max][0] = transferlist[i].team_id;
	    ids[max][1] = transferlist[i].player_number;
	    max++;
	}
    }

    /* determine which columns to show depending
       on the scout's quality */
    set_attribs_from_scout(attrib);
    
    show_players(player_info, ids, max, attrib, 10);
}

void
set_average_skills(gint team_id)
{
    GtkWidget *label_av_skill = 
	lookup_widget(main_window, "label_av_skill");
    gchar buf[SMALL];

    sprintf(buf, "%.2f (%+.2f)  %.2f (%+.2f)",
	    average_skill(team_id, 11, TRUE),
	    average_skill(team_id, 11, TRUE) - 
	    average_skill(my_team, 11, TRUE),
	    average_skill(team_id, 20, FALSE),
	    average_skill(team_id, 20, FALSE) - 
	    average_skill(my_team, 20, FALSE));
    gtk_label_set_text(GTK_LABEL(label_av_skill), buf);
}

/* show a team in 'team browse' mode so that the
   human player can try to buy a player not on
   the transferlist */
void
show_team_browse(gint team_id, GtkWidget *treeview)
{
    gint i;
    gint ids[20][2];
    gint attrib[ATT_DUMMY1];
    GtkWidget *treeview_local = (treeview == NULL) ?
	lookup_widget(main_window, "player_info") :
	treeview;

    gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(treeview_local),
				 FALSE);

    if(treeview == NULL)
	set_average_skills(team_id);

    for(i=0;i<20;i++)
    {
	ids[i][0] = team_id;
	ids[i][1] = i;
    }

    set_attribs_from_scout(attrib);
    
    show_players(treeview_local, ids, 20,
		 attrib, 0);
}

/* set up the treeview for displaying fixtures */
void
set_up_fixtures(GtkWidget *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;

    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      FALSE);
    
    /* columns: team 1, result, team 2 */
    for(i=0;i<3;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(col, renderer,
						cdf_fixture_column,
						GINT_TO_POINTER(i),
						NULL);
	/* alignment */
	g_object_set(renderer, "xalign", 1 - (gfloat)i / 2,
		     NULL);
    }

}

void
find_leagues(gint *leagues, gint week_number)
{
    gint k, i;

    if(get_place(status, 4) != 0)
    {
	leagues[0] = (get_place(status, 3) + 
		      get_place(status, 4) * 10) * 100;
	return;
    }

    i = 1;
    for(k = 0; k < FIX_END; k++)
	if(((i == 1 && fixtures[k].week_number >= week_number) ||
	    (i == 0 && fixtures[k].week_number == week_number)) &&
	   my_team_involved(fixtures[k]) &&
	   fixtures[k].type != -1 && 
	   fixtures[k].type != leagues[i] )
	{
	    if(i == 1 && fixtures[k].week_number > week_number)
		week_number = fixtures[k].week_number;
	    
	    if(i < 0)
	    {
		g_print("find_leagues: found too many fixtures for week %d team %d\n",
			week_number, my_team);
		return;
	    }
	    leagues[(i + 1) % 2] = fixtures[k].type;
	    i--;
	}
}

void
create_fixture_liststore(GtkListStore *liststore,
			      gint week_number)
{
    gint i, j, k;
    gint leagues[2] = {-10, -10};
    gint types[2];
    gint fixture_type = get_place(status, 3) +
	get_place(status, 4) * 10;
    gint places = 2;
    GtkTreeIter iter;

    find_leagues(leagues, week_number);

    for(i=0;i<2;i++)
    {	
	if(leagues[i] == -10)
	    return;
	
	types[0] = get_place(leagues[i], 12);
	types[1] = -10;

	if(fixture_type >= 60 && fixture_type <= 90)
	    places = 1;

	/* this is an exception: the only fixtures the types of which
	   have different first two digits although they belong together
	   are the uefa-cup 1/16 finals; the types are 8010-8160 and
	   8015-8165 (first and second leg, resp.) */
	if(get_place(leagues[i], 10 + places) == 80 ||
	   get_place(leagues[i], 10 + places) == 81)
	    types[1] = (get_place(leagues[i], 10 + places) == 81) ? 80 : 81;

	/* find the first fixture of the given type */
	for(k=0;k<FIX_END;k++)
	{
	    if( (get_place(fixtures[k].type, 10 + places) == get_place(types[0], 10 + places) || 
		 get_place(fixtures[k].type, 10 + places) == get_place(types[1], 10 + places) ) &&
		fixtures[k].week_number >= week_number &&
		fixtures[k].type != -1)
		break;
	}
	if(k == FIX_END)
	    return;

	if(fixtures[k].week_number > week_number)
	{
	    week_number = fixtures[k].week_number;
	    status = status - get_place(status, 22) + week_number;
	}

	/* fill in the first two rows which contain the week
	   number and the name of the league (and the type
	   like 'semi-final' etc) */
	if(i == 0)
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, week_number + 1000000,
			       1, -5, 2, -5, -1);
	}
	
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, fixtures[k].type + 2000000,
			   1, -5,
			   2, fixtures[k].type + 2000000, -1);

	/* a blank row */
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter, 0, -5, 1, -5, 2, -5, -1);   

	/* now fill the rest of the rows with the fixtures */
	while(fixtures[k].type != -1)
	{
	    if( (get_place(fixtures[k].type, 10 + places) == get_place(types[0], 10 + places) || 
		 get_place(fixtures[k].type, 10 + places) == get_place(types[1], 10 + places) ) &&
		fixtures[k].week_number == week_number )
	    {
		gtk_list_store_append(liststore, &iter);
		for(j=0;j<3;j++)
		    gtk_list_store_set(liststore, &iter, j,
				       k, -1);
	    }
	    k++;
	}
    
	/* a blank row */
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter, 0, -5, 1, -5, 2, -5, -1);   
    }
}

/* create a new model for fixtures and fill it or append
   new data to an existing model */
GtkTreeModel*
create_fixtures(gint week_number)
{
    GtkListStore *liststore =
	gtk_list_store_new(3, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);

    create_fixture_liststore(liststore,
			     week_number);
    
    return GTK_TREE_MODEL(liststore);
}

/* show the fixtures for the given week and leagues */
void
show_fixtures(gint week_number)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *fixture_model;

    clear_treeview(NULL);

    set_up_fixtures(player_info);

    fixture_model = create_fixtures(week_number);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(fixture_model));
    
    g_object_unref(fixture_model);
}

/* fill a table with the teams ranked between the two bounds */
void
fill_table_liststore(GtkListStore *liststore, gint *bound)
{
    gint i, j;
    GtkTreeIter iter;

    /* set the first row so that the name of the league
       will be displayed */
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, -5,
		       1, 1000000 + bound[0], -1);
    for(j=2;j<10;j++)
	gtk_list_store_set(liststore, &iter,
			   j, -5, -1);

    gtk_list_store_append(liststore, &iter);
    for(j=0;j<10;j++)
	gtk_list_store_set(liststore, &iter,
			   j, -5, -1);

    for(i=bound[0];i<bound[1];i++)
    {
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, i - bound[0] + 1, -1);
	for(j=1;j<10;j++)
	    gtk_list_store_set(liststore, &iter,
			       j, rank_ids[i], -1);
    }

    gtk_list_store_append(liststore, &iter);
    for(j=0;j<10;j++)
	gtk_list_store_set(liststore, &iter,
			   j, -5, -1);
}

/* create the model for a table */
GtkTreeModel*
create_tables(gint league)
{
    gint i;
    gint bound[2];
    GtkListStore *liststore =
	gtk_list_store_new(10, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
			   G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
			   G_TYPE_INT, G_TYPE_INT, G_TYPE_INT,
			   G_TYPE_INT);
    
    if(league < 6)
    {
	get_league_bounds(league, bound);
	fill_table_liststore(liststore, bound);
    }
    else
    {
	for(i=0;i<4;i++)
	{
	    bound[0] = 114 + i * 4;
	    bound[1] = bound[0] + 4;
	    
	    fill_table_liststore(liststore, bound);
	}
    }

    return GTK_TREE_MODEL(liststore);
}

/* set up the treeview for displaying a table */
void
set_up_table(GtkWidget *treeview)
{
    gint i;
    gchar *titles[10] = {"",
			 "Team",
			 "PL",
			 "W",
			 "DW",
			 "L",
			 "GF",
			 "GA",
			 "GD",
			 "Points"};
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;

    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      TRUE);
    
    /* columns: rank, name, games, won, lost, draw,
       goals for, goals against, goal difference,
       points */
    for(i=0;i<10;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, titles[i]);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(col, renderer,
						cdf_table_column,
						GINT_TO_POINTER(i),
						NULL);
	/* alignment */
	if(i < 2)
	{
	    g_object_set(renderer, "xalign", 1 - (gfloat)i,
			 NULL);
	    gtk_tree_view_column_set_alignment(col, 0.5);	
	}
	else
	{
	    g_object_set(renderer, "xalign", 0.5,
			 NULL);
	    gtk_tree_view_column_set_alignment(col, 0.5);
	}
    }
}

/* show tables of an english division or CL group */
void
show_tables_league(gint league)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *table_model;

    clear_treeview(NULL);

    set_up_table(player_info);

    table_model = create_tables(league);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(table_model));
    
    g_object_unref(table_model);
}

/* create the model for the league results */
GtkTreeModel*
create_league_results(void)
{
    gint i, j;
    gint league = get_league_from_id(my_team);
    gint bound[2];
    GtkListStore *liststore =
	gtk_list_store_new(3, G_TYPE_INT, 
			   G_TYPE_INT, G_TYPE_INT);
    GtkTreeIter iter;

    if(league == 1)
    {
	bound[0] = 0;
	bound[1] = 20;
    }
    else if(league < 5)
    {
	bound[0] = 20 + (league - 2) * 24;
	bound[1] = bound[0] + 24;
    }
    else
    {
	bound[0] = 92;
	bound[1] = 114;
    }

    for(i=bound[0];i<bound[1];i++)
    {
	if(rank_ids[i] != my_team)
	{
	    gtk_list_store_append(liststore, &iter);
	    for(j=0;j<3;j++)
		gtk_list_store_set(liststore, &iter,
				   j, rank_ids[i],
				   -1);
	}
    }
    
    return GTK_TREE_MODEL(liststore);
}

/* set up the treeview for displaying league results
   of the human player's team */
void
set_up_league_results(GtkWidget *treeview)
{
    gint i;
    gchar *titles[3] = {"Team",
			"Away",
			"Home"};
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;

    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      TRUE);
    
    /* columns: team, away result, home result */
    for(i=0;i<3;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, titles[i]);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(col, renderer,
						cdf_league_result_column,
						GINT_TO_POINTER(i),
						NULL);
	/* alignment */
	if(i == 1)
	{
	    g_object_set(renderer, "xalign", 0.5,
			 NULL);
	    gtk_tree_view_column_set_alignment(col, 0.5);
	}
	else
	{
	    g_object_set(renderer, "xalign", 0.0,
			 NULL);
	    gtk_tree_view_column_set_alignment(col, 0.0);	
	}
    }
}

/* show the human player's league results of the season
   by team */
void
show_league_results(void)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *league_result_model;

    clear_treeview(NULL);

    set_up_league_results(player_info);
    
    league_result_model = 
	create_league_results();
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(league_result_model));
    
    g_object_unref(league_result_model);
}

/* fill a model with finances and stadium info */
GtkTreeModel*
create_fin_stad(void)
{
    gint i;
    gchar buf[SMALL];
    gchar buf2[SMALL];
    gint capacity_increase = 
	stadium_improve_costs(0, -1);
    gint safety_increase =
	stadium_improve_costs(1, -1);
    GtkListStore *liststore =
	gtk_list_store_new(2, G_TYPE_STRING, 
			   G_TYPE_STRING);
    GtkTreeIter iter;
    gchar *categories[FIN_DUMMY1] =
	{"Money",
	 "Ticket sale",
	 "Journey costs",
	 "Player wages",
	 "Scout wage",
	 "Physio",
	 "Transfers",
	 "Stadium improvement",
	 "Stadium bills",
	 "Prize money",
	 "Debts"};

    if(counters[COUNT_LOAN] > 0)
	sprintf(buf2, "\nRepay in %d weeks\nCurrent interest rate: %.1f%%",
		counters[COUNT_LOAN], rint(interest_rate() * 1000) / 10);
    else if(counters[COUNT_LOAN] == 0)
	sprintf(buf2, " repay this week!!!");

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Finances", 1, "", -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "", 1, "", -1);

    for(i=0;i<FIN_DUMMY1;i++)
    {
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, categories[i], -1);
	print_grouped_int(finances[i], buf, 0);

	if(i == FIN_DEBTS && counters[COUNT_LOAN] >= -1)
	    strcat(buf, buf2);
	
	if(i == FIN_STADIUM)
	{
	    if(counters[COUNT_INC_CAP] < 0)
		strcat(buf, " (enlargement planned)");
	    else if(counters[COUNT_INC_CAP] > 0)
		strcat(buf, " (enlargement in progress)");
	}

	gtk_list_store_set(liststore, &iter,
			   1, buf, -1);
    }

    print_grouped_int(calculate_max_loan_credit(1), buf, 0);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Drawing credit", 1, buf, -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "", 1, "", -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Stadium", 1, "", -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "", 1, "", -1);

    gtk_list_store_append(liststore, &iter);
    print_grouped_int(stadiums[my_team].capacity, buf, 0);
    gtk_list_store_set(liststore, &iter,
		       0, "Capacity", 1, buf, -1);

    gtk_list_store_append(liststore, &iter);
    sprintf(buf, "%d", stadiums[my_team].average_attendance);
    gtk_list_store_set(liststore, &iter,
		       0, "Average attendance", 1, buf, -1);
    
    gtk_list_store_append(liststore, &iter);
    sprintf(buf, "%.0f%%", stadiums[my_team].safety * 100);
    gtk_list_store_set(liststore, &iter,
		       0, "Safety", 1, buf, -1);


    gtk_list_store_append(liststore, &iter);
    print_grouped_int(capacity_increase, buf, 0);
    gtk_list_store_set(liststore, &iter,
		       0, "500 seats cost", 1, buf, -1);

    gtk_list_store_append(liststore, &iter);
    print_grouped_int(safety_increase, buf, 0);
    gtk_list_store_set(liststore, &iter,
		       0, "5% safety increase costs", 1, buf, -1);

    return GTK_TREE_MODEL(liststore);
}

/* set up the treeview for displaying finances and stadium
   info */
void
set_up_fin_stad(GtkWidget *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;

    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      FALSE);
    
    /* columns: categorie, value */
    for(i=0;i<2;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
					   "text", i);
	if(strcmp(font_name, "0") != 0)
	    g_object_set(renderer, "font", font_name, NULL);
    }
}

/* show finances and stadium info */
void
show_fin_stad(void)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *fin_stad_model;
    
    clear_treeview(NULL);

    set_up_fin_stad(player_info);
    
    fin_stad_model = 
	create_fin_stad();
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(fin_stad_model));
    
    g_object_unref(fin_stad_model);
}

GtkTreeModel*
create_best_players(gint league,
		    gint best_players[][2])
{
    gint i, j, k;
    GtkListStore *liststore =
	gtk_list_store_new(5, G_TYPE_INT, G_TYPE_INT, 
			   G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
    GtkTreeIter iter;
    
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, -5,
		       1, league - 100,
		       2, -5,
		       3, -5,
		       4, -5,
		       -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, -5, 1, -5, 2, -5, 3, -5, 4, -5,
		       -1);

    for(j=0;j<2;j++)
    {    
	gtk_list_store_append(liststore, &iter);
	if(j == 0)
	    gtk_list_store_set(liststore, &iter,
			       0, -5, 1, -10,
			       2, -5, 3, -5, 4, -5,
			       -1);
	else
	    gtk_list_store_set(liststore, &iter,
			       0, -5, 1, -20,
			       2, -5, 3, -5, 4, -5,
			       -1);

	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, -5, 1, -5, 2, -5, 3, -5, 4, -5,
			   -1);

	for(i=0;i<10;i++)
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, i + 1, -1);

	    for(k=1;k<5;k++)
		gtk_list_store_set(liststore, &iter,
				   k, 100 * best_players[i + j * 10][0] +
				   best_players[i + j * 10][1],
				   -1);
	}

	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, -5, 1, -5, 2, -5, 3, -5, 4, -5,
			   -1);
    }

    return GTK_TREE_MODEL(liststore);
}

void
set_up_best_players(GtkWidget *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    gchar *titles[5] =
	{"",
	 "Name",
	 "Team",
	 "Goals",
	 "Goals / Game"};
    
    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      TRUE);

    /* columns: rank, name, team, goals, goals / game */
    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_cell_data_func(col, renderer,
					    cdf_int_to_cell,
					    GINT_TO_POINTER(0),
					    NULL);
    g_object_set(renderer, "xalign", 1.0,
		 NULL);
    gtk_tree_view_column_set_alignment(col, 1.0);
    
    for(i=1;i<5;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	gtk_tree_view_column_set_title(col, titles[i]);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_set_cell_data_func(col, renderer,
						cdf_best_players,
						GINT_TO_POINTER(i),
						NULL);
	if(i == 3)
	{
	    g_object_set(renderer, "xalign", 0.5,
			 NULL);
	    gtk_tree_view_column_set_alignment(col, 0.5);
	}
    }
}

/* show the 20 players given in the int arrays */
void
show_best_players(gint league, gint best_players[][2])
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *best_players_model;
    
    clear_treeview(NULL);

    set_up_best_players(player_info);
    
    best_players_model =
	create_best_players(league, best_players);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(best_players_model));
    
    g_object_unref(best_players_model);
}

void
create_extended_fixture(GtkListStore *liststore,
			gint fix_idx,
			gint show_ext_info,
			gint show_week_number)
{
    gint i, j, count[2] = {0, 0};
    gint inj_book_values[2][22];
    gint goal_idx =
	get_statistics_variable_index(fixtures[fix_idx]);
    GtkTreeIter iter;

    if(show_week_number == 1)
    {
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, fixtures[fix_idx].week_number + 1000000,
			   1, -5, 2, -5, -1);
    }
    
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, fixtures[fix_idx].type + 2000000,
		       1, -5,
		       2, fixtures[fix_idx].type + 2000000, -1);
    
    gtk_list_store_append(liststore, &iter);
    for(i=0;i<3;i++)
	gtk_list_store_set(liststore, &iter, i,
			   fix_idx, -1);
    
    if(show_ext_info < 0)
	return;

    for(i=0;i<50;i++)
	if(goals[goal_idx][i].minute != -1 &&
	   (goals[goal_idx][i].time == GOAL_TIME_REGULATION ||
	    goals[goal_idx][i].time == GOAL_TIME_EXTRA) )
	{
	    gtk_list_store_append(liststore, &iter);
	    if( (goals[goal_idx][i].team_id ==
		 fixtures[fix_idx].team_id[0] &&
		 goals[goal_idx][i].type != GOAL_TYPE_OWN) ||
		(goals[goal_idx][i].team_id ==
		 fixtures[fix_idx].team_id[1] &&
		 goals[goal_idx][i].type == GOAL_TYPE_OWN) )
		gtk_list_store_set(liststore, &iter,
				   0, 3000000 + goal_idx * 100 + i,
				   1, -5, 2, -5, -1);
	    else
		gtk_list_store_set(liststore, &iter,
				   0, -5, 1, -5,
				   2, 3000000 + goal_idx * 100 + i,
				   -1);
	}
    
    /* stadium attendance */
    gtk_list_store_append(liststore, &iter);
    if(fixtures[fix_idx].type % 1000 == 0 &&
       fixtures[fix_idx].type > 5000)
	gtk_list_store_set(liststore, &iter,
			   0, -410,
			   1, -5,
			   2, -5, -1);
    else
	gtk_list_store_set(liststore, &iter,
			   0, -400,
			   1, -5,
			   2, -5, -1);


    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, -500 - goal_idx,
		       1, -5,
		       2, -5, -1);    

    if(stadium_facts[goal_idx][1] != 0)
    {
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, -510 - stadium_facts[goal_idx][1],
			   1, -5,
			   2, -5, -1);
    }

    if((goal_idx == 0 && my_team < 114) ||
       sum(injuries[0], 0, 10) + sum(booked[0], 0, 10) +
       sum(injuries[1], 0, 10) + sum(booked[1], 0, 10) == -44)
	return;

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, -100,
		       1, -5,
		       2, -5, -1);
    
    for(i=0;i<2;i++)
	for(j=0;j<22;j++)
	    inj_book_values[i][j] = -5;

    for(i=0;i<2;i++)
	for(j=0;j<11;j++)
	{
	    if(injuries[i][j] != -1)
	    {
		inj_book_values[i][count[i]] = -800 - injuries[i][j];
		count[i]++;
	    }
    
	    if(booked[i][j] != -1)
	    {
		inj_book_values[i][count[i]] = -1000 - booked[i][j];
		count[i]++;
	    }
	}
    
    for(i=0;i<22;i++)
	if(inj_book_values[0][i] + inj_book_values[1][i] != -10)
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, inj_book_values[0][i],
			       1, -5,
			       2, inj_book_values[1][i], -1);
	}
}

GtkTreeModel*
create_preview(gint *fix_indices)
{
    gint i;
    GtkListStore *liststore =
	gtk_list_store_new(3, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT);
    GtkTreeIter iter;

    for(i=0;i<8;i++)
	if(fix_indices[i] != -10)
	{
	    create_extended_fixture(liststore, 
				    fix_indices[i],
				    1 - i, (i + 1) % 2);

	    if(i < 1 || fixtures[fix_indices[i]].type < 6000)
	    {
		gtk_list_store_append(liststore, &iter);
		gtk_list_store_set(liststore, &iter,
				   0, -5,
				   1, -5,
				   2, -5, -1);
	    }
	}

    create_fixture_liststore(liststore, week);

    return GTK_TREE_MODEL(liststore);
}

/* show the results of last week and the fixtures
   of next week */
void
show_preview(gint *fix_indices)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *preview_model;
    
    clear_treeview(NULL);

    set_up_fixtures(player_info);

    preview_model =
	create_preview(fix_indices);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(preview_model));
    
    g_object_unref(preview_model);
}


/* convert the rank of the human player into
   text */
void
stat_rank_to_text(gint rank, gchar *buf)
{
    gint local_rank = abs(rank);

    if(local_rank < 60)
    {
	sprintf(buf, "%d", local_rank);
	return;
    }

/*     get_league_name_from_id(get_place(local_rank, 11) * -1, buf);   */
    strcpy(buf, "");

    if(local_rank < 80)
    {
	if(local_rank == 65)
	    strcat(buf, "Round robin");
	else if(get_place(local_rank, 1) == 8)
	    strcat(buf, "Last 16");
	else if(get_place(local_rank, 1) == 4)
	    strcat(buf, "Quarter-Final");
	else if(get_place(local_rank, 1) == 2)
	    strcat(buf, "Semi-Final");
	else if(rank > 0)
	    strcat(buf, "Runner-up");
	else
	    strcat(buf, "Champion");

	return;
    }

    if(get_place(local_rank, 1) < 2)
	strcat(buf, "Last 32");
    else if(get_place(local_rank, 1) == 2)
	strcat(buf, "Last 16");
    else if(get_place(local_rank, 1) == 4)
	strcat(buf, "Quarter-Final");
    else if(get_place(local_rank, 1) == 6)
	strcat(buf, "Semi-Final");
    else if(rank > 0)
	strcat(buf, "Runner-up");
    else
	strcat(buf, "Champion");
}


GtkTreeModel*
create_history(season_stat *stat)
{
    GtkListStore *liststore =
	gtk_list_store_new(5, G_TYPE_STRING, G_TYPE_STRING,
			   G_TYPE_STRING, G_TYPE_STRING,
			   G_TYPE_STRING);
    GtkTreeIter iter;
    gint i;
    gchar buf[SMALL];
    gchar buf2[SMALL];
    gchar champions[STAT_END - 1][50];

    for(i=0;i<STAT_END - 1;i++)
	strcpy(champions[i], league_names[LEAGUE_NAMES_LEAGUE1 + i]);

    sprintf(buf, "%d", stat->season_number);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Season",
		       1, buf,
		       2, "", 3, "", 4, "",
		       -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Your team",
		       1, stat->team_names[STAT_MY_TEAM],
		       2, "", 3, "", 4, "",
		       -1);

    get_league_name_from_id(stat->my_league * -1, buf);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Your league",
		       1, buf,
		       2, "", 3, "", 4, "",
		       -1);

    stat_rank_to_text(stat->my_rank, buf);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Your rank",
		       1, buf,
		       2, "", 3, "", 4, "",
		       -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "", 1, "", 2, "", 3, "", 4, "",
		       -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "CHAMPIONS",
		       1, "", 2, "", 3, "", 4, "",
		       -1);
    
    for(i=0;i<STAT_END - 1;i++)
    {
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, champions[i],
			   1, stat->team_names[i + 1],
			   2, "", 3, "", 4, "",
			   -1);	
    }

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "", 1, "", 2, "", 3, "", 4, "",
		       -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "BEST PLAYERS",
		       1, "", 2, "", 3, "", 4, "",
		       -1);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "FIELD PLAYERS",
		       1, "", 2, "", 3, "", 4, "",
		       -1);

    for(i=0;i<6;i++)
    {
	if(i == 3)
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, "", 1, "", 2, "", 3, "", 4, "",
			       -1);
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, "GOALIES",
			       1, "", 2, "", 3, "", 4, "",
			       -1);
	}

	sprintf(buf, "%d", stat->best_players[i].goals);
	sprintf(buf2, "%.1f", 
		(gfloat)stat->best_players[i].goals /
		(gfloat)stat->best_players[i].games);
	gtk_list_store_append(liststore, &iter);
	gtk_list_store_set(liststore, &iter,
			   0, stat->best_players[i].name,
			   1, stat->best_players[i].team_name,
			   2, buf, 3, buf2, 4, "",
			   -1);
    }

    return GTK_TREE_MODEL(liststore);
}

void
set_up_history(GtkWidget *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    
    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      FALSE);

    for(i=0;i<5;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
					   "text", i);
    }
}

/* write the history stored in stat into a treeview */
void
show_history(season_stat *stat)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *history_model;
    
    clear_treeview(NULL);

    set_up_history(player_info);

    history_model =
	create_history(stat);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    GTK_TREE_MODEL(history_model));
    
    g_object_unref(history_model);

}

void
get_result_minute(gint idx, gint minute,
		       gint *result)
{
    gint i;
    gint goal_idx =
	get_statistics_variable_index(fixtures[idx]);

    result[0] = result[1] = 0;

    for(i=0;i<50;i++)
	if(goals[goal_idx][i].minute > 0 &&
	   goals[goal_idx][i].minute <= minute)
	{
	    if(goals[goal_idx][i].type <= GOAL_TYPE_PEN)
	    {
		if( (goals[goal_idx][i].team_id == 
		     fixtures[idx].team_id[0] &&
		     goals[goal_idx][i].type != GOAL_TYPE_OWN) ||
		    (goals[goal_idx][i].team_id == 
		     fixtures[idx].team_id[1] &&
		     goals[goal_idx][i].type == GOAL_TYPE_OWN) )
		    result[0]++;
		else
		    result[1]++;
	    }
	}
}

void
get_result_penalties(gint idx, gint max_goal,
			  gint *result)
{
    gint i;
    gint goal_idx =
	get_statistics_variable_index(fixtures[idx]);

    result[0] = result[1] = 0;

    for(i=0;i<max_goal;i++)
	if(goals[goal_idx][i].minute > 0 &&
	   goals[goal_idx][i].time == GOAL_TIME_PENALTY &&
	   goals[goal_idx][i].type == GOAL_TYPE_PEN)
	{
	    if(goals[goal_idx][i].team_id == 
	       fixtures[idx].team_id[0])
		result[0]++;
	    else
		result[1]++;
	}
}

gint
get_extra_time(gint idx)
{
    gint i, k;
    gint goal_idx = 
	get_statistics_variable_index(fixtures[idx]);

    for(i=49;i>=0;i--)
	if(goals[goal_idx][i].minute > 0 &&
	   goals[goal_idx][i].time == GOAL_TIME_REGULATION)
	    break;

    for(k=49;k>=0;k--)
	if(goals[goal_idx][k].minute > 0 &&
	   goals[goal_idx][k].time != GOAL_TIME_REGULATION)
	    break;


    if(k != -1)
    {
	if(i == -1 ||
	   goals[goal_idx][i].minute < 91)
	    return 91;

	return goals[goal_idx][i].minute + 1;
    }

    return 150;
}

GtkTreeModel*
create_live(gint idx, gint type)
{
    gint i;
    gint minute = (type < 1000) ? type : 120;
    gchar buf[SMALL], buf2[SMALL];
    gint result[2], pen_result[2];
    gint goal_idx =
	get_statistics_variable_index(fixtures[idx]);
    gint extra_time =
	get_extra_time(idx);
    GtkListStore *liststore =
	gtk_list_store_new(3, G_TYPE_STRING, 
			   G_TYPE_STRING, G_TYPE_STRING);
    GtkTreeIter iter;

    get_result_minute(idx, minute, result);
    if(type < 1000)
	sprintf(buf, "%d - %d", result[0], result[1]);
    else
    {
	if(type < 2000)
	    get_result_penalties(idx, type % 1000 - 1, pen_result);
	else
	    get_result_penalties(idx, type % 1000, pen_result);

	sprintf(buf, "%d - %d\n%d - %d", 
		result[0], result[1],
		pen_result[0], pen_result[1]);
    }
    
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, teams[fixtures[idx].team_id[0]].name,
		       1, buf,
		       2, teams[fixtures[idx].team_id[1]].name, -1);

    for(i=0;i<50;i++)
    {
	if( minute >= extra_time &&
	    (goals[goal_idx][i].minute < 0 ||
	     (goals[goal_idx][i].minute > 0 &&
	      goals[goal_idx][i].time != GOAL_TIME_REGULATION)) )
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, "", 1, "", 2, "", -1);
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, "Extra time",
			       1, "", 2, "", -1);
	    extra_time = 150;
	}
	
	if(goals[goal_idx][i].minute != -1 &&
	   goals[goal_idx][i].minute <= minute)
	{	       
	    sprintf(buf, "%s %d.",
		    teams[goals[goal_idx][i].team_id].
		    players[goals[goal_idx][i].scorer].
		    name,
		    goals[goal_idx][i].minute);
	    if(goals[goal_idx][i].type == GOAL_TYPE_OWN)
		strcat(buf, " (own goal)");
	    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN)
		strcat(buf, " (penalty)");
	    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_MISSED)
		strcat(buf, " (penalty: MISSED !!)");
	    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_SAVE)
		strcat(buf, " (penalty: SAVE !!)");
	    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_POST)
		strcat(buf, " (penalty: POST !!)");
	    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_CROSS)
		strcat(buf, " (penalty: CROSS-BAR !!)");

	    gtk_list_store_append(liststore, &iter);
	    if( (goals[goal_idx][i].team_id ==
		 fixtures[idx].team_id[0] &&
		 goals[goal_idx][i].type != GOAL_TYPE_OWN) ||
		(goals[goal_idx][i].team_id ==
		 fixtures[idx].team_id[1] &&
		 goals[goal_idx][i].type == GOAL_TYPE_OWN) )
		gtk_list_store_set(liststore, &iter,
				   0, buf,
				   1, "", 2, "", -1);
	    else
		gtk_list_store_set(liststore, &iter,
				   0, "", 1, "",
				   2, buf,
				   -1);
	}
    }

    if(type < 1000)
	return GTK_TREE_MODEL(liststore);

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "", 1, "", 2, "", -1);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, "Penalty shoot-out",
		       1, "", 2, "", -1);

    for(i = 0; i < type % 1000; i++)
    {
	if(goals[goal_idx][i].time == GOAL_TIME_PENALTY)
	{
	    gtk_list_store_append(liststore, &iter);
	    
	    sprintf(buf, "%s shoots . . .",
		    teams[goals[goal_idx][i].team_id].
		    players[goals[goal_idx][i].scorer].
		    name);

	    if(type >= 2000 ||
	       i < type % 1000 - 1)
	    {		
		get_result_penalties(idx, i + 1, pen_result);
		sprintf(buf2, "%d - %d", 
			pen_result[0], pen_result[1]);
		if(goals[goal_idx][i].type == GOAL_TYPE_PEN)
		    strcat(buf, " Goal!");
		else
		{
		    if(goals[goal_idx][i].type == GOAL_TYPE_PEN_SAVE)
			strcat(buf, " Save!");
		    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_MISSED)
			strcat(buf, " Missed!");
		    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_POST)
			strcat(buf, " Post!");
		    else if(goals[goal_idx][i].type == GOAL_TYPE_PEN_CROSS)
			strcat(buf, " Cross-bar!");
		}
	    }

	    if(goals[goal_idx][i].team_id ==
	       fixtures[idx].team_id[0])
		gtk_list_store_set(liststore, &iter,
				   0, buf,
				   1, buf2,
				   2, "", -1);
	    else
		gtk_list_store_set(liststore, &iter,
				   0, "",
				   1, buf2,
				   2, buf,
				   -1);
	}	
    }    

    return GTK_TREE_MODEL(liststore);
}

void
set_up_live(GtkWidget *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    
    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      FALSE);

    for(i=0;i<3;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
					   "text", i);

	if(i == 1)
	    g_object_set(renderer, "xalign", 0.5,
			 NULL);

	if(strcmp(font_name, "0") != 0)
	    g_object_set(renderer, "font", font_name, NULL);
    }
}

void
show_live_window(GtkWidget *live_window, gint idx, gint type)
{
    GtkWidget *scrolledwindow =
	lookup_widget(live_window, "scrolledwindow_live");
    GtkWidget *treeview_live =
	lookup_widget(live_window, "treeview_live");
    GtkTreeModel *live_model;
    GtkAdjustment *adjustment;
    
    clear_treeview(treeview_live);

    set_up_live(treeview_live);

    live_model =
	create_live(idx, type);
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_live),
			    GTK_TREE_MODEL(live_model));
    
    g_object_unref(live_model);

    adjustment = gtk_scrolled_window_get_vadjustment(
	GTK_SCROLLED_WINDOW(scrolledwindow));
    adjustment->value = adjustment->upper;
    gtk_adjustment_value_changed(adjustment);
}

/* get the ids of the opponents of the user's team
   this week; store the type of game (home/away/neutral) */
void
get_opponent_ids(gint *opponent_ids)
{
    gint i;
    gint idx = 0;

    for(i = FIX_END - 1; i >= 0; i--)
	if(fixtures[i].type != -1 &&
	   my_team_involved(fixtures[i]) && 
	   fixtures[i].week_number == week)
	{
	    if(idx == 2)
	    {
		g_print("get_opponents_id: Warning: found too many fixtures for team %d week %d fix.type %d",
			my_team, week, fixtures[i].type);
		return;
	    }

	    opponent_ids[idx] = fixtures[i].team_id[ (fixtures[i].team_id[0] == my_team) ];
	    opponent_ids[idx + 4] = i;
	    /* neutral stadium */
	    if(fixtures[i].type % 1000 == 0 &&
	       fixtures[i].type > 5000)
		opponent_ids[idx + 2] = 2;
	    /* home game */
	    else if(fixtures[i].team_id[0] == my_team)
		opponent_ids[idx + 2] = 0;
	    /* away game */
	    else
		opponent_ids[idx + 2] = 1;
	    idx++;
	}
}

GtkTreeModel*
create_opponents(void)
{
    GtkListStore *liststore =
	gtk_list_store_new(3, G_TYPE_INT, G_TYPE_STRING,
			   G_TYPE_STRING);
    gint opponent_ids[6];
    gint i;
    gchar buf[SMALL];
    GtkTreeIter iter;

    for(i=0;i<6;i++)
	opponent_ids[i] = -1;

    get_opponent_ids(opponent_ids);    

    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, opponent_ids[0], 1, "Your next opponent(s)", 2, "", -1);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter,
		       0, opponent_ids[0], 1, "", 2, "", -1);

    for(i=0;i<2;i++)
    {
	strcpy(buf, "");
	if(opponent_ids[i + 2] == 0)
	    strcpy(buf, "Home");
	else if(opponent_ids[i + 2] == 2)
	    strcpy(buf, "Neutral stadium");
	else if(opponent_ids[i + 2] != -1)
	    strcpy(buf, "Away");
	if(opponent_ids[i] != -1)
	{
	    gtk_list_store_append(liststore, &iter);
	    gtk_list_store_set(liststore, &iter,
			       0, opponent_ids[i], 1, "", 2, buf, -1);
	}

	create_opponent_info(liststore, opponent_ids, i);
    }

    return GTK_TREE_MODEL(liststore);
}

void
set_up_opponents(GtkTreeView *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    
    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(treeview),
	GTK_SELECTION_SINGLE);
    
    gtk_tree_view_set_headers_visible(treeview,
				      FALSE);

    col = gtk_tree_view_column_new();
    gtk_tree_view_append_column(treeview, col);
    renderer = gtk_cell_renderer_text_new();
    gtk_tree_view_column_pack_start(col, renderer, TRUE);
    gtk_tree_view_column_set_cell_data_func(col, renderer,
					    cdf_opponents,
					    GINT_TO_POINTER(0),
					    NULL);
    for(i=0;i<2;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(treeview, col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
					   "text", i + 1);

	if(strcmp(font_name, "0") != 0)
	    g_object_set(renderer, "font", font_name, NULL);
    }
}

/* show info about the team(s) the user will play against
   the current week */
void
show_next_opponents(void)
{
    GtkWidget *player_info =
	lookup_widget(main_window, "player_info");
    GtkTreeModel *opponents_model;
    
    clear_treeview(NULL);

    set_up_opponents(GTK_TREE_VIEW(player_info));

    opponents_model =
	create_opponents();
    
    gtk_tree_view_set_model(GTK_TREE_VIEW(player_info),
			    opponents_model);
    
    g_object_unref(opponents_model);
}

void
create_awards_player(GtkListStore *liststore, gint prized_players[][3])
{
  gint i;
  gchar buf[SMALL];
  gchar buf2[SMALL];
  gchar buf3[SMALL];
  GtkTreeIter iter;
  gchar *titles[4] =
    {"BEST PLAYER",
     "BEST DEFENDER",
     "BEST MIDFIELDER",
     "BEST FORWARD"};

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "PLAYER AWARDS", 2, "", 3, "", -1);

  for(i=PRIZE_PLAYER_GOALGET;i<PRIZE_PLAYER_GOALIE + 3;i++)
    {
      if(i == PRIZE_PLAYER_GOALGET)
	{
	  gtk_list_store_append(liststore, &iter);
	  gtk_list_store_set(liststore, &iter,
			     0, "", 1, "BEST GOAL SCORERS", 2, "", 3, "", -1);
	}
      else if(i == PRIZE_PLAYER_GOALIE)
	{
	  gtk_list_store_append(liststore, &iter);
	  gtk_list_store_set(liststore, &iter,
			     0, "", 1, "BEST GOALIES", 2, "", 3, "", -1);
	}
      
      sprintf(buf, "%d", (PRIZE_PLAYER_GOALGET * 2 + i) % 3 + 1);
      print_grouped_int(prized_players[i][2], buf2, 0);
      sprintf(buf3, "%s (%d / %d )", 
	      teams[prized_players[i][0]].players[prized_players[i][1]].name,
	      teams[prized_players[i][0]].players[prized_players[i][1]].goals,
	      teams[prized_players[i][0]].players[prized_players[i][1]].games);
      gtk_list_store_append(liststore, &iter);
      gtk_list_store_set(liststore, &iter,
			 0, buf, 1, teams[prized_players[i][0]].name,
			 2, buf3,
			 3, buf2, -1);
    }
  
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "", 2, "", 3, "", -1);
  
  for(i=PRIZE_PLAYER_BEST_PLAYER;i<PRIZE_PLAYER_END;i++)
    {
      gtk_list_store_append(liststore, &iter);
      gtk_list_store_set(liststore, &iter,
			 0, "", 1, titles[i - PRIZE_PLAYER_BEST_PLAYER],
			 2, "", 3, "", -1);
      
      print_grouped_int(prized_players[i][2], buf2, 0);
      gtk_list_store_append(liststore, &iter);
      gtk_list_store_set(liststore, &iter,
			 0, "", 1, teams[prized_players[i][0]].name,
			 2, teams[prized_players[i][0]].players[prized_players[i][1]].name,
			 3, buf2, -1);
    }
}

void
create_awards_team(GtkListStore *liststore, gint prized_teams[][2])
{
  gint i;
  gchar buf[SMALL];
  gchar buf2[SMALL];
  GtkTreeIter iter;

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "TEAM AWARDS", 2, "", 3, "", -1);
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "BEST TEAMS", 2, "", 3, "", -1);

  for(i=0;i<4;i++)
    {
      if(i < 3 || get_league_from_id(my_team) == 1)
	{
	  sprintf(buf, "%d", i + 1);
	  print_grouped_int(prized_teams[i][1], buf2, 0);
	  gtk_list_store_append(liststore, &iter);
	  gtk_list_store_set(liststore, &iter,
			     0, buf, 1, teams[prized_teams[i][0]].name,
			     2, "", 3, buf2, -1);
	}
      else
	{
	  print_grouped_int(prized_teams[PRIZE_TEAM_PROM][1], buf2, 0);
	  gtk_list_store_append(liststore, &iter);
	  gtk_list_store_set(liststore, &iter,
			     0, "", 1, teams[prized_teams[PRIZE_TEAM_PROM][0]].name,
			     2, "(Prom. games winner)", 3, buf2, -1);
	}
    }

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "", 2, "", 3, "", -1);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "BEST OFFENSIVE TEAM", 2, "", 3, "", -1);
  
  sprintf(buf, "(%d:%d Goals)", 
	  teams[prized_teams[PRIZE_TEAM_OFFENSIVE][0]].results[RES_GF],
	  teams[prized_teams[PRIZE_TEAM_OFFENSIVE][0]].results[RES_GA]);
  print_grouped_int(prized_teams[PRIZE_TEAM_OFFENSIVE][1], buf2, 0);
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, teams[prized_teams[PRIZE_TEAM_OFFENSIVE][0]].name,
		     2, buf, 3, buf2, -1);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "BEST DEFENSIVE TEAM", 2, "", 3, "", -1);
  
  sprintf(buf, "(%d:%d Goals)", 
	  teams[prized_teams[PRIZE_TEAM_DEFENSIVE][0]].results[RES_GF],
	  teams[prized_teams[PRIZE_TEAM_DEFENSIVE][0]].results[RES_GA]);
  print_grouped_int(prized_teams[PRIZE_TEAM_DEFENSIVE][1], buf2, 0);
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, teams[prized_teams[PRIZE_TEAM_DEFENSIVE][0]].name,
		     2, buf, 3, buf2, -1);

  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "", 2, "", 3, "", -1);
}

GtkTreeModel*
create_awards(gint prized_teams[][2], gint prized_players[][3])
{
  GtkListStore *liststore =
    gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING,
		       G_TYPE_STRING, G_TYPE_STRING);
  GtkTreeIter iter;
  gchar buf[SMALL];

  sprintf(buf, "SEASON AWARDS");
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, buf, 2, "", 3, "", -1);
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "", 2, "", 3, "", -1);

  create_awards_team(liststore, prized_teams);

  create_awards_player(liststore, prized_players);

  if(finances[FIN_PRIZE] == 0)
    return GTK_TREE_MODEL(liststore);
    
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "", 2, "", 3, "", -1);

  print_grouped_int(finances[FIN_PRIZE], buf, 0);
  gtk_list_store_append(liststore, &iter);
  gtk_list_store_set(liststore, &iter,
		     0, "", 1, "Your team's prize money", 2, "", 3, buf, -1);

  return GTK_TREE_MODEL(liststore);
}

void
set_up_awards(GtkTreeView *treeview)
{
    gint i;
    GtkTreeViewColumn *col;
    GtkCellRenderer *renderer;
    gchar *titles[4] =
	{"",
	 "Team",
	 "Player",
	 "Prize money"};
    
    gtk_tree_selection_set_mode(
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview)),
	GTK_SELECTION_NONE);
    
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(treeview),
				      TRUE);

    for(i=0;i<4;i++)
    {
	col = gtk_tree_view_column_new();
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
	gtk_tree_view_column_set_title(col, titles[i]);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer,
					   "text", i);

	if(strcmp(font_name, "0") != 0)
	    g_object_set(renderer, "font", font_name, NULL);
    }    
}

/* show the awards featuring the teams, players and prize
   money given in the arguments in a treeview */
void
show_season_awards(gint prized_teams[][2], gint prized_players[][3])
{
  GtkWidget *player_list =
    lookup_widget(main_window, "player_list");
  GtkTreeModel *awards_model;

  clear_treeview(player_list);

  set_up_awards(GTK_TREE_VIEW(player_list));

  awards_model = create_awards(prized_teams, prized_players);

  gtk_tree_view_set_model(GTK_TREE_VIEW(player_list),
			  awards_model);
  
  g_object_unref(awards_model);
}
