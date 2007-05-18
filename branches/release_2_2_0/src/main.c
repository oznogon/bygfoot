/*
   main.c

   Bygfoot Football Manager -- a small and simple GTK2-based
   football management game.

   http://bygfoot.sourceforge.net

   Copyright (C) 2005  Gyözö Both (gyboth@bygfoot.com)

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/*
 * Initial main.c file generated by Glade. Edit as required.
 * Glade will not overwrite this file.
 */

#include <time.h>
#include <glib/gprintf.h>

#include "bet_struct.h"
#include "debug.h"
#include "file.h"
#include "free.h"
#include "job_struct.h"
#include "language.h"
#include "lg_commentary.h"
#include "live_game.h"
#include "load_save.h"
#include "main.h"
#include "misc.h"
#include "misc_callbacks.h"
#include "name_struct.h"
#include "option.h"
#include "start_end.h"
#include "stat_struct.h"
#include "strategy_struct.h"
#include "transfer_struct.h"
#include "variables.h"
#include "window.h"
#include "xml_strategy.h"

/** Whether the last save gets loaded at startup
    (cl switch -l). */
gboolean load_last_save;

/** Parse the command line arguments given by the user. */
void
main_parse_cl_arguments(gint *argc, gchar ***argv)
{
    gboolean testcom = FALSE, calodds = FALSE;
    gchar *support_dir = NULL, *lang = NULL,
	*testcom_file = NULL, *token_file = NULL, 
	*event_name = NULL, *debug_text = NULL;
    gint deb_level = -1, number_of_passes = 1,
	deb_writer = -1,
	num_matches = 100, skilldiffmax = 20;
    GError *error = NULL;
    GOptionContext *context = NULL;
    GOptionEntry entries[] =
	{{ "support-dir", 's', 0, G_OPTION_ARG_STRING, &support_dir, 
	   _("Specify additional support directory (takes priority over default ones)"), "DIR" },

	 { "debug-level", 'd', 0, G_OPTION_ARG_INT, &deb_level, "[developer] Debug level to use", "N" },

	 { "debug-writer", 'w', 0, G_OPTION_ARG_INT, &deb_writer, "[developer] Debug writer level to use", "N" },

	 { "lang", 'L', 0, G_OPTION_ARG_STRING, &lang, _("Language to use (a code like 'de')"), "CODE" },

	 { "last-save", 'l', 0, G_OPTION_ARG_NONE, &load_last_save, _("Load last savegame"), NULL },

	 { "testcom", 't', 0, G_OPTION_ARG_NONE, &testcom, _("Test an XML commentary file"), NULL },

	 { "commentary-file", 'c', 0, G_OPTION_ARG_STRING, &testcom_file,
	   _("Commentary file name (may be in a support dir)"), "FILE" },

	 { "token-file", 'T', 0, G_OPTION_ARG_STRING, &token_file,
	   /* Live game tokens are strings like _AT_ in live game commentary files
	      which will be filled in by the program during runtime; e.g. _AT_ becomes
	      the attendace for the match. */
	   _("File containing live game tokens (may be in a support dir)"), "FILE" },

	 { "event-name", 'e', 0, G_OPTION_ARG_STRING, &event_name,
	   _("Commentary event to test; leave out to test all commentaries"), "EVENTNAME" },

	 { "num-passes", 'n', 0, G_OPTION_ARG_INT, &number_of_passes,
	   _("How many commentaries to generate per event"), "N" },

	 { "calodds", 'o', 0, G_OPTION_ARG_NONE, &calodds,
	   "[developer] Calibrate the betting odds by simulating a lot of matches", NULL },

	 { "num-matches", 'm', 0, G_OPTION_ARG_INT, &num_matches,
	   "[developer] How many matches to simulate per skill diff step", "N" },

	 { "num-skilldiff", 'S', 0, G_OPTION_ARG_INT, &skilldiffmax,
	   "[developer] How many skill diff steps to take", "N" },

	 { "deb", 'D', 0, G_OPTION_ARG_STRING, &debug_text,
	   "[developer] A debug command like 'deb100 to set the debug level'; see the debug window and debug.c", "STRING" },

	 {NULL}};

    if(argc == NULL || argv == NULL)
	return;

    context = g_option_context_new(_("- a simple and addictive GTK2 football manager"));
    g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
    g_option_context_add_group(context, gtk_get_option_group (TRUE));
    g_option_context_parse(context, argc, argv, &error);
    g_option_context_free(context);

    misc_print_error(&error, TRUE);

    if(calodds)
    {
	option_add(&options, "int_opt_calodds", 1, NULL);
	option_add(&options, "int_opt_calodds_skilldiffmax", skilldiffmax, NULL);
	option_add(&options, "int_opt_calodds_matches", num_matches, NULL);
    }

    if(testcom)
    {
	lg_commentary_test(testcom_file, token_file, event_name, number_of_passes);
	main_exit_program(EXIT_OK, NULL);
    }

    if(support_dir != NULL)
    {
	gchar *fullpath = g_path_get_dirname(support_dir);
	file_add_support_directory_recursive(fullpath);
	g_free(fullpath);
	g_free(support_dir);
    }

    if(deb_level != -1)
	option_set_int("int_debug", &constants, deb_level);

    if(deb_writer != -1)
	option_set_int("int_debug_writer", &constants, deb_writer);

    if(lang != NULL)
    {
	language_set(language_get_code_index(lang) + 1);
	file_load_hints_file();
    }

    if(debug_text != NULL)
	statp = debug_text;
}

