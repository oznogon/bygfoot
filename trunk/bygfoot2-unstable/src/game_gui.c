#include <unistd.h>

#include "file.h"
#include "game_gui.h"
#include "gui.h"
#include "league.h"
#include "live_game.h"
#include "maths.h"
#include "option.h"
#include "treeview.h"
#include "support.h"
#include "team.h"
#include "user.h"
#include "variables.h"
#include "window.h"

/** Show the live game in the live game window.
    @param unit The current unit we show.
    @param sleep_factor The factor determining the live game speed. */
void
game_gui_live_game_show_unit(const LiveGameUnit *unit)
{
    gchar buf[SMALL];
    gfloat fraction = (gfloat)live_game_unit_get_minute(unit) / 90;
    GtkProgressBar *progress_bar;
    GtkWidget *button_pause = lookup_widget(window.live, "button_pause"),
	*button_resume = lookup_widget(window.live, "button_resume"),
	*button_live_close = lookup_widget(window.live, "button_live_close"),
	*eventbox_poss[2] = {lookup_widget(window.live, "eventbox_poss0"),
			     lookup_widget(window.live, "eventbox_poss1")};
    GdkColor color;

    if(unit->event.type == LIVE_GAME_EVENT_START_MATCH)
	treeview_live_game_show_initial_commentary(unit);
    else
	treeview_live_game_show_commentary(unit);

    treeview_live_game_show_result(unit);

    gdk_color_parse (const_str("string_live_game_possession_color"), &color);
    gtk_widget_modify_bg(eventbox_poss[unit->possession], GTK_STATE_NORMAL, &color);
    gtk_widget_modify_bg(eventbox_poss[!unit->possession], GTK_STATE_NORMAL, NULL);

    game_gui_live_game_set_hscale(unit,
				  GTK_HSCALE(lookup_widget(window.live, "hscale_area")));

    sprintf(buf, "%d.", live_game_unit_get_minute(unit));
    progress_bar = GTK_PROGRESS_BAR(lookup_widget(window.live, "progressbar_live"));
    gtk_progress_bar_set_fraction(progress_bar, (fraction > 1) ? 1 : fraction);
    gtk_progress_bar_set_text(progress_bar, buf);
    usleep(500500 + option_int("int_opt_user_live_game_speed",
				 usr(stat2).options) * 50000);
    while(gtk_events_pending())
	gtk_main_iteration();

    if(unit->event.type == LIVE_GAME_EVENT_START_MATCH)
    {
	gtk_widget_set_sensitive(button_live_close, FALSE);
	gtk_widget_show(button_pause);
	gtk_widget_hide(button_resume);
    }
    else if(unit->event.type == LIVE_GAME_EVENT_END_MATCH)
    {
	gtk_widget_set_sensitive(lookup_widget(window.live, "button_live_close"), TRUE);
	gtk_widget_hide(lookup_widget(window.live, "button_pause"));
    }
    else if(unit->event.type == LIVE_GAME_EVENT_PENALTIES)
	gtk_widget_hide(lookup_widget(window.live, "button_pause"));	
}

/** Set the area scale position and color in the live game window.
    @param unit The current unit.
    @param hscale The scale widget. */
