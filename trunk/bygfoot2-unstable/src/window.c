#include "file.h"
#include "finance.h"
#include "free.h"
#include "game_gui.h"
#include "gui.h"
#include "interface.h"
#include "misc_interface.h"
#include "misc2_interface.h"
#include "option.h"
#include "option_gui.h"
#include "options_interface.h"
#include "support.h"
#include "user.h"
#include "window.h"

/**
   Show the country selection window. All files with prefix
   'country_' from $HOME/.bygfoot/definitions are appended to a combo box.
*/
void
window_show_startup(void)
{
    GtkWidget *window_startup =
	window_create(WINDOW_STARTUP);
    GtkWidget *combo_country =
	lookup_widget(window_startup, "combo_country");
    gchar country_dir[SMALL];
    GPtrArray *dir_contents = NULL;
    GList *combo_strings = NULL;
    gint i;
    
    file_get_definitions_dir(country_dir);

    dir_contents = file_dir_get_contents((const gchar*)country_dir, "country_");

    for(i=0;i<dir_contents->len;i++)
	combo_strings = g_list_append(combo_strings,
				      ((GString*)g_ptr_array_index(dir_contents, i))->str);

    gtk_combo_set_popdown_strings(GTK_COMBO(combo_country), combo_strings);

    free_g_string_array(&dir_contents);
}


/**  Show the options window. */
void
window_show_options(void)
{
    window_create(WINDOW_OPTIONS);

    option_gui_set_up_window();
}


/** Show the digits window with the labels and values set 
    according to the arguments. */
void
window_show_digits(gchar *text_main, gchar* text1, gint value1, gchar* text2, gint value2)
{
    GtkLabel *label_main, *label_1, *label_2;
    GtkSpinButton *spinbutton1, *spinbutton2;

    window_create(WINDOW_DIGITS);

    label_main = GTK_LABEL(lookup_widget(window.digits, "label_main"));
    label_1 = GTK_LABEL(lookup_widget(window.digits, "label_1"));
    label_2 = GTK_LABEL(lookup_widget(window.digits, "label_2"));

    spinbutton1 = GTK_SPIN_BUTTON(lookup_widget(window.digits, "spinbutton1"));
    spinbutton2 = GTK_SPIN_BUTTON(lookup_widget(window.digits, "spinbutton2"));

    if(stat0 == STATUS_GET_LOAN ||
       stat0 == STATUS_PAY_LOAN)
	gtk_spin_button_set_range(spinbutton1, (gdouble)1, (gdouble)value1);

    gtk_spin_button_set_value(spinbutton1, (gdouble)value1);
    gtk_spin_button_set_value(spinbutton2, (gdouble)value2);

    gtk_label_set_text(label_main, text_main);

    if(text1 != NULL)
	gtk_label_set_text(label_1, text1);
    else
	gtk_widget_hide(GTK_WIDGET(label_1)->parent);

    if(text2 != NULL)
	gtk_label_set_text(label_2, text2);
    else
	gtk_widget_hide(GTK_WIDGET(label_2)->parent);

}

/** Show the stadium window for the current user. */
void
window_show_stadium(void)
{
    gchar buf[SMALL];
    const Team *tm = current_user.tm;
    GtkLabel *label_capacity,
	*label_costs_capacity, *label_costs_safety,
	*label_duration_capacity, *label_duration_safety,
	*label_stadium_status;
    GtkSpinButton *spinbutton_capacity, *spinbutton_safety;
    GtkProgressBar *progressbar_safety;

    window_create(WINDOW_STADIUM);

    label_capacity = GTK_LABEL(lookup_widget(window.stadium, "label_capacity"));
    label_costs_capacity = GTK_LABEL(lookup_widget(window.stadium, "label_costs_capacity"));
    label_costs_safety = GTK_LABEL(lookup_widget(window.stadium, "label_costs_safety"));
    label_duration_capacity = GTK_LABEL(lookup_widget(window.stadium, "label_duration_capacity"));
    label_duration_safety = GTK_LABEL(lookup_widget(window.stadium, "label_duration_safety"));
    label_stadium_status = GTK_LABEL(lookup_widget(window.stadium, "label_stadium_status"));
    
    spinbutton_capacity = GTK_SPIN_BUTTON(lookup_widget(window.stadium, "spinbutton_capacity"));
    spinbutton_safety = GTK_SPIN_BUTTON(lookup_widget(window.stadium, "spinbutton_safety"));

    progressbar_safety = GTK_PROGRESS_BAR(lookup_widget(window.stadium, "progressbar_safety"));

    gui_label_set_text_from_int(label_capacity, tm->stadium.capacity, FALSE);
    gui_label_set_text_from_int(label_costs_capacity, 
				(gint)rint(finance_wage_unit(tm) *
					   const_float("float_stadium_improvement_wage_unit_factor_seats")),
				FALSE);
    gui_label_set_text_from_int(label_costs_safety, 
				(gint)rint(finance_wage_unit(tm) *
					   (const_float("float_stadium_improvement_wage_unit_factor_safety") /
					    100)), FALSE);
    gui_label_set_text_from_int(label_duration_capacity, 1, FALSE);
    gui_label_set_text_from_int(label_duration_safety, 1, FALSE);

    gtk_spin_button_set_value(spinbutton_capacity, 
			      (gdouble)const_int("int_stadium_improvement_base_seats"));
    gtk_spin_button_set_value(spinbutton_safety, 
			      const_float("float_stadium_improvement_base_safety") * 100);

    gtk_progress_bar_set_fraction(progressbar_safety, tm->stadium.safety);

    sprintf(buf, "%d%%", (gint)rint(tm->stadium.safety * 100));
    gtk_progress_bar_set_text(progressbar_safety, buf);

    if(current_user.counters[COUNT_USER_STADIUM_CAPACITY] + 
       current_user.counters[COUNT_USER_STADIUM_SAFETY] != 0)
    {
	sprintf(buf, _("Improvement in progress.\n%d seats and %d%% safety still to be done.\nExpected finish: %d weeks."),
		current_user.counters[COUNT_USER_STADIUM_CAPACITY],
		current_user.counters[COUNT_USER_STADIUM_SAFETY],
		MAX(finance_get_stadium_improvement_duration(
			(gfloat)current_user.counters[COUNT_USER_STADIUM_CAPACITY], TRUE),
		    finance_get_stadium_improvement_duration(
			(gfloat)current_user.counters[COUNT_USER_STADIUM_SAFETY] / 100, FALSE)));
	gtk_label_set_text(label_stadium_status, buf);
    }
    else
	gtk_label_set_text(label_stadium_status, _("No improvements currently in progress."));
}