/**
   Initialize some global variables. Most of them get nullified.
*/
void
main_init_variables(void)
{
    gint i;

    ligs = g_array_new(FALSE, FALSE, sizeof(League));
    cps = g_array_new(FALSE, FALSE, sizeof(Cup));;
    acps = g_ptr_array_new();
    country.name = NULL;
    country.symbol = NULL;
    country.sid = g_strdup("NONAME");

    season = week = week_round = 1;

    for(i=0;i<COUNT_END;i++)
	counters[i] = 0;

    counters[COUNT_LEAGUE_ID] = ID_LEAGUE_START;
    counters[COUNT_CUP_ID] = ID_CUP_START;
    counters[COUNT_HINT_NUMBER] = -1;

    window.main = window.startup =
	window.live = window.warning = window.progress = window.digits =
	window.stadium = window.job_offer = window.yesno = 
	window.options = window.font_sel =
	window.file_chooser = window.contract = 
	window.menu_player = window.user_management = 
	window.mmatches = window.bets = window.splash =
	window.training_camp = NULL;
    
    live_game_reset(&live_game_temp, NULL, FALSE);

    users = g_array_new(FALSE, FALSE, sizeof(User));
    transfer_list = g_array_new(FALSE, FALSE, sizeof(Transfer));
    season_stats = g_array_new(FALSE, FALSE, sizeof(SeasonStat));
    name_lists = g_array_new(FALSE, FALSE, sizeof(NameList));
    strategies = g_array_new(FALSE, FALSE, sizeof(Strategy));
    bets[0] = g_array_new(FALSE, FALSE, sizeof(BetMatch));
    bets[1] = g_array_new(FALSE, FALSE, sizeof(BetMatch));
    jobs = g_array_new(FALSE, FALSE, sizeof(Job));
    job_teams = g_array_new(FALSE, FALSE, sizeof(Team));

    save_file = NULL;

    constants_app.list = settings.list =
	constants.list = options.list = tokens.list = 
	hints.list = NULL;
    constants_app.datalist = settings.datalist =
	constants.datalist = options.datalist = 
	tokens.datalist = hints.datalist = NULL;

    selected_row = -1;

    timeout_id = -1;

    for(i=0;i<LIVE_GAME_EVENT_END;i++)
	lg_commentary[i] = g_array_new(FALSE, FALSE, sizeof(LGCommentary));

    file_load_conf_files();
    xml_strategy_load_strategies();

    language_set(language_get_code_index(opt_str("string_opt_language_code")) + 1);

    option_add(&options, "int_opt_calodds", 0, NULL);

    /** Some of these (or all) are disabled (set to 1) in supernational
	country defs. */
    option_add(&settings, "int_opt_disable_finances", 0, NULL);
    option_add(&settings, "int_opt_disable_transfers", 0, NULL);
    option_add(&settings, "int_opt_disable_stadium", 0, NULL);
    option_add(&settings, "int_opt_disable_contracts", 0, NULL);
    option_add(&settings, "int_opt_disable_boost_on", 0, NULL);
    option_add(&settings, "int_opt_disable_ya", 0, NULL);
    option_add(&settings, "int_opt_disable_training_camp", 0, NULL);
}

/**
   Process the command line arguments and do some things
   that have to be done at the beginning (like initializing the
   random number generator).
   @param argc Number of command line arguments.
   @param argv Command line arguments array.
*/
void
main_init(gint *argc, gchar ***argv)
{
    gchar buf[SMALL];
    gchar *pwd = g_get_current_dir();

#ifdef G_OS_WIN32
    os_is_unix = FALSE;
#else
    os_is_unix = TRUE;
#endif

    support_directories = NULL;

#ifdef G_OS_UNIX
    file_add_support_directory_recursive(PACKAGE_DATA_DIR "/" PACKAGE "/support_files");
    sprintf(buf, "%s%s%s", g_get_home_dir(), G_DIR_SEPARATOR_S, HOMEDIRNAME);
    file_add_support_directory_recursive(buf);
#endif

    sprintf(buf, "%s%ssupport_files", pwd, G_DIR_SEPARATOR_S);
    file_add_support_directory_recursive(buf);
    sprintf(buf, "%s%ssaves", pwd, G_DIR_SEPARATOR_S);
    file_add_support_directory_recursive(buf);
    g_free(pwd);
    
    /* initialize the random nr generator */
    rand_generator = g_rand_new();

    main_init_variables();

    load_last_save = FALSE;
    main_parse_cl_arguments(argc, argv);
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

    main_init(&argc, &argv);

    if((load_last_save && !load_game_from_command_line("last_save")) ||
       (!load_last_save && (argc == 1 ||
			    (argc > 1 && !load_game_from_command_line(argv[1])))))
    {
	stat0 = STATUS_SPLASH;
	window_show_splash();

	if(os_is_unix)
	    file_check_home_dir();
    }

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
main_exit_program(gint exit_code, gchar *format, ...)
{
    gchar text[SMALL];
    va_list args;
     
    if(format != NULL)
    {
	va_start (args, format);
	g_vsprintf(text, format, args);
	va_end (args);
	g_warning(text);
    }

    if(gtk_main_level() > 0)
	gtk_main_quit();

    free_memory();

    if(!os_is_unix && exit_code != EXIT_OK)
    {
	g_warning("Press RETURN. Program will exit.");
	getchar();
    }
    
    exit(exit_code);
}
