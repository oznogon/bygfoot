/*************************************************************************
 * Support functions for the callback functions in 'editor_callbacks.c'  *
 *************************************************************************/

#include "editor_defs.h"
#include "callback_func.h"
#include "editor_callback_func.h"
#include "gui.h"
#include "maths.h"
#include "misc.h"
#include "player.h"
#include "startup.h"
#include "support.h"
#include "team.h"
#include "files.h"
#include "treeview.h"
#include "window.h"

void
set_spinbutton_values(void)
{
    GtkWidget *entry_name =
	lookup_widget(main_window, "entry_name");
    GtkWidget *spinbutton_skill =
	lookup_widget(main_window, "spinbutton_skill");
    GtkWidget *spinbutton_talent =
	lookup_widget(main_window, "spinbutton_talent");
    GtkWidget *spinbutton_birth_year =
	lookup_widget(main_window, "spinbutton_birth_year");
    GtkWidget *spinbutton_birth_month =
	lookup_widget(main_window, "spinbutton_birth_month");
    GtkWidget *spinbutton_swap1 =
	lookup_widget(main_window, "spinbutton_swap1");
    GtkWidget *spinbutton_move =
	lookup_widget(main_window, "spinbutton_move");
    GtkWidget *label_number =
	lookup_widget(main_window, "label_number");

    if(selected_player == -1)
	return;

    gtk_entry_set_text(GTK_ENTRY(entry_name),
		       teams[114].players[selected_player].name);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_skill),
			      teams[114].players[selected_player].skill);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_talent),
			      teams[114].players[selected_player].talent);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_birth_year),
			      (gfloat)get_place(birth_dates[selected_player], 14));
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_birth_month),
			      (gfloat)get_place(birth_dates[selected_player], 22));

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_move),
			      (gfloat)selected_player + 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_swap1),
			      (gfloat)selected_player + 1);

    label_set_text_from_int(GTK_LABEL(label_number), selected_player + 1, 0);
}

void
write_average_skill(void)
{
    GtkWidget *spinbutton_average_skill =
	lookup_widget(main_window, "spinbutton_average_skill");

    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_average_skill),
			      average_skill(114, 20, FALSE));
}

void
write_team_to_screen(void)
{
    gint i;
    GtkWidget *entry_structure1 =
	lookup_widget(main_window, "entry_structure1");
    GtkWidget *entry_structure2 =
	lookup_widget(main_window, "entry_structure2");
    GtkWidget *treeview_players =
	lookup_widget(main_window, "treeview_players");
    GtkAdjustment *adjustment;
    GtkTreeSelection *selection =
	gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview_players));
    GtkTreePath *path;
    gchar buf[SMALL];
    gint attrib[ATT_END];
    gint ids[20][2];
    gfloat adjust_factor = 0.5;

    for(i=0;i<ATT_DUMMY1;i++)
	attrib[i] = 0;

    attrib[ATT_NAME] = attrib[ATT_POS] =
	attrib[ATT_SKILL] = attrib[ATT_TALENT] =
	attrib[ATT_AGE] = 1;

    for(i=0;i<20;i++)
    {
	teams[114].players[i].pos =
	    teams[114].players[i].cpos =
	    get_position_from_structure(114, (i > 10) * structure2,
					i, 1);	
	ids[i][0] = 114;
	ids[i][1] = i;
    }

    show_players(treeview_players, ids, 20, attrib, 0);

    if(selected_player < 6)
	adjust_factor = 0;
    else if(selected_player > 13)
	adjust_factor = 1;

    adjustment = gtk_scrolled_window_get_vadjustment(
	GTK_SCROLLED_WINDOW(treeview_players->parent));
    adjustment->value = ((adjustment->upper - adjustment->page_size) * adjust_factor +
			 adjustment->lower * (1 - adjust_factor));
    
    gtk_adjustment_value_changed(adjustment);

    entry_set_text_from_int(GTK_ENTRY(entry_structure1), teams[114].structure);
    entry_set_text_from_int(GTK_ENTRY(entry_structure2), structure2);
    write_average_skill();
    
    if(selected_player < 11)
	sprintf(buf, "%d", selected_player);
    else
	sprintf(buf, "%d", selected_player + 1);
    path = gtk_tree_path_new_from_string(buf);
    gtk_tree_selection_select_path(selection, path);
}

