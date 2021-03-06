#include "defs.h"

/* read the file until the next line that's not a comment or
   a blank line and copy the line into buf; or, if find_text is
   not NULL, read the file until 'find_text' is found */
gboolean
get_next_line(FILE *fil, gchar *buf, gchar *find_text)
{
    gchar local_buf[BUF_SIZE_BIG];

    strcpy(local_buf, "");

    if(find_text != NULL)
    {
	while(strcmp(local_buf, find_text) != 0 &&
	      feof(fil) == 0)
	{
	    fscanf(fil, "%[\n \t]*", local_buf);
	    fscanf(fil, "%[^\n]", local_buf);
	}

	if(strcmp(local_buf, find_text) != 0)
	    return FALSE;

	return TRUE;
    }

    while( (local_buf[0] == '#' || strlen(local_buf) == 0) &&
	   feof(fil) == 0)
	{
	    fscanf(fil, "%[\n \t]*", local_buf);
	    fscanf(fil, "%[^\n]", local_buf);
	}

    if(local_buf[0] != '#' && strlen(local_buf) != 0)
    {
	if(strlen(local_buf) > 100)
	    g_print("\n*** Warning: the text file I'm reading contains a line longer than 100 chars.\n\n");

	if(buf != NULL)
	    strcpy(buf, local_buf);
	return TRUE;
    }

    return FALSE;
}

/* write the names of the options that will
   identify them in the conf file into the array */
void
write_opt_names(gchar opt_names[][50])
{
    strcpy(opt_names[OPT_CONF_NEW_WEEK], "confirm_new_week");
    strcpy(opt_names[OPT_CONF_UNFIT], "confirm_unfit");
    strcpy(opt_names[OPT_CONF_QUIT], "confirm_quit");
    strcpy(opt_names[OPT_OVERWRITE], "save_will_ovewrite");
    strcpy(opt_names[OPT_SHOW_MY_GAMES], "new_week_show_my_games");
    strcpy(opt_names[OPT_SHOW_LIVE], "show_live_game");
    strcpy(opt_names[OPT_LIVE_DURATION], "live_game_duration");
    strcpy(opt_names[OPT_LIVE_TENDENCY], "show_tendency_bar");
    strcpy(opt_names[OPT_NOTIFY], "notify_transfer");
    strcpy(opt_names[OPT_NOTIFY_POS0], "notify_goalie");
    strcpy(opt_names[OPT_NOTIFY_POS1], "notify_defender");
    strcpy(opt_names[OPT_NOTIFY_POS2], "notify_midfielder");
    strcpy(opt_names[OPT_NOTIFY_POS3], "notify_forward");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE1], "notify_league_one");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE2], "notify_league_two");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE3], "notify_league_three");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE4], "notify_league_four");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE5], "notify_league_five");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE6], "notify_league_international");
    strcpy(opt_names[OPT_NOTIFY_AGE], "notify_age");
    strcpy(opt_names[OPT_NOTIFY_SKILL], "notify_skill");
    strcpy(opt_names[OPT_NOTIFY_ETAL], "notify_estimated_talent");
    strcpy(opt_names[OPT_NOTIFY_VALUE], "notify_value");
    strcpy(opt_names[OPT_NOTIFY_POS], "notify_position");
    strcpy(opt_names[OPT_NOTIFY_LEAGUE], "notify_league_bool");
    strcpy(opt_names[OPT_JOBS], "show_job_offers");
    strcpy(opt_names[OPT_MESS], "message_style");
    strcpy(opt_names[OPT_PREFER_MESS], "prefer_messages");
    strcpy(opt_names[OPT_AUTOSAVE], "autosave");
    strcpy(opt_names[OPT_SKIP_WEEKS], "skip_weeks");
    strcpy(opt_names[OPT_SORT_TRANSFERS], "sort_transfer_list");
    strcpy(opt_names[OPT_REARRANGE], "reaarrange_adapts");
    strcpy(opt_names[OPT_SWAP], "swap_adapts");
    strcpy(opt_names[OPT_FIT_COLORS], "fitness_colors");

    strcpy(opt_names[OPT_ATT_NAME], "show_attribute_name");
    strcpy(opt_names[OPT_ATT_CPOS], "show_attribute_current_position");
    strcpy(opt_names[OPT_ATT_POS], "show_attribute_position");
    strcpy(opt_names[OPT_ATT_CSKILL], "show_attribute_current_skill");
    strcpy(opt_names[OPT_ATT_SKILL], "show_attribute_skill");
    strcpy(opt_names[OPT_ATT_TALENT], "show_attribute_talent");
    strcpy(opt_names[OPT_ATT_FIT], "show_attribute_fitness");
    strcpy(opt_names[OPT_ATT_GAMES], "show_attribute_games");
    strcpy(opt_names[OPT_ATT_GOALS], "show_attribute_goals");
    strcpy(opt_names[OPT_ATT_STATUS], "show_attribute_status");
    strcpy(opt_names[OPT_ATT_AGE], "show_attribute_age");
    strcpy(opt_names[OPT_ATT_ETAL], "show_attribute_estimated_talent");
    strcpy(opt_names[OPT_ATT_BOOKED], "show_attribute_booked");
    strcpy(opt_names[OPT_ATT_VALUE], "show_attribute_value");
    strcpy(opt_names[OPT_ATT_WAGE], "show_attribute_wage");
    strcpy(opt_names[OPT_ATT_TEAM], "show_attribute_team");
    strcpy(opt_names[OPT_ATT_LEAGUE], "show_attribute_league");
}

