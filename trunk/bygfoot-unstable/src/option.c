#include "files.h"
#include "maths.h"
#include "option.h"
#include "support.h"
#include "window.h"

/* this will be needed when dealing with the options
   window */
enum
{
    ZERO_ONE0 = 0,
    ZERO_ONE1,
    ZERO_ONE2,
    ZERO_ONE3,
    ZERO_ONE4,
    ZERO_ONE5,
    ZERO_ONE6,
    ZERO_ONE7,
    ZERO_ONE8,
    ZERO_ONE9,
    ZERO_ONE12,
    ZERO_ONE13,
    ZERO_ONE14,
    ZERO_ONE15,
    ZERO_ONE16,
    ZERO_ONE17,
    ZERO_ONE18,
    ZERO_ONE19,
    ZERO_ONE20,
    ZERO_ONE21,
    ZERO_ONE22,
    ZERO_ONE23,
    ZERO_ONE24,
    ZERO_ONE25,
    ZERO_ONE26,
    ZERO_ONE27,
    ZERO_ONE28,
    ZERO_ONE30,
    ZERO_ONE31,
    ZERO_ONE32,
    ZERO_ONE33,
    ZERO_ONE34,
    ZERO_ONE35,
    ZERO_ONE36,
    ZERO_ONE37,
    ZERO_ONE38,
    ZERO_ONE39,
    ZERO_ONE40,
    ZERO_ONE41,
    ZERO_ONE42,
    ZERO_ONE43,
    ZERO_ONE44,
    ZERO_ONE45,
    ZERO_ONE46,
    ZERO_ONE47,
    ZERO_ONE48,
    ZERO_ONE_END
};