void
game_gui_live_game_set_hscale(const LiveGameUnit *unit, GtkHScale *hscale)
{
    GdkColor color;

    gtk_widget_modify_bg(GTK_WIDGET(hscale), GTK_STATE_NORMAL, NULL);

    if(unit->area == LIVE_GAME_UNIT_AREA_MIDFIELD)
    {
	gdk_color_parse(const_str("string_game_gui_live_game_scale_color_midfield"), &color);
	gtk_range_set_value(GTK_RANGE(hscale),
			    const_float("float_game_gui_live_game_scale_range") / 2);
    }
    else if(unit->event.type == LIVE_GAME_EVENT_GOAL ||
	    unit->event.type == LIVE_GAME_EVENT_OWN_GOAL)
    {
	gdk_color_parse(const_str("string_game_gui_live_game_scale_color_goal"), &color);

	gtk_range_set_value(GTK_RANGE(hscale), 
			    const_float("float_game_gui_live_game_scale_range") *
			    (unit->possession == 0));
    }
    else if(unit->event.type == LIVE_GAME_EVENT_SCORING_CHANCE ||
	    unit->event.type == LIVE_GAME_EVENT_PENALTY ||
	    unit->event.type == LIVE_GAME_EVENT_FREE_KICK)
    {
	gdk_color_parse(const_str("string_game_gui_live_game_scale_color_chance"), &color);

	gtk_range_set_value(GTK_RANGE(hscale), 
			    const_float("float_game_gui_live_game_scale_range") / 2 +
			    (const_float("float_game_gui_live_game_scale_range") * 
			     const_float("float_game_gui_live_game_scale_chance") *
			     ((unit->possession == 0) ? 1 : -1)));
    }
    else if(unit->event.type == LIVE_GAME_EVENT_POST ||
	    unit->event.type == LIVE_GAME_EVENT_MISSED ||
	    unit->event.type == LIVE_GAME_EVENT_SAVE ||
	    unit->event.type == LIVE_GAME_EVENT_CROSS_BAR)
	gdk_color_parse(const_str("string_game_gui_live_game_scale_color_miss"), &color);
    else if(unit->area == LIVE_GAME_UNIT_AREA_ATTACK)
    {
	gdk_color_parse(const_str("string_game_gui_live_game_scale_color_attack"), &color);	
	gtk_range_set_value(GTK_RANGE(hscale),
			    const_float("float_game_gui_live_game_scale_range") / 2 +
			    (const_float("float_game_gui_live_game_scale_range") * 
			     const_float("float_game_gui_live_game_scale_attack") *
			     ((unit->possession == 0) ? 1 : -1)));
    }
    else if(unit->area == LIVE_GAME_UNIT_AREA_DEFEND)
    {
	gdk_color_parse(const_str("string_game_gui_live_game_scale_color_defend"), &color);
	gtk_range_set_value(GTK_RANGE(hscale),
			    const_float("float_game_gui_live_game_scale_range") / 2 +
			    (const_float("float_game_gui_live_game_scale_range") * 
			     const_float("float_game_gui_live_game_scale_attack") *
			     ((unit->possession == 0) ? -1 : 1)));
    }
    else
	g_warning("game_gui_live_game_set_hscale: don't know what to do!\n");

    gtk_widget_modify_bg(GTK_WIDGET(hscale), GTK_STATE_NORMAL, &color);
}

/** Look up the widgets in the main window. */
void
game_gui_get_radio_items(GtkWidget **style, GtkWidget **scout,
			 GtkWidget **physio, GtkWidget **boost)
{
    style[0] = lookup_widget(window.main, "menu_all_out_defend");
    style[1] = lookup_widget(window.main, "menu_defend");
    style[2] = lookup_widget(window.main, "menu_balanced");
    style[3] = lookup_widget(window.main, "menu_attack");
    style[4] = lookup_widget(window.main, "menu_all_out_attack");

    scout[0] = lookup_widget(window.main, "menu_scout_best");
    scout[1] = lookup_widget(window.main, "menu_scout_good");
    scout[2] = lookup_widget(window.main, "menu_scout_average");
    scout[3] = lookup_widget(window.main, "menu_scout_bad");

    physio[0] = lookup_widget(window.main, "menu_physio_best");
    physio[1] = lookup_widget(window.main, "menu_physio_good");
    physio[2] = lookup_widget(window.main, "menu_physio_average");
    physio[3] = lookup_widget(window.main, "menu_physio_bad");

    boost[0] = lookup_widget(window.main, "menu_boost_anti");
    boost[1] = lookup_widget(window.main, "menu_boost_off");
    boost[2] = lookup_widget(window.main, "menu_boost_on");
}