void
callback_change_structures(void)
{
    gchar buf[SMALL];
    GtkWidget *entry_structure1 =
	lookup_widget(main_window, "entry_structure1");
    GtkWidget *entry_structure2 =
	lookup_widget(main_window, "entry_structure2");
    gint local_structure1 = entry_get_int(GTK_ENTRY(entry_structure1));
    gint local_structure2 = entry_get_int(GTK_ENTRY(entry_structure2));

    if(get_place(local_structure1, 1) + get_place(local_structure1, 2) + 
       get_place(local_structure1, 3) != 10 ||
       get_place(local_structure2, 1) + get_place(local_structure2, 2) + 
       get_place(local_structure2, 3) != 8)
    {
	sprintf(buf, "One of the structure numbers is not a valid structure specifier. The digits of the first structure must add up to 10, those of the second one to 8. ");
	show_popup_window(buf, NULL);
	return;
    }

    structure2 = local_structure2;
    teams[114].structure = local_structure1;

    write_team_to_screen();
}

void
set_teams_file_pulldown(void)
{
    GtkWidget *combo_team_file =
	lookup_widget(main_window, "combo_team_file");
    GList *league_files = NULL;
    gchar filenames[FILES_PLAYER_NAMES][SMALL];
    gint i;
    
    for(i=0;i<FILES_PLAYER_NAMES;i++)
	text_file_number_to_char(i, filenames[i], TRUE);

    for(i=0;i<FILES_PLAYER_NAMES;i++)
	if(strcmp(filenames[i], "") != 0)
	    league_files = g_list_append(league_files, filenames[i]);

    gtk_combo_set_popdown_strings(GTK_COMBO(combo_team_file),
				  league_files);

    g_list_free(league_files);
}

void
callback_load_team_file(void)
{
    GtkWidget *entry_country_file =
	lookup_widget(main_window, "entry_country_file");
    GtkWidget *combo_team_names =
	lookup_widget(main_window, "combo_team_names");
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry_country_file));
    gchar *filename;
    FILE *fil;
    gchar buf[SMALL];
    gchar team_names[500][50];
    gint i;
    GList *country_list = NULL;

    filename = g_malloc(strlen(entry_text) + 1);
    strcpy(filename, entry_text);
    fil = fopen(filename, "r");	
    
    if(fil == NULL)
    {
	sprintf(buf, "Could not open file: %s .", filename);
	show_popup_window(buf, NULL);
	return;
    }

    fclose(fil);

    for(i=0;i<500;i++)
	strcpy(team_names[i], "1");
    
    get_names(filename, team_names);

    for(i=0;i<500;i++)
	if(strtol(team_names[i], NULL, 10) == 0 ||
	   strlen(team_names[i]) > 2)
	    country_list =
		g_list_append (country_list, team_names[i]);

    gtk_combo_set_popdown_strings (GTK_COMBO (combo_team_names),
				   country_list);

    g_list_free(country_list);
    g_free(filename);
}

void
callback_load_team(void)
{
    GtkWidget *entry_team_name =
	lookup_widget(main_window, "entry_team_name");
    GtkWidget *entry_team = 
	lookup_widget(main_window, "entry_team");
    const gchar *team_name =
	gtk_entry_get_text(GTK_ENTRY(entry_team));
    GtkWidget *entry_definitions_file =
	lookup_widget(main_window, "entry_definitions_file");
    const gchar *def_file = gtk_entry_get_text(GTK_ENTRY(entry_definitions_file));
    FILE *fil = fopen(def_file, "r");
    gchar buf[SMALL];
    gint i;

    if(strcmp(team_name, "") == 0 || strcmp(def_file, "") == 0)
    {
	sprintf(buf, "You haven't selected a team you want to edit or a definitions file. ");
	show_popup_window(buf, NULL);
	return;	
    }

    if(fil == NULL)
    {
	sprintf(buf, "The file %s could not be opened.",
		def_file);
	show_popup_window(buf, NULL);
	return;
    }

    fclose(fil);

    strcpy(teams[114].name, team_name);
    gtk_entry_set_text(GTK_ENTRY(entry_team_name), team_name);

    teams[114].structure = 442;
    structure2 = 332;
    for(i=0;i<20;i++)
    {
	birth_dates[i] = 198006;
	strcpy(teams[114].players[i].name, "# NOBODY");
	teams[114].players[i].skill = 
	    teams[114].players[i].talent = 5;
	teams[114].players[i].age = get_age_from_birth(birth_dates[i]);
	teams[114].players[i].pos =
	    teams[114].players[i].cpos =
	    get_position_from_structure(114, (i > 10) * structure2,
					i, 1);
    }

    read_teams_file(2, team_name, &structure2, birth_dates);

    write_team_to_screen();
}