void
get_option_widgets(GtkWidget *opt_window,
		   gint **zero_one_options,
		   GtkWidget **zero_one_widgets,
		   GtkWidget **radio_buttons,
		   GtkWidget **radiobutton_sort,
		   GtkWidget **spinbuttons_history)
{
    zero_one_widgets[ZERO_ONE0] =
	lookup_widget(opt_window, "check_conf_new_week");
    zero_one_options[ZERO_ONE0] = &options[OPT_CONF_NEW_WEEK];

    zero_one_widgets[ZERO_ONE1] =
	lookup_widget(opt_window, "check_conf_unfit");
    zero_one_options[ZERO_ONE1] = &options[OPT_CONF_UNFIT];

    zero_one_widgets[ZERO_ONE2] =
	lookup_widget(opt_window, "check_conf_quit");
    zero_one_options[ZERO_ONE2] = &options[OPT_CONF_QUIT];

    zero_one_widgets[ZERO_ONE3] =
	lookup_widget(opt_window, "check_overwrite");
    zero_one_options[ZERO_ONE3] = &options[OPT_OVERWRITE];

    zero_one_widgets[ZERO_ONE4] =
	lookup_widget(opt_window, "checkbutton_skip_weeks");
    zero_one_options[ZERO_ONE4] = &options[OPT_SKIP_WEEKS];

    zero_one_widgets[ZERO_ONE5] =
	lookup_widget(opt_window, "checkbutton_prefer_mess");
    zero_one_options[ZERO_ONE5] = &options[OPT_PREFER_MESS];

    zero_one_widgets[ZERO_ONE6] =
	lookup_widget(opt_window, "check_show_live");
    zero_one_options[ZERO_ONE6] = &options[OPT_SHOW_LIVE];

    zero_one_widgets[ZERO_ONE7] =
	lookup_widget(opt_window, "checkbutton_rearrange");
    zero_one_options[ZERO_ONE7] = &options[OPT_REARRANGE];

    zero_one_widgets[ZERO_ONE8] =
	lookup_widget(opt_window, "checkbutton_swap");
    zero_one_options[ZERO_ONE8] = &options[OPT_SWAP];

    zero_one_widgets[ZERO_ONE9] =
	lookup_widget(opt_window, "checkbutton_fit_colors");
    zero_one_options[ZERO_ONE9] = &options[OPT_FIT_COLORS];

    zero_one_widgets[ZERO_ONE12] =
	lookup_widget(opt_window, "check_jobs");
    zero_one_options[ZERO_ONE12] = &options[OPT_JOBS];

    zero_one_widgets[ZERO_ONE24] =
	lookup_widget(opt_window, "check_att_name");
    zero_one_options[ZERO_ONE24] = &options[OPT_ATT_NAME];

    zero_one_widgets[ZERO_ONE25] =
	lookup_widget(opt_window, "check_att_cpos");
    zero_one_options[ZERO_ONE25] = &options[OPT_ATT_CPOS];

    zero_one_widgets[ZERO_ONE13] =
	lookup_widget(opt_window, "check_att_pos");
    zero_one_options[ZERO_ONE13] = &options[OPT_ATT_POS];

    zero_one_widgets[ZERO_ONE26] =
	lookup_widget(opt_window, "check_att_cskill");
    zero_one_options[ZERO_ONE26] = &options[OPT_ATT_CSKILL];

    zero_one_widgets[ZERO_ONE14] =
	lookup_widget(opt_window, "check_att_skill");
    zero_one_options[ZERO_ONE14] = &options[OPT_ATT_SKILL];

    zero_one_widgets[ZERO_ONE15] =
	lookup_widget(opt_window, "check_att_fit");
    zero_one_options[ZERO_ONE15] = &options[OPT_ATT_FIT];

    zero_one_widgets[ZERO_ONE16] =
	lookup_widget(opt_window, "check_att_games");
    zero_one_options[ZERO_ONE16] = &options[OPT_ATT_GAMES];

    zero_one_widgets[ZERO_ONE17] =
	lookup_widget(opt_window, "check_att_goals");
    zero_one_options[ZERO_ONE17] = &options[OPT_ATT_GOALS];

    zero_one_widgets[ZERO_ONE18] =
	lookup_widget(opt_window, "check_att_status");
    zero_one_options[ZERO_ONE18] = &options[OPT_ATT_STATUS];

    zero_one_widgets[ZERO_ONE19] =
	lookup_widget(opt_window, "check_att_age");
    zero_one_options[ZERO_ONE19] = &options[OPT_ATT_AGE];

    zero_one_widgets[ZERO_ONE20] =
	lookup_widget(opt_window, "check_att_etal");
    zero_one_options[ZERO_ONE20] = &options[OPT_ATT_ETAL];

    zero_one_widgets[ZERO_ONE21] =
	lookup_widget(opt_window, "check_att_booked");
    zero_one_options[ZERO_ONE21] = &options[OPT_ATT_BOOKED];

    zero_one_widgets[ZERO_ONE22] =
	lookup_widget(opt_window, "check_att_value");
    zero_one_options[ZERO_ONE22] = &options[OPT_ATT_VALUE];

    zero_one_widgets[ZERO_ONE23] =
	lookup_widget(opt_window, "check_att_wage");
    zero_one_options[ZERO_ONE23] = &options[OPT_ATT_WAGE];

    zero_one_widgets[ZERO_ONE27] =
	lookup_widget(opt_window, "check_att_team");
    zero_one_options[ZERO_ONE27] = &options[OPT_ATT_TEAM];

    zero_one_widgets[ZERO_ONE28] =
	lookup_widget(opt_window, "check_att_league");
    zero_one_options[ZERO_ONE28] = &options[OPT_ATT_LEAGUE];

    zero_one_widgets[ZERO_ONE30] =
	lookup_widget(opt_window, "check_live_tendency");
    zero_one_options[ZERO_ONE30] = &options[OPT_LIVE_TENDENCY];

    zero_one_widgets[ZERO_ONE31] =
	lookup_widget(opt_window, "check_att2_name");
    zero_one_options[ZERO_ONE31] = &options[OPT_ATT2_NAME];

    zero_one_widgets[ZERO_ONE32] =
	lookup_widget(opt_window, "check_att2_cpos");
    zero_one_options[ZERO_ONE32] = &options[OPT_ATT2_CPOS];

    zero_one_widgets[ZERO_ONE33] =
	lookup_widget(opt_window, "check_att2_pos");
    zero_one_options[ZERO_ONE33] = &options[OPT_ATT2_POS];

    zero_one_widgets[ZERO_ONE34] =
	lookup_widget(opt_window, "check_att2_cskill");
    zero_one_options[ZERO_ONE34] = &options[OPT_ATT2_CSKILL];

    zero_one_widgets[ZERO_ONE35] =
	lookup_widget(opt_window, "check_att2_skill");
    zero_one_options[ZERO_ONE35] = &options[OPT_ATT2_SKILL];

    zero_one_widgets[ZERO_ONE36] =
	lookup_widget(opt_window, "check_att2_fit");
    zero_one_options[ZERO_ONE36] = &options[OPT_ATT2_FIT];

    zero_one_widgets[ZERO_ONE37] =
	lookup_widget(opt_window, "check_att2_games");
    zero_one_options[ZERO_ONE37] = &options[OPT_ATT2_GAMES];

    zero_one_widgets[ZERO_ONE38] =
	lookup_widget(opt_window, "check_att2_goals");
    zero_one_options[ZERO_ONE38] = &options[OPT_ATT2_GOALS];

    zero_one_widgets[ZERO_ONE39] =
	lookup_widget(opt_window, "check_att2_status");
    zero_one_options[ZERO_ONE39] = &options[OPT_ATT2_STATUS];

    zero_one_widgets[ZERO_ONE40] =
	lookup_widget(opt_window, "check_att2_age");
    zero_one_options[ZERO_ONE40] = &options[OPT_ATT2_AGE];

    zero_one_widgets[ZERO_ONE41] =
	lookup_widget(opt_window, "check_att2_etal");
    zero_one_options[ZERO_ONE41] = &options[OPT_ATT2_ETAL];

    zero_one_widgets[ZERO_ONE42] =
	lookup_widget(opt_window, "check_att2_booked");
    zero_one_options[ZERO_ONE42] = &options[OPT_ATT2_BOOKED];

    zero_one_widgets[ZERO_ONE43] =
	lookup_widget(opt_window, "check_att2_value");
    zero_one_options[ZERO_ONE43] = &options[OPT_ATT2_VALUE];

    zero_one_widgets[ZERO_ONE44] =
	lookup_widget(opt_window, "check_att2_wage");
    zero_one_options[ZERO_ONE44] = &options[OPT_ATT2_WAGE];

    zero_one_widgets[ZERO_ONE45] =
	lookup_widget(opt_window, "check_att2_team");
    zero_one_options[ZERO_ONE45] = &options[OPT_ATT2_TEAM];

    zero_one_widgets[ZERO_ONE46] =
	lookup_widget(opt_window, "check_att2_league");
    zero_one_options[ZERO_ONE46] = &options[OPT_ATT2_LEAGUE];

    zero_one_widgets[ZERO_ONE47] =
	lookup_widget(opt_window, "checkbutton_delete_team");
    zero_one_options[ZERO_ONE47] = &options[OPT_HISTORY_TEAM_DELETE];

    zero_one_widgets[ZERO_ONE48] =
	lookup_widget(opt_window, "checkbutton_delete_player");
    zero_one_options[ZERO_ONE48] = &options[OPT_HISTORY_PLAYER_DELETE];

    radio_buttons[0] = 
	lookup_widget(opt_window, "radio_mess1");
    radio_buttons[1] = 
	lookup_widget(opt_window, "radio_mess2");
    radio_buttons[2] = 
	lookup_widget(opt_window, "radio_mess3");
    radio_buttons[3] = 
	lookup_widget(opt_window, "radio_show1");
    radio_buttons[4] = 
	lookup_widget(opt_window, "radio_show2");

    radiobutton_sort[0] =
	lookup_widget(opt_window, "radiobutton_sort_pos");
    radiobutton_sort[1] =
	lookup_widget(opt_window, "radiobutton_sort_skill");
    radiobutton_sort[2] =
	lookup_widget(opt_window, "radiobutton_sort_age");
    radiobutton_sort[3] =
	lookup_widget(opt_window, "radiobutton_sort_etal");
    radiobutton_sort[4] =
	lookup_widget(opt_window, "radiobutton_sort_league");

    spinbuttons_history[0] =
	lookup_widget(opt_window, "spinbutton_interval_team");
    spinbuttons_history[1] =
	lookup_widget(opt_window, "spinbutton_interval_player");
    spinbuttons_history[2] =
	lookup_widget(opt_window, "spinbutton_max_team");
    spinbuttons_history[3] =
	lookup_widget(opt_window, "spinbutton_max_player");
}