/** Show the window where the user can select between yes and no.
    @param text The text shown in the window.
    @param checkbutton Whether to show the checkbutton. */
void
window_show_yesno(gchar *text, gboolean checkbutton)
{
    window_create(WINDOW_YESNO);

    gtk_label_set_text(GTK_LABEL(lookup_widget(window.yesno, "label_yesno")), text);
    
    if(!checkbutton)
	gtk_widget_hide(lookup_widget(window.yesno, "checkbutton_yesno"));
}

/** Create and show a window. Which one depends on the argument.
    @param window_type An integer telling us which window to
    create.
    @param count_popups Whether this window adds to the popup
    counter that determines when the main window gets (in)sensitive.
    @return The pointer to the new window.
    @see #Windows */
GtkWidget*
window_create(gint window_type)
{
    gint old_popups_active = popups_active;
    gchar buf[SMALL];
    GtkWidget *wind = NULL;
    
    sprintf(buf, "Bygfoot Football Manager %s", VERS);

    switch(window_type)
    {
	default:
	    g_warning("window_create: unknown window type %d\n", window_type);
	    break;
	case WINDOW_MAIN:
	    if(window.main == NULL)
	    {
		window.main = create_main_window();
		wind = window.main;
		game_gui_print_message("Welcome to Bygfoot "VERS);
	    }
	    else
		wind = window.main;
	    break;
	case WINDOW_STARTUP:
	    if(window.startup != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.startup = create_window_startup();
	    }
	    wind = window.startup;
	    break;
	case WINDOW_LIVE:
	    if(window.live != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.live = create_window_live();
	    }
	    strcpy(buf, "Bygfoot Live game");
	    wind = window.live;
	    gtk_spin_button_set_value(
		GTK_SPIN_BUTTON(lookup_widget(wind, "spinbutton_speed")),
		(gfloat)option_int("int_opt_user_live_game_speed", &usr(stat2).options));
	    break;
	case WINDOW_STARTUP_USERS:
	    if(window.startup_users != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.startup_users = create_window_startup_users();
	    }
	    strcpy(buf, "Users");
	    wind = window.startup_users;
	    break;
	case WINDOW_WARNING:
	    if(window.warning != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
		window.warning = create_window_warning();
	    wind = window.warning;
	    strcpy(buf, "Erm...");
	    break;
	case WINDOW_PROGRESS:
	    if(window.progress != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
		window.progress = create_window_progress();
	    wind = window.progress;
	    strcpy(buf, "");
	    break;
	case WINDOW_DIGITS:
	    if(window.digits != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.digits = create_window_digits();
	    }
	    wind = window.digits;
	    strcpy(buf, _("Numbers..."));
	    break;
	case WINDOW_STADIUM:
	    if(window.stadium != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.stadium = create_window_stadium();
	    }
	    wind = window.stadium;
	    strcpy(buf, _("Your stadium"));
	    break;
	case WINDOW_JOB_OFFER:
	    if(window.job_offer != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.job_offer = create_window_job_offer();
	    }
	    wind = window.job_offer;
	    strcpy(buf, _("Job offer"));
	    break;
	case WINDOW_YESNO:
	    if(window.yesno != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.yesno = create_window_yesno();
	    }
	    wind = window.yesno;
	    strcpy(buf, "???");
	    break;
	case WINDOW_OPTIONS:
	    if(window.options != NULL)
		g_warning("window_create: called on already existing window\n");
	    else
	    {
		popups_active++;
		window.options = create_window_options();
	    }
	    wind = window.options;
	    strcpy(buf, "Options");
	    break;
    }

    gtk_window_set_title(GTK_WINDOW(wind), buf);
    gtk_widget_show(wind);

    if(popups_active != old_popups_active &&
       window.main != NULL)
	gtk_widget_set_sensitive(window.main, FALSE);

    return wind;
}

/** Destroy a window widget and set the popups and
    main window sensitivity correctly. 
    @param window The window we destroy.
    @param count_popups Whether this window adds to the popup
    counter that determines when the main window gets (in)sensitive. */
void
window_destroy(GtkWidget **wind, gboolean count_popups)
{
    if(*wind == NULL)
	return;

    if(*wind != window.main && count_popups)
    {
	popups_active--;

	if(popups_active == 0 && window.main != NULL)
	    gtk_widget_set_sensitive(window.main, TRUE);
    }

    gtk_widget_destroy(*wind);

    *wind = NULL;
}