/* save the name of the edited team to
   the country file */
void
save_team_name(void)
{
    GtkWidget *entry_country_file =
	lookup_widget(main_window, "entry_country_file");
    GtkWidget *entry_team_name =
	lookup_widget(main_window, "entry_team_name");
    GtkWidget *entry_team = 
	lookup_widget(main_window, "entry_team");
    const gchar *team_name =
	gtk_entry_get_text(GTK_ENTRY(entry_team));
    const gchar *new_name =
	gtk_entry_get_text(GTK_ENTRY(entry_team_name));
    const gchar *country_file =
	gtk_entry_get_text(GTK_ENTRY(entry_country_file));
    FILE *fil;
    GString *file_content;
    gchar buf[BIG];

    if(strcmp(team_name, new_name) == 0)
	return;

    if(strlen(new_name) == 0)
    {
	show_popup_window("You haven't entered a new team name. ", NULL);
	return;
    }
    
    fil = fopen(country_file, "r");
    if(fil == NULL)
    {
	sprintf(buf,
		"Could not open file: %s. The new team name could not be saved.",
		country_file);
	show_popup_window(buf, NULL);
	return;
    }
    
    file_content = g_string_new("");
    strcpy(buf, "");

    while(strcmp(buf, team_name) != 0 &&
	  strcmp(buf, "1000") != 0 &&
	  feof(fil) == 0)
    {
	fscanf(fil, "%[ \n\t]*", buf);
	g_string_append(file_content, buf);

	fscanf(fil, "%[^\n]", buf);
	if(strcmp(buf, team_name) != 0)
	    g_string_append(file_content, buf);
    }

    if(strcmp(buf, "1000") == 0 ||
       feof(fil) != 0)
    {
	show_popup_window("Could not save new team name to %s. I couldn't find the old name.",
			  NULL);
	g_string_free(file_content, TRUE);
	return;
    }

    g_string_append(file_content, new_name);
    
    while(feof(fil) == 0)
    {
	fscanf(fil, "%[ \n\t]*", buf);
	g_string_append(file_content, buf);
	fscanf(fil, "%[^\n]", buf);
	g_string_append(file_content, buf);
    }

    fclose(fil);
    fopen(country_file, "w");
    fprintf(fil, "%s", file_content->str);
    fclose(fil);
    
    g_string_free(file_content, TRUE);
}


/* save currently edited team to the
   defs file or alternatively to the argument file */
void
save_defs(const gchar *def_file)
{
    gint i, skill, talent;
    gchar buf[SMALL];
    gchar player_name[19];
    GtkWidget *entry_team_name =
	lookup_widget(main_window, "entry_team_name");
    const gchar *new_name =
	gtk_entry_get_text(GTK_ENTRY(entry_team_name));
    FILE *fil;
    GString *new_def_file = g_string_new("");
    gboolean talent_error = FALSE;

    strcpy(buf, "");
    fil = fopen(def_file, "r");    

    /* copy everything that's in the old file
       up until the currently edited team or
       the end of the definitions is reached */
    while(feof(fil) == 0 &&
	  strcmp(buf, teams[114].name) != 0)
    {
	fscanf(fil, "%[\n \t]*", buf);
	g_string_append(new_def_file, buf);
	fscanf(fil, "%[^\n]", buf);
	if(feof(fil) == 0 &&
	   strcmp(buf, teams[114].name) != 0)
	    g_string_append(new_def_file, buf);
    }

    strcpy(teams[114].name, new_name);

    /* write new team properties to the gstring */
    if(feof(fil) != 0)
	g_string_append_printf(new_def_file, "\n%s\n", teams[114].name);
    else
	g_string_append_printf(new_def_file, "%s\n", teams[114].name);
    g_string_append_printf(new_def_file, "%d %d\n",
			   teams[114].structure, structure2);	
    for(i=0;i<20;i++)
    {
	strcpy(player_name, teams[114].players[i].name);
	skill = (gint)rint(teams[114].players[i].skill * 10);
	talent = (gint)rint(teams[114].players[i].talent * 10);
	    
	if(player_name[0] != '#')
	{
	    if(talent >= skill)
		g_string_append_printf(new_def_file, "%d %s & %d & %d & %d\n",
				       i + 1, player_name, skill, talent, birth_dates[i]);
	    else
		talent_error = TRUE;
	}
    }	
    g_string_append(new_def_file, "end_players");

    /* copy the rest of the old file */
    if(feof(fil) == 0)
    {
	while(strcmp(buf, "end_players") != 0)
	{
	    fscanf(fil, "%[\n \t]*", buf);
	    fscanf(fil, "%[^\n]", buf);
	}

	fscanf(fil, "%[\n \t]*", buf);
	while(feof(fil) == 0)
	{
	    fscanf(fil, "%[\n \t]*", buf);
	    g_string_append(new_def_file, buf);
	    fscanf(fil, "%[^\n]*", buf);
	    g_string_append(new_def_file, buf);
	}	
    }
    else	
	g_string_append(new_def_file, "\n");


    fclose(fil);

    /* overwrite old file */
    fil = fopen(def_file, "w");
    fprintf(fil, "%s", new_def_file->str);
    fclose(fil);

    if(talent_error)
	show_popup_window("One or more players could not be saved because of an invalid talent value. The talent value of a players has to be greater or equal his skill value. ", NULL);
}

