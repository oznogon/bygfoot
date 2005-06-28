/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#include <time.h>

#include "file.h"
#include "free.h"
#include "language.h"
#include "live_game.h"
#include "main.h"
#include "misc_callbacks.h"
#include "name_struct.h"
#include "option.h"
#include "stat_struct.h"
#include "transfer_struct.h"
#include "variables.h"
#include "window.h"

/**
   Initialize some global variables. Most of them get nullified.
*/
void
main_init_variables(void)
{
    gint i;

    ligs = cps = NULL;
    acps = NULL;
    country.name = country.symbol = country.sid = NULL;
    
    season = week = week_round = 1;

    for(i=0;i<COUNT_END;i++)
	counters[i] = 0;

    counters[COUNT_LEAGUE_ID] = ID_LEAGUE_START;
    counters[COUNT_CUP_ID] = ID_CUP_START;

    window.main = window.startup =
	window.live = window.warning = window.progress = window.digits =
	window.stadium = window.job_offer = window.yesno = 
	window.options = window.font_sel =
	window.file_chooser = window.contract = 
	window.menu_player = window.user_management = NULL;
    
    live_game_reset(&live_game_temp, NULL, FALSE);

    users = g_array_new(FALSE, FALSE, sizeof(User));
    transfer_list = g_array_new(FALSE, FALSE, sizeof(Transfer));
    season_stats = g_array_new(FALSE, FALSE, sizeof(SeasonStat));
    name_lists = g_array_new(FALSE, FALSE, sizeof(NameList));

    save_file = g_string_new("");

    constants_app.list = settings.list =
	constants.list = options.list = lg_tokens.list = NULL;
    constants_app.datalist = settings.datalist =
	constants.datalist = options.datalist = lg_tokens.datalist = NULL;

    popups_active = 0;
    selected_row[0] = selected_row[1] = -1;

    timeout_id = -1;

    for(i=0;i<LIVE_GAME_EVENT_END;i++)
	lg_commentary[i] = g_ptr_array_new();

    file_load_conf_files();

    language_set(language_get_code_index(opt_str("string_opt_language_code")) + 1);

    /** Some of these (or all) are disabled (set to 1) in supernational
	country defs. */
    option_add(&settings, "int_opt_disable_finances", 0, NULL);
    option_add(&settings, "int_opt_disable_transfers", 0, NULL);
    option_add(&settings, "int_opt_disable_stadium", 0, NULL);
    option_add(&settings, "int_opt_disable_contracts", 0, NULL);
    option_add(&settings, "int_opt_disable_boost_on", 0, NULL);
}

/**
   Process the command line arguments and do some things
   that have to be done at the beginning (like initializing the
   random number generator).
   @param argc Number of command line arguments.
   @param argv Command line arguments array.
*/
void
main_init(gint argc, gchar *argv[])
{
    gchar buf[SMALL];
    gchar *pwd = g_get_current_dir();

    /* initialize the random nr generator */
    rand_generator = g_rand_new();

    support_directories = NULL;

    file_add_support_directory_recursive(PACKAGE_DATA_DIR "/" PACKAGE "/support_files");

    sprintf(buf, "%s/%s", g_get_home_dir(), HOMEDIRNAME);
    file_add_support_directory_recursive(buf);

    sprintf(buf, "%s/support_files", pwd);
    g_free(pwd);
    file_add_support_directory_recursive(buf);
    
    file_check_home_dir();

    main_init_variables();
}

/**
   Initialize the GTK stuff and the gettext stuff.
   Start the game.
   @param argc Number of command line arguments.
   @param argv Command line arguments array.
*/
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
  
    main_init(argc, argv);

    window_show_startup();
    stat0 = STATUS_TEAM_SELECTION;

    gtk_main ();

    main_exit_program(EXIT_OK, NULL);

    return 0;
}

/** Exit the program with the given exit code and message. Try to
    destroy all widgets and free all memory first.
    @param exit_code The number we return to the shell.
    @param exit_message The message we print.
    @return The exit code of the program. */
void
main_exit_program(gint exit_code, gchar *exit_message)
{
    if(gtk_main_level() > 0)
	gtk_main_quit();

    free_memory();
    
    if(exit_message != NULL)
	g_warning(exit_message);
    
    exit(exit_code);
}