/** Set information like season, user, week etc. into the appropriate labels. */
void
game_gui_set_main_window_header(void)
{
    GtkLabel *label_user= GTK_LABEL(lookup_widget(window.main, "label_user"));
    GtkLabel *label_season= GTK_LABEL(lookup_widget(window.main, "label_season"));
    GtkLabel *label_week= GTK_LABEL(lookup_widget(window.main, "label_week"));
    GtkLabel *label_round= GTK_LABEL(lookup_widget(window.main, "label_round"));
    GtkLabel *label_team= GTK_LABEL(lookup_widget(window.main, "label_team"));
    GtkLabel *label_league= GTK_LABEL(lookup_widget(window.main, "label_league"));
    GtkLabel *label_rank= GTK_LABEL(lookup_widget(window.main, "label_rank"));
    GtkLabel *label_money= GTK_LABEL(lookup_widget(window.main, "label_money"));

    gtk_label_set_text(label_user, usr(current_user).name->str);
    gui_label_set_text_from_int(label_season, season, FALSE);
    gui_label_set_text_from_int(label_week, week, FALSE);
    gui_label_set_text_from_int(label_round, week_round, FALSE);
    gui_label_set_text_from_int(label_rank, week_round, FALSE);
    gui_label_set_text_from_int(label_money, usr(current_user).finances[FIN_MONEY], FALSE);
    gui_label_set_text_from_int(label_rank, team_rank(usr(current_user).tm), FALSE);

    gtk_label_set_text(label_team, usr(current_user).tm->name->str);
    gtk_label_set_text(label_league, league_from_clid(usr(current_user).tm->clid)->name->str);    

    game_gui_write_av_skills();

    game_gui_write_radio_items();
}

/** Set the average skills of the current team
    into the appropriate labels. */
void
game_gui_write_av_skills(void)
{
    gchar buf[SMALL];
    GtkLabel *label_av_skills= GTK_LABEL(lookup_widget(window.main, "label_av_skills"));

    sprintf(buf, "%.1f  %.1f", 
	    team_get_average_skill(usr(current_user).tm, TRUE),
	    team_get_average_skill(usr(current_user).tm, FALSE));
    gtk_label_set_text(label_av_skills, buf);
}

/** Activate the appropriate radio items for
    playing style etc. according to the user settings. */
void
game_gui_write_radio_items(void)
{
    GtkWidget *style[5], *scout[4], *physio[4], *boost[3];

    game_gui_get_radio_items(style, scout, physio, boost);

    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(style[usr(current_user).tm->style + 2]), TRUE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(scout[usr(current_user).scout]), TRUE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(physio[usr(current_user).physio]), TRUE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(boost[usr(current_user).tm->boost + 1]), TRUE);
}

/** Set playing style etc. variables according to
    the items.
    @param widget The widget that received a click. */
void
game_gui_read_radio_items(GtkWidget *widget)
{
    gint i;
    GtkWidget *boost[3];
    GtkWidget *style[5], *scout[4], *physio[4];
    gint old_scout = usr(current_user).scout,
	old_physio = usr(current_user).physio;

    game_gui_get_radio_items(style, scout, physio, boost);

    for(i=0;i<3;i++)
	if(widget == boost[i])
	    usr(current_user).tm->boost = i - 1;

    for(i=0;i<5;i++)
	if(widget == style[i])
	    usr(current_user).tm->style = i - 2;
    
    for(i=0;i<4;i++)
	if(widget == scout[i])
	    usr(current_user).scout = 100 + i * 10 + old_scout % 10;

    for(i=0;i<4;i++)
	if(widget == physio[i])
	    usr(current_user).physio = 100 + i * 10 + old_physio % 10;
    
    if(math_get_place(usr(current_user).scout, 2) == old_scout % 10)
	usr(current_user).scout = old_scout % 10;

    if(math_get_place(usr(current_user).physio, 2) == old_physio % 10)
	usr(current_user).physio = old_physio % 10;

    if(old_scout != usr(current_user).scout ||
       old_physio != usr(current_user).physio)
	game_gui_print_message(_("Next week you'll fire him and hire a new one."));
}

/** Show the main menu. */
void
game_gui_show_main(void)
{
    game_gui_set_main_window_header();
    treeview_show_user_player_list(&usr(current_user), 1);
    treeview_show_user_player_list(&usr(current_user), 2);
}

/** Print a message into the main window entry. */
void
game_gui_print_message(gchar *text)
{
    gtk_entry_set_text(GTK_ENTRY(lookup_widget(window.main, "entry_message")), text);

    g_timeout_add(const_int("int_game_gui_message_duration") * 1000,
		  (GSourceFunc)game_gui_clear_entry_message, NULL);
}