gboolean
callback_save_defs(void)
{
    gchar buf[SMALL];
    GtkWidget *entry_team = 
	lookup_widget(main_window, "entry_team");
    const gchar *team_name =
	gtk_entry_get_text(GTK_ENTRY(entry_team));
    GtkWidget *entry_definitions_file =
	lookup_widget(main_window, "entry_definitions_file");
    const gchar *def_file = gtk_entry_get_text(GTK_ENTRY(entry_definitions_file));
    FILE *fil;

    if(strcmp(team_name, "") == 0)
    {
	show_popup_window( "You haven't selected a team. ", NULL);
	return FALSE;	
    }

    fil = fopen(def_file, "a+");
    if(fil == NULL)
    {
	sprintf(buf, "The file %s could not be opened in read/write mode.",
		def_file);
	show_popup_window(buf, NULL);
	return FALSE;
    }

    fclose(fil);
    save_defs(def_file);
    save_team_name();
    return TRUE;
}

void
callback_swap_players(void)
{
    GtkWidget *spinbutton_swap1 =
	lookup_widget(main_window, "spinbutton_swap1");
    GtkWidget *spinbutton_swap2 =
	lookup_widget(main_window, "spinbutton_swap2");
    gint swap1 = 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_swap1));
    gint swap2 = 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_swap2));

    swap_players(114, swap1 - 1, 114, swap2 - 1);

    selected_player = swap2 - 1;

    swap_int(&birth_dates[swap1 - 1], &birth_dates[swap2 - 1]);

    set_spinbutton_values();

    write_team_to_screen();
}

void
callback_play(gint save)
{
    if(save == 1)
	if(!callback_save_defs())
	    return;

    gtk_widget_destroy(main_window);

    main_window = 
	return_main_window();

    start(0);
}

void
callback_edit_player(GtkTreeSelection *selection,
			  GdkEventButton *event)
{   
    gint selected = 
	get_row_index(selection, event, 0) - 1;

    if(selected < 0)
	return;

    selected_player = selected;
    
    set_spinbutton_values();

    write_team_to_screen();
}

void
callback_update(void)
{
    GtkWidget *entry_name =
	lookup_widget(main_window, "entry_name");
    const gchar *new_name = 
	gtk_entry_get_text(GTK_ENTRY(entry_name));
    GtkWidget *spinbutton_skill =
	lookup_widget(main_window, "spinbutton_skill");
    gfloat new_skill = 
	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_skill));
    GtkWidget *spinbutton_talent =
	lookup_widget(main_window, "spinbutton_talent");
    gfloat new_talent =
	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_talent));
    GtkWidget *spinbutton_birth_year =
	lookup_widget(main_window, "spinbutton_birth_year");
    GtkWidget *spinbutton_birth_month =
	lookup_widget(main_window, "spinbutton_birth_month");
    gint birth_date =
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_birth_year)) * 100 +
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_birth_month));
    gfloat new_age = get_age_from_birth(birth_date);

    if(selected_player == -1)
	return;    
    
    if(new_skill > new_talent)
    {
	show_popup_window("You have entered an invalid talent value. The talent value of a player has to be greater than his skill value. ", NULL);
	return;
    }

    if(new_name[0] == '#')
	show_popup_window("You have entered a player name beginning with the comment character '#'. The player will be ignored when loading the team from the definitions file. ", NULL);
    
    if(new_age < 17)
    {
	show_popup_window("You have entered an invalid birth date. Players must be older than 17. ", NULL);
	return;
    }

    sprintf(teams[114].players[selected_player].name, "%s", new_name);
    teams[114].players[selected_player].skill = new_skill;
    teams[114].players[selected_player].talent = new_talent;
    teams[114].players[selected_player].age = new_age;

    birth_dates[selected_player] = birth_date;

    write_team_to_screen();
}