/* set up the content of the options window: 
   if read is 1, we read the global options variables
   and set the checkbuttons etc. accordingly;
   else we read the states of the buttons and 
   write the variables accordingly */
void
set_up_options_window(GtkWidget *opt_window, gint read)
{
    gint i;
    gint *zero_one_options[ZERO_ONE_END];
    GtkWidget *zero_one_widgets[ZERO_ONE_END];
    GtkWidget *radio_buttons[5];
    GtkWidget *hbox52 = lookup_widget(opt_window, "hbox52");
    GtkWidget *checkbutton_notify =
	lookup_widget(opt_window, "checkbutton_notify");
    GtkWidget *spin_live_duration =
	lookup_widget(opt_window, "spin_live_duration");
    GtkWidget *entry_team_name =
	lookup_widget(opt_window, "entry_team_name");
    GtkWidget *checkbutton_autosave =
	lookup_widget(opt_window, "checkbutton_autosave");
    GtkWidget *spinbutton_autosave = 
	lookup_widget(opt_window, "spinbutton_autosave");
    GtkWidget *checkbutton_sort_transfers = 
	lookup_widget(opt_window, "checkbutton_sort_transfers");
    GtkWidget *checkbutton_sort_asc =
	lookup_widget(opt_window, "checkbutton_sort_asc");
    GtkWidget *checkbutton_save_conf = 
	lookup_widget(opt_window, "checkbutton_save_conf");
    GtkWidget *spinbuttons_history[4];
    GtkWidget *radiobutton_sort[SORT_END - 1];
   
    get_option_widgets(opt_window, zero_one_options,
		       zero_one_widgets, radio_buttons,
		       radiobutton_sort, spinbuttons_history);

    set_up_transfer_notify(opt_window, read);

    if(read == 1)
    {
	for(i=0;i<ZERO_ONE_END;i++)
	    gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(zero_one_widgets[i]),
		(*(zero_one_options[i]) == 1));

	for(i=0;i<3;i++)
	    gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(radio_buttons[i]),
		(options[OPT_MESS] == i + 1));
	
	for(i=3;i<5;i++)	    
		gtk_toggle_button_set_active(
		    GTK_TOGGLE_BUTTON(radio_buttons[i]),
		    (options[OPT_SHOW_MY_GAMES] == i - 3));

	for(i=0;i<4;i++)
	    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttons_history[i]),
				      (gfloat)options[OPT_HISTORY_TEAM_INTERVAL + i]);
	
	gtk_spin_button_set_value(
	    GTK_SPIN_BUTTON(spin_live_duration),
	    (gfloat)options[OPT_LIVE_DURATION]);
	if(options[OPT_SHOW_LIVE] == 0)
	    gtk_widget_set_sensitive(spin_live_duration->parent->parent,
				     FALSE);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_notify),
				     options[OPT_NOTIFY]);
	gtk_widget_set_sensitive(hbox52, options[OPT_NOTIFY]);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_autosave),
				     (options[OPT_AUTOSAVE] > 0));
	gtk_widget_set_sensitive(spinbutton_autosave,
				 (options[OPT_AUTOSAVE] > 0));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbutton_autosave),
				  (gfloat)abs(options[OPT_AUTOSAVE]));

	gtk_entry_set_text(GTK_ENTRY(entry_team_name),
			   teams[my_team].name);

	gtk_widget_set_sensitive(radiobutton_sort[0]->parent,
				 (options[OPT_SORT_TRANSFERS] > 0));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_sort_transfers),
				     (options[OPT_SORT_TRANSFERS] > 0));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_sort_asc),
				     (abs(options[OPT_SORT_TRANSFERS]) > 10));

	for(i=0;i<SORT_END - 1;i++)
	    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton_sort[i]),
					 (abs(options[OPT_SORT_TRANSFERS]) % 10 == i + 1));

	return;
    }    

    /* write option variables according to option window content
       when it's been closed with the 'ok' button */
    for(i=0;i<ZERO_ONE_END;i++)
	if(gtk_toggle_button_get_active(
	       GTK_TOGGLE_BUTTON(zero_one_widgets[i])))
	    *(zero_one_options[i]) = 1;
	else
	    *(zero_one_options[i]) = 0;
    
    for(i=0;i<3;i++)
	if(gtk_toggle_button_get_active(
	       GTK_TOGGLE_BUTTON(radio_buttons[i])))
	    options[OPT_MESS] = i + 1;

    for(i=3;i<5;i++)
	if(gtk_toggle_button_get_active(
	       GTK_TOGGLE_BUTTON(radio_buttons[i])))
	    options[OPT_SHOW_MY_GAMES] = i - 3;
    
    for(i=0;i<4;i++)
	options[OPT_HISTORY_TEAM_INTERVAL + i] =
	    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbuttons_history[i]));

    options[OPT_LIVE_DURATION] =
	gtk_spin_button_get_value_as_int(
	    GTK_SPIN_BUTTON(spin_live_duration));
	
    strcpy(teams[my_team].name, gtk_entry_get_text(GTK_ENTRY(entry_team_name)));

    options[OPT_NOTIFY] = 
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_notify));

    options[OPT_AUTOSAVE] = 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinbutton_autosave)) *
	(-1 + 2 * (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_autosave))));
    if(counters[COUNT_AUTOSAVE] > options[OPT_AUTOSAVE])
	counters[COUNT_AUTOSAVE] = options[OPT_AUTOSAVE];

    for(i=0;i<SORT_END - 1;i++)
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton_sort[i])))
	{
	    options[OPT_SORT_TRANSFERS] =
		(i + 1 + gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_sort_asc)) * 10) *
		(-1 + 2 * gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_sort_transfers)));
	    if(options[OPT_SORT_TRANSFERS] > 0)
		sort_transferlist(options[OPT_SORT_TRANSFERS]);
	}

    /* if the scout's not good enough, the age isn't shown in the transfers
       window, so we can't sort by that */
    if(options[OPT_SORT_TRANSFERS] % 10 == SORT_AGE &&
       scout > 2)
    {
	options[OPT_SORT_TRANSFERS] *= -1;
	show_popup_window(_("Your scout's not good enough, he doesn't know the age of the players on the transfer list. Sorting the transfer list is switched off. "), NULL);
    }

    /* save options to the conf file if
       necessary */
    if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_save_conf)))
	save_conf_file();
}