/* write the game options to the file
   $HOME/.bygfoot/bygfoot.conf */
void
save_conf_file(void)
{
    gint i;
    gchar opt_names[OPT_DUMMY1][50];
    FILE *fil;
    gchar buf[BUF_SIZE_SMALL];

    sprintf(buf, "%s/.bygfoot/text_files/bygfoot.conf", getenv("HOME"));

    fil = fopen(buf, "w");

    if(fil == NULL)
    {
	show_popup_window("Could not open Bygfoot conf file. ", NULL);
	return;
    }
    
    write_opt_names(opt_names);

    for(i=0;i<OPT_DUMMY1;i++)
	fprintf(fil, "%s %d\n", opt_names[i], options[i]);

    if(font_name != NULL)
	fprintf(fil, "font_name %s\n", font_name);
    else
	fprintf(fil, "font_name (nil)");

    fprintf(fil, "debug %d\n", debug);

    fclose(fil);
}

/* read the bygfoot configurations file */
void
read_conf_file(void)
{
    gint i;
    gchar opt_names[OPT_DUMMY1][50];
    FILE *fil;
    gchar buf[BUF_SIZE_SMALL];
    gchar buf2[BUF_SIZE_SMALL];

    text_file_number_to_char(TEXT_FILES_CONF, buf, TRUE);

    fil = fopen(buf, "r");

    if(fil == NULL)
    {
	g_print("*** Error: could not open conf file %s. ***",
		buf);
	exit(2);
    }

    write_opt_names(opt_names);

    while(feof(fil) == 0)
    {
	get_next_line(fil, buf, NULL);
	
	for(i=0;i<OPT_DUMMY1;i++)
	    if(strncmp(opt_names[i], buf, strlen(opt_names[i])) == 0)
		sscanf(buf, "%[^-0-9 ]%d", buf2, &options[i]);

	if(strncmp(buf, "font_name", strlen("font_name")) == 0)
	    sscanf(buf, "%[font_name ]%[^\n]", buf2, font_name);
	if(strncmp(buf, "debug", strlen("debug")) == 0)
	    sscanf(buf, "%[debug ]%d", buf2, &debug);
    }
}

/* try to make a .bygfoot directory in the user's home directory
   and copy the support files there if possible */
void
check_home_dir(void)
{
    gint i, success = 1;
    gchar home[BUF_SIZE_SMALL];
    gchar buf[BUF_SIZE_SMALL];
    gchar buf2[BUF_SIZE_SMALL];
    gchar buf3[BUF_SIZE_SMALL];
    gchar text_files[TEXT_FILES_END][BUF_SIZE_SMALL];
    gchar text_files_full_path[TEXT_FILES_END][BUF_SIZE_SMALL];

    sprintf(home, "%s", getenv("HOME"));

    for(i=0;i<TEXT_FILES_END;i++)
    {
	text_file_number_to_char(i, text_files_full_path[i], TRUE);
	text_file_number_to_char(i, text_files[i], FALSE);
    }

    sprintf(buf, "test -d %s/.bygfoot", home);
    if(system(buf) != 0)
    {
	g_print("\nCreating %s/.bygfoot. . .\n", home);
	sprintf(buf2, "mkdir -v %s/.bygfoot", home);
	
	if(system(buf2) == 0)
	    g_print("Done.\n");
	else
	    g_print("Failed. This isn't a serious problem if you don't want to edit teams.\nMaybe you should create the directory manually\nand copy all text files from the text_files directory.\n");
    }
	
    if(system(buf) != 0)
	return;
    
    sprintf(buf, "%s/.bygfoot", home);
    sprintf(buf2, "mkdir -v %s/.bygfoot/saves", home);
    sprintf(buf3, "test -d %s/saves", buf);
    if(system(buf3) != 0)
	system(buf2);

    sprintf(buf2, "mkdir -v %s/text_files", buf);
    sprintf(buf3, "test -d %s/text_files", buf);
    if(system(buf3) != 0)
	system(buf2);
    
    for(i=0;i<TEXT_FILES_END;i++)
    {
	sprintf(buf3, "test -e %s/text_files/%s", buf, text_files[i]);
	if(system(buf3) != 0)
	{
	    if(strcmp(text_files_full_path[i], "") != 0)
	    {
		g_print("Copying ");
		sprintf(buf3, "cp -v %s %s/text_files/%s", text_files_full_path[i], buf,
			text_files[i]);
	    }
	    if(system(buf3) != 0)
		success = 0;
	}
    }
    
    if(success == 0)
    {
	g_print("\nSome files could not be copied to %s.\nYou should try to copy them manually:\n\n", buf);
	for(i=0;i<TEXT_FILES_END;i++)
	{
	    sprintf(buf3, "test -e %s/text_files/%s", buf, text_files[i]);
	    if(system(buf3) != 0 && 
	       strcmp(text_files_full_path[i], "") != 0)
		g_print("%s\n", text_files_full_path[i]);
	}
    }
}

