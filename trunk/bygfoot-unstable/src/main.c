/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#include <time.h>

#include "callbacks.h"
#include "free.h"
#include "generation.h"
#include "gui.h"
#include "load_save.h"
#include "main.h"
#include "startup.h"
#include "support.h"
#include "files.h"
#include "window.h"

/* set some variables that are used but not saved
   in the game */
void
set_variables(void)
{
    gint i, j;
    gchar buf[SMALL];

    for(i=0;i<2;i++)
    {
	selected_rows[i] = -1;
	undo_info[i] = 0;
	for(j=0;j<2;j++)
	    bookmaker_tips[i][j] = -1;
    }

    /* debug mode or no? */
    debug = FALSE;

    for(i=0;i<NOTIFY_END;i++)
	notify_status[i] = FALSE;

    for(i=0;i<178;i++)
    {
	teams[i].history = NULL;
	for(j=0;j<20;j++)
	    teams[i].players[j].history = NULL;
    }

    status = save_status = popups_active = 0;
    /* reset window pointers */
    main_window = progressbar_window = NULL;
    /* reset default save file name */
    sprintf(buf, "%s/.bygfoot/saves/", g_get_home_dir());
    save_file = g_string_new(buf);	
    /* reset history chained list */
    history = NULL;
    /* reset font and country file names */
    strcpy(font_name, "0");
    strcpy(country_file_name, "0");

    /* this plays a role in case the human player has
       specified a country file on the command line */
    strcpy(teams[0].name, "");

    /* read options from the configuration file */
    read_conf_file();
}

void
bygfoot_init(gint argc, gchar *argv[])
{
    gint i;
    gchar buf[SMALL];
    gchar *pwd = g_get_current_dir();

    /* initialize the random nr generator */
    srandom((unsigned)time(NULL));

    sprintf(buf, "%s/support_files", pwd);
    g_free(pwd);
    add_support_directory_recursive(buf);
    sprintf(buf, "%s/.bygfoot", g_get_home_dir());
    add_support_directory_recursive(buf);

    set_variables();
    
     /* look for pixmap directory and country file */
    for(i=1;i<argc - 1;i++)
    {
	if(strcmp(argv[i], "-d") == 0)
	    add_support_directory_recursive(argv[i + 1]);
	else if(strcmp(argv[i], "-f") == 0)
	    country_names(0, argv[i + 1]);
    }
    
    check_home_dir();

    /* fill the player names array */
    text_file_number_to_char(FILES_PLAYER_NAMES, buf, TRUE);
    get_names(buf, player_names);

    /* look for 'start editor' option */
    for(i=1;i<argc;i++)
	if(strcmp(argv[i], "--editor") == 0 ||
	   strcmp(argv[i], "-e") == 0)
	{
	    start_editor();
	    gtk_main ();
	    return;
	}
	else if(strcmp(argv[i], "--update") == 0 ||
		strcmp(argv[i], "-u") == 0)
	{
	    start_update();	    
	    return;
	}

    main_window = return_main_window();
    
    /* look for savegame */
    for(i=1;i<argc;i++)
	if(strcmp(argv[i - 1], "-d") != 0 &&
	   strcmp(argv[i], "-d") != 0 &&
	   strcmp(argv[i - 1], "-f") != 0 &&
	   strcmp(argv[i], "-f") != 0)
	{
	    if(load_game(argv[i]))
	    {				
		on_button_back_to_main_clicked(NULL, NULL);
		gtk_widget_show(main_window);
		set_save(1);
		gtk_main ();
		return;
	    }
	    else
	    {		
		g_print("\nUsage: bygfoot [-u|--update] [-e|--editor] [-d pixmapdir] [-f country_file] [savegame]\n\n");
		g_print("Options\n");
		g_print("-u, --update:\t Start the GUI frontend for the bygfoot-update script\n");
		g_print("-e, --editor:\t Start Bygfoot Team Editor instead of the game itself\n");
		g_print("-d\t\t Add the 'pixmapdir' directory to the list Bygfoot searches when\n");
		g_print("\t\t looking for text files and pixmap files.\n");
		g_print("-f\t\t Load the file 'country_file' which is structured like\n");
		g_print("\t\t 'country_eng' and specifies team and cup names\n\n");
		g_print("Any other argument is interpreted as a savegame file.\n\n");
		
		if(strcmp(argv[i], "-h") != 0 &&
		   strcmp(argv[i], "--help") != 0)
		    g_print(
			"\n *** Doesn't seem to be a Bygfoot savegame: %s ***\n\n",
			save_file->str);
		
		g_string_free(save_file, TRUE);
		gtk_widget_destroy(main_window);
		exit(0);
	    }
	    break;
	}

    start(0);
    gtk_main ();
    return;
}

gint
main (gint argc, gchar *argv[])
{

#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif
    
  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_support_directory_recursive(PACKAGE_DATA_DIR "/" PACKAGE "/support_files");
  
  bygfoot_init(argc, argv);

  free_memory();
  
  return 0;
}