enum
{
    NOTIFY_CHECK_AGE = 0,
    NOTIFY_CHECK_SKILL,
    NOTIFY_CHECK_ETAL,
    NOTIFY_CHECK_VALUE,
    NOTIFY_CHECK_POS,
    NOTIFY_CHECK_LEAGUE,
    NOTIFY_CHECK_POS0,
    NOTIFY_CHECK_POS1,
    NOTIFY_CHECK_POS2,
    NOTIFY_CHECK_POS3,
    NOTIFY_CHECK_LEAGUE1,
    NOTIFY_CHECK_LEAGUE2,
    NOTIFY_CHECK_LEAGUE3,
    NOTIFY_CHECK_LEAGUE4,
    NOTIFY_CHECK_LEAGUE5,
    NOTIFY_CHECK_LEAGUE6,
    NOTIFY_CHECK_END
};

enum
{
    NOTIFY_SPIN_AGE1 = 0,
    NOTIFY_SPIN_AGE2,
    NOTIFY_SPIN_SKILL1,
    NOTIFY_SPIN_SKILL2,
    NOTIFY_SPIN_ETAL1,
    NOTIFY_SPIN_ETAL2,
    NOTIFY_SPIN_VALUE,
    NOTIFY_SPIN_END
};

void
get_check_widgets(GtkWidget *opt_window, GtkWidget **notify_check)
{
    notify_check[NOTIFY_CHECK_AGE] =
	lookup_widget(opt_window, "checkbutton_notify_age");
    notify_check[NOTIFY_CHECK_SKILL] =
	lookup_widget(opt_window, "checkbutton_notify_skill");
    notify_check[NOTIFY_CHECK_ETAL] =
	lookup_widget(opt_window, "checkbutton_notify_etal");
    notify_check[NOTIFY_CHECK_VALUE] =
	lookup_widget(opt_window, "checkbutton_notify_value");
    notify_check[NOTIFY_CHECK_POS] =
	lookup_widget(opt_window, "checkbutton_notify_positions");
    notify_check[NOTIFY_CHECK_LEAGUE] =
	lookup_widget(opt_window, "checkbutton_notify_league");

    notify_check[NOTIFY_CHECK_POS0] =
	lookup_widget(opt_window, "checkbutton_notify_pos0");
    notify_check[NOTIFY_CHECK_POS1] =
	lookup_widget(opt_window, "checkbutton_notify_pos1");
    notify_check[NOTIFY_CHECK_POS2] =
	lookup_widget(opt_window, "checkbutton_notify_pos2");
    notify_check[NOTIFY_CHECK_POS3] =
	lookup_widget(opt_window, "checkbutton_notify_pos3");

    notify_check[NOTIFY_CHECK_LEAGUE1] = 
	lookup_widget(opt_window, "checkbutton_notify_league1");
    notify_check[NOTIFY_CHECK_LEAGUE2] = 
	lookup_widget(opt_window, "checkbutton_notify_league2");
    notify_check[NOTIFY_CHECK_LEAGUE3] = 
	lookup_widget(opt_window, "checkbutton_notify_league3");
    notify_check[NOTIFY_CHECK_LEAGUE4] = 
	lookup_widget(opt_window, "checkbutton_notify_league4");
    notify_check[NOTIFY_CHECK_LEAGUE4] = 
	lookup_widget(opt_window, "checkbutton_notify_league4");
    notify_check[NOTIFY_CHECK_LEAGUE5] = 
	lookup_widget(opt_window, "checkbutton_notify_league5");
    notify_check[NOTIFY_CHECK_LEAGUE6] = 
	lookup_widget(opt_window, "checkbutton_notify_league6");
}