/** Function that gets called from time to time. */
gboolean
game_gui_clear_entry_message(gpointer data)
{
    gtk_entry_set_text(GTK_ENTRY(lookup_widget(window.main, "entry_message")), "");

    return FALSE;
}

/** Which parts of the main window get affected
    by a live game pause. */
enum MainWindowInensitiveItems
{
    INSENSITIVE_ITEM_TOOLBAR = 0,
    INSENSITIVE_ITEM_PLAYER_LIST2,
    INSENSITIVE_ITEM_MENU_FILE,
    INSENSITIVE_ITEM_MENU_OPTIONS,
    INSENSITIVE_ITEM_MENU_FIGURES,
    INSENSITIVE_ITEM_MENU_SCOUT,
    INSENSITIVE_ITEM_MENU_PHYSIO,
    INSENSITIVE_ITEM_MENU_BROWSE_TEAMS,
    INSENSITIVE_ITEM_MENU_PUT_ON_TRANSFER_LIST,
    INSENSITIVE_ITEM_MENU_REMOVE_FROM_TRANSFER_LIST,
    INSENSITIVE_ITEM_MENU_FIRE,
    INSENSITIVE_ITEM_MENU_MANAGE_USERS,
    INSENSITIVE_ITEM_MENU_FINANCES_STADIUM,
    INSENSITIVE_ITEM_MENU_HELP,
    INSENSITIVE_ITEM_END
};

/** Set appropriate parts of the main window insensitive when
    the live game is paused or resumed.
    @param value Whether we set sensitive or insensitive. */
void
game_gui_set_main_window_sensitivity(gboolean value)
{
    gint i;
    GtkWidget *insensitive_items[INSENSITIVE_ITEM_END];
    
    insensitive_items[INSENSITIVE_ITEM_TOOLBAR] = 
	lookup_widget(window.main, "hbox1");
    insensitive_items[INSENSITIVE_ITEM_PLAYER_LIST2] = 
	lookup_widget(window.main, "vbox8");
    insensitive_items[INSENSITIVE_ITEM_MENU_FILE] = 
	lookup_widget(window.main, "menu_file");
    insensitive_items[INSENSITIVE_ITEM_MENU_OPTIONS] = 
	lookup_widget(window.main, "menu_options");
    insensitive_items[INSENSITIVE_ITEM_MENU_FIGURES] = 
	lookup_widget(window.main, "menu_figures");
    insensitive_items[INSENSITIVE_ITEM_MENU_SCOUT] = 
	lookup_widget(window.main, "menu_scout");
    insensitive_items[INSENSITIVE_ITEM_MENU_PHYSIO] = 
	lookup_widget(window.main, "menu_physio");
    insensitive_items[INSENSITIVE_ITEM_MENU_BROWSE_TEAMS] = 
	lookup_widget(window.main, "menu_browse_teams");
    insensitive_items[INSENSITIVE_ITEM_MENU_PUT_ON_TRANSFER_LIST] = 
	lookup_widget(window.main, "menu_put_on_transfer_list");
    insensitive_items[INSENSITIVE_ITEM_MENU_REMOVE_FROM_TRANSFER_LIST] = 
	lookup_widget(window.main, "menu_remove_from_transfer_list");
    insensitive_items[INSENSITIVE_ITEM_MENU_FIRE] = 
	lookup_widget(window.main, "menu_fire");
    insensitive_items[INSENSITIVE_ITEM_MENU_MANAGE_USERS] = 
	lookup_widget(window.main, "menu_manage_users");
    insensitive_items[INSENSITIVE_ITEM_MENU_FINANCES_STADIUM] = 
	lookup_widget(window.main, "menu_finances_stadium");
    insensitive_items[INSENSITIVE_ITEM_MENU_HELP] = 
	lookup_widget(window.main, "menu_help");

    for(i=0;i<INSENSITIVE_ITEM_END;i++)
	gtk_widget_set_sensitive(insensitive_items[i], !value);

    gtk_widget_set_sensitive(window.main, value);
}

/** Show a window with a warning.
    @param text The text to show in the window. */
void
game_gui_show_warning(gchar *text)
{
    window_create(WINDOW_WARNING);

    gtk_label_set_text(GTK_LABEL(lookup_widget(window.warning, "label_warning")), text);    
}