void
callback_move(gint direction)
{
    gint new_number = selected_player + direction;

    if(selected_player == -1)
	return;

    if(new_number == 20)
	new_number = 0;
    else if(new_number == -1)
	new_number = 19;

    swap_players(114, selected_player, 114, new_number);

    swap_int(&birth_dates[selected_player], &birth_dates[new_number]);

    selected_player = new_number;

    set_spinbutton_values();

    write_team_to_screen();
}

/* select next or previous player in the list */
void
callback_change_player(gint direction)
{
    selected_player += direction;

    if(selected_player == 20)
	selected_player = 0;
    else if(selected_player == -1)
	selected_player = 19;

    set_spinbutton_values();

    write_team_to_screen();
}

void
callback_change_average_skill(void)
{
    gint i;
    gfloat current_average = average_skill(114, 20, FALSE);
    GtkWidget *spinbutton_average_skill = 
	lookup_widget(main_window, "spinbutton_average_skill");
    gfloat changed_average = 
	gtk_spin_button_get_value(GTK_SPIN_BUTTON(spinbutton_average_skill));
    gfloat diff = changed_average - current_average;

    if(current_average == changed_average)
	return;

    for(i=0;i<20;i++)
	if(teams[114].players[i].name[0] != '#' &&
	   teams[114].players[i].skill <= 9.9 - diff)
	    teams[114].players[i].skill += diff;

    write_team_to_screen();
}

/* extend the team definitions file by the contents
   of another team file */
void
import_file(const gchar *file_name)
{
    GtkWidget *entry_definitions_file =
	lookup_widget(main_window, "entry_definitions_file");
    const gchar *def_file = gtk_entry_get_text(GTK_ENTRY(entry_definitions_file));
    gchar buf[BIG];
    GString *imported_teams = g_string_new("");
    FILE *definitions_file;
    FILE *import_file;
    gboolean no_imports = TRUE;

    if(strcmp(def_file, file_name) == 0)
    {
	show_popup_window("It doesn't make sense to import a file into itself. ", NULL);
	return;
    }

    definitions_file = fopen(def_file, "a");
    import_file = fopen(file_name, "r");

    if(definitions_file == NULL || import_file == NULL)
    {
	show_popup_window("One of the files could not be opened. ", NULL);
	return;
    }

    g_string_append_printf(imported_teams, 
			   "The following teams were added to %s:\n\n", def_file);

    while(feof(import_file) == 0)
    {
	strcpy(buf, "#");
	/* read comments and blank lines */
	while(buf[0] == '#' && feof(import_file) == 0)
	{
	    fscanf(import_file, "%[\n \t]*", buf);
	    fprintf(definitions_file, "%s", buf);
	    fscanf(import_file, "%[^\n]*", buf);
	    fprintf(definitions_file, "%s", buf);
	}

	/* we've arrived at the beginning of a team definition */
	if(feof(import_file) == 0)
	{	    
	    /* save name in the list */
	    g_string_append_printf(imported_teams, "%s\n", buf);
	    no_imports = FALSE;
	    
	    while(strcmp(buf, "end_players") != 0 &&
		  feof(import_file) == 0)
	    {
		fscanf(import_file, "%[\n \t]*", buf);
		fprintf(definitions_file, "%s", buf);
		fscanf(import_file, "%[^\n]*", buf);
		fprintf(definitions_file, "%s", buf);
	    }
	}
    }

    fclose(import_file);
    fclose(definitions_file);

    if(no_imports)
	g_string_append_printf(imported_teams, "NONE\n");
    
    show_popup_window(imported_teams->str, NULL);

    g_string_free(imported_teams, TRUE);
}