void
get_spin_widgets(GtkWidget *opt_window, 
		      GtkWidget **notify_spin)
{
    notify_spin[NOTIFY_SPIN_SKILL1] = 
	lookup_widget(opt_window, "spinbutton_notify_skill1");
    notify_spin[NOTIFY_SPIN_SKILL2] = 
	lookup_widget(opt_window, "spinbutton_notify_skill2");
    notify_spin[NOTIFY_SPIN_ETAL1] = 
	lookup_widget(opt_window, "spinbutton_notify_etal1");
    notify_spin[NOTIFY_SPIN_ETAL2] = 
	lookup_widget(opt_window, "spinbutton_notify_etal2");
    notify_spin[NOTIFY_SPIN_AGE1] = 
	lookup_widget(opt_window, "spinbutton_notify_age1");
    notify_spin[NOTIFY_SPIN_AGE2] = 
	lookup_widget(opt_window, "spinbutton_notify_age2");
    notify_spin[NOTIFY_SPIN_VALUE] = 
	lookup_widget(opt_window, "spinbutton_notify_value");
}

/* set up all the criteria for transfer list notification */
void
set_up_transfer_notify(GtkWidget *opt_window, gint read)
{
    gint i;
    GtkWidget *notify_check[NOTIFY_CHECK_END];
    GtkWidget *notify_spin[NOTIFY_SPIN_END];

    get_check_widgets(opt_window, notify_check);
    get_spin_widgets(opt_window, notify_spin);

    if(read == 1)
    {
	for(i = OPT_NOTIFY_AGE; i <= OPT_NOTIFY_LEAGUE; i++)
	    gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(notify_check[i - OPT_NOTIFY_AGE]), options[i] > 0);

	for(i = OPT_NOTIFY_POS0; i <= OPT_NOTIFY_LEAGUE6; i++)
	    gtk_toggle_button_set_active(
		GTK_TOGGLE_BUTTON(notify_check[NOTIFY_CHECK_POS0 + i - OPT_NOTIFY_POS0]),
		options[i]);
	
	for(i=0;i<3;i++)
	{
	    gtk_spin_button_set_value(
		GTK_SPIN_BUTTON(notify_spin[NOTIFY_SPIN_AGE1 + i * 2]),
		(gfloat)get_place(abs(options[OPT_NOTIFY_AGE + i]), 12) / (10 - 9 * (i == 0)));
	    gtk_spin_button_set_value(
		GTK_SPIN_BUTTON(notify_spin[NOTIFY_SPIN_AGE2 + i * 2]),
		(gfloat)get_place(abs(options[OPT_NOTIFY_AGE + i]), 22) / (10 - 9 * (i == 0)));
	}

	gtk_spin_button_set_value(
	    GTK_SPIN_BUTTON(notify_spin[NOTIFY_SPIN_VALUE]),
	    abs(options[OPT_NOTIFY_VALUE]));
	
	return;
    }
    
    for(i = OPT_NOTIFY_POS0; i <= OPT_NOTIFY_LEAGUE6; i++)
	options[i] = gtk_toggle_button_get_active(
	    GTK_TOGGLE_BUTTON(notify_check[NOTIFY_CHECK_POS0 + i - OPT_NOTIFY_POS0]));
	
    for(i=0;i<3;i++)
	options[OPT_NOTIFY_AGE + i] = 
	    ((gint)rint(gtk_spin_button_get_value(
			   GTK_SPIN_BUTTON(notify_spin[NOTIFY_SPIN_AGE1 + i * 2])) 
		       * (10 - 9 * (i == 0))) * 100 +
	    (gint)rint(gtk_spin_button_get_value(
			   GTK_SPIN_BUTTON(notify_spin[NOTIFY_SPIN_AGE2 + i * 2])) 
		       * (10 - 9 * (i == 0)))) *
	    (-1 + 2 * gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(notify_check[NOTIFY_CHECK_AGE + i])));
    
    options[OPT_NOTIFY_VALUE] = 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(notify_spin[NOTIFY_SPIN_VALUE])) * 
	(-1 + 2 * gtk_toggle_button_get_active(
	    GTK_TOGGLE_BUTTON(notify_check[NOTIFY_CHECK_VALUE])));

    for(i=0;i<2;i++)
	options[OPT_NOTIFY_POS + i] = 
	    gtk_toggle_button_get_active(
		GTK_TOGGLE_BUTTON(notify_check[NOTIFY_CHECK_POS + i]));
}
