/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#include <time.h>

#include "defs.h"
#include "callbacks.h"
#include "generation.h"
#include "gui.h"
#include "load_save.h"
#include "main.h"
#include "startup.h"
#include "support.h"
#include "text_files.h"
#include "window.h"

/* set some variables that are used but not saved
   in the game */
void
set_variables(void)
{
    gint i, j;
    gchar buf[BUF_SIZE_SMALL];

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

    status = save_status = popups_active = 0;
    /* reset main window pointer */
    main_window = NULL;
    /* reset default save file name */
    sprintf(buf, "%s/.bygfoot/saves/", getenv("HOME"));
    save_file = g_string_new(buf);
    /* reset history chained list */
    history = NULL;
    /* reset font and country file names */
    strcpy(font_name, "0");
    strcpy(country_file_name, "0");

    /* this plays a role in case the human player has
       specified a country file on the command line */
    strcpy(teams[0].name, "");    
}

gint
bygfoot_init(gint argc, gchar *argv[])
{
    gint i;
    gchar buf[BUF_SIZE_SMALL];
    gchar *buf2;

    /* initialize the random nr generator */
    srandom((unsigned)time(NULL));

    set_variables();
    
    sprintf(buf, "%s/support_files", getenv("PWD"));
    add_support_directory(buf);

    sprintf(buf, "%s/.bygfoot", getenv("HOME"));
    add_support_directory(buf);

    /* look for pixmap directory and country file */
    for(i=1;i<argc - 1;i++)
    {
	if(strcmp(argv[i], "-d") == 0)
	    add_support_directory (argv[i + 1]);
	else if(strcmp(argv[i], "-f") == 0)
	    country_names(0, argv[i + 1]);
    }
    
    /* check for the files with team and player names
       and help file */
    check_files();
    
    check_home_dir();

    /* fill the player names array */
    text_file_number_to_char(TEXT_FILES_PLAYER_NAMES, buf, TRUE);
    get_names(buf, player_names);

    /* look for 'start editor' option */
    for(i=1;i<argc;i++)
	if(strcmp(argv[i], "--editor") == 0 ||
	   strcmp(argv[i], "-e") == 0)
	    return 5;

    main_window = return_main_window();
    
    /* look for savegame */
    for(i=1;i<argc;i++)
	if(strcmp(argv[i - 1], "-d") != 0 &&
	   strcmp(argv[i], "-d") != 0 &&
	   strcmp(argv[i - 1], "-f") != 0 &&
	   strcmp(argv[i], "-f") != 0)
	{
	    sprintf(buf, "%s", argv[i]);
	    buf2 = find_support_file(buf);

	    if(buf2 == NULL || argv[i][0] == '/')
		g_string_assign(save_file, argv[i]);
	    else
		g_string_assign(save_file, buf2);

	    if(buf2 != NULL)
		g_free(buf2);
		
	    if(check_save_game(save_file->str))
	    {
		load_game(save_file->str);		
		on_button_back_to_main_clicked(NULL, NULL);
		gtk_widget_show(main_window);
		set_save(1);
		return 10;
	    }
	    else
	    {		
		g_print("\nUsage: bygfoot [-e|--editor] [-d pixmapdir] [-f country_file] [savegame]\n\n");
		g_print("Options\n");
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

    return 0;
}

gint
main (gint argc, gchar *argv[])
{
    gint return_value;


#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
#endif
    
  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_support_directory(PACKAGE_DATA_DIR "/" PACKAGE "/support_files");
  
  return_value = bygfoot_init(argc, argv);

  if(return_value == 5)
      start_editor();
  else if(return_value == 0)
      start(0);
  
  gtk_main ();
  return 0;
}
