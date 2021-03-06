#include "file.h"
#include "options_callbacks.h"
#include "options_interface.h"
#include "option_gui.h"
#include "support.h"
#include "user.h"
#include "variables.h"
#include "window.h"

void
on_button_options_ok_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
    gboolean save_global =
	gtk_toggle_button_get_active(
	    GTK_TOGGLE_BUTTON(lookup_widget(window.options, "checkbutton_save_global"))),
	save_user = 
	gtk_toggle_button_get_active(
	    GTK_TOGGLE_BUTTON(lookup_widget(window.options, "checkbutton_save_user")));
    const gchar *conf_dir = file_get_first_support_dir();
    gchar buf[SMALL];

    option_gui_write_options();

    if(save_global)
    {
	sprintf(buf, "%s/bygfoot.conf", conf_dir);
	file_save_opt_file(buf, &options);
    }

    if(save_user)
    {
	if(strcmp(current_user.name->str, "NONAME") == 0)
	    sprintf(buf, "%s/bygfoot_user.conf", conf_dir);
	else
	    sprintf(buf, "%s/bygfoot_%s.conf", conf_dir, current_user.name->str);

	file_save_opt_file(buf, &current_user.options);
    }

    window_destroy(&window.options, TRUE);
}


void
on_button_options_cancel_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
    window_destroy(&window.options, TRUE);
}


void
on_button_font_name_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
    window_create(WINDOW_FONT_SEL);
}


void
on_button_reload_constants_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
    const gchar *constants_file =
	gtk_entry_get_text(GTK_ENTRY(lookup_widget(window.options, "entry_constants_file")));

    file_load_opt_file(constants_file, &constants);
}


gboolean
on_checkbutton_save_global_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{
    if(event->button == 3)
    {
	gchar *conf_file = file_find_support_file("bygfoot.conf", TRUE);
	
	file_load_opt_file(conf_file, &options);
	g_free(conf_file);

	option_gui_set_up_window();
	return TRUE;
    }

    return FALSE;
}


gboolean
on_checkbutton_save_user_button_press_event
                                        (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data)
{

    if(event->button == 3)
    {
	file_load_user_conf_file(&current_user);
	option_gui_set_up_window();
	return TRUE;
    }

    return FALSE;
}