/* check for the files with team and player names
   and help file */
void
check_files(void)
{
    gint i;
    gchar filenames[4][BUF_SIZE_SMALL];
    FILE *fil;
    
    text_file_number_to_char(TEXT_FILES_COUNTRY_ENG, filenames[0], TRUE);
    text_file_number_to_char(TEXT_FILES_PLAYER_NAMES, filenames[1], TRUE);
    text_file_number_to_char(TEXT_FILES_CONF, filenames[2], TRUE);
    text_file_number_to_char(TEXT_FILES_HELP, filenames[3], TRUE);
    for(i=0;i<4;i++)
    {
	fil = fopen(filenames[i], "r");
	if(fil == NULL)
	{
	    g_print("\n*** Couldn't find file: %s ***\n\n", filenames[i]);
	    g_print("\n*** You can add the path to the support files by calling 'bygfoot -d my_support_files_path' ***\n\n");
	    if(i < 3)
		exit(5);
	}
	
	if(fil != NULL)
	    fclose(fil);
    }
}

/* write team or player names from 'filename'
   to the array 'names'; both files are read until a '1000' appears
   in a line by itself */
void
get_names(gchar *filename, gchar names[][50])
{
    gint i = 0;
    gint linenr = 0;
    gchar trash[BUF_SIZE_BIG];
    gchar buf[BUF_SIZE_SMALL];
    FILE *fil = fopen(filename, "r");
    
    if(fil == NULL)
    {
	g_print("get_names: could not open file %s\n\n", filename);
	return;
    }

    while(1)
    {
	fscanf(fil,"%[^\n]", trash);
	linenr++;
	if(strcmp(trash, "1000") == 0 || feof(fil) != 0)
	{
	    fclose(fil);
	    break;
	}
	/* ignore comment lines */
	else if(trash[0] != '#')
	{
	    if(strlen(trash) < 50)
	    {
		strcpy(names[i], trash);
		i++;
	    }
	    else
	    {
		sprintf(buf,"*** The lines in '%s' which aren't comment lines (leading '#') must have length < 50. ***\nLine %d is too long! ***\n\n", filename, linenr);
		g_print("%s", buf);
	    }
	}

	fscanf(fil,"%[\n ]*", trash);
    }
}

/* return filename corresponding to a number defined in defs.h/enums.h */
void
text_file_number_to_char(gint number, gchar *filename, gboolean full_path)
{
    gchar buf[BUF_SIZE_SMALL];
    gchar *buf2;

    strcpy(filename, "");

    switch(number)
    {
	default:
	    strcpy(buf, "country_eng");
	    break;
	case TEXT_FILES_COUNTRY_DE:
	    strcpy(buf, "country_de");
	    break;
	case TEXT_FILES_COUNTRY_IT:
	    strcpy(buf, "country_it");
	    break;
	case TEXT_FILES_COUNTRY_FR:
	    strcpy(buf, "country_fr");
	    break;
	case TEXT_FILES_COUNTRY_ES:
	    strcpy(buf, "country_es");
	    break;
	case TEXT_FILES_COUNTRY_RO:
	    strcpy(buf, "country_ro");
	    break;
	case TEXT_FILES_COUNTRY_BR:
	    strcpy(buf, "country_br");
	    break;
	case TEXT_FILES_COUNTRY_PL:
	    strcpy(buf, "country_pl");
	    break;
	case TEXT_FILES_PLAYER_NAMES:
	    strcpy(buf, "player_names");
	    break;
	case TEXT_FILES_DEFINITIONS:
	    strcpy(buf, "teams");
	    break;
	case TEXT_FILES_HELP:
	  strcpy(buf, "help");
	    break;
	case TEXT_FILES_CONF:
	  strcpy(buf, "bygfoot.conf");
	    break;
    }

    buf2 = find_support_file(buf);

    if(full_path && buf2 != NULL)
    {
	strcpy(filename, buf2);
	g_free(buf2);
	return;
    }

    if(buf2 != NULL)
	g_free(buf2);

    strcpy(filename, buf);
}

