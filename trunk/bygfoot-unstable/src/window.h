#ifndef WINDOW_H
#define WINDOW_H

#include "bygfoot.h"

/* create the live window and return the pointer */
GtkWidget*
return_live_window(void);

/* create the main window and return the pointer */
GtkWidget*
return_main_window(void);

/* create options window */
void
show_opt_window(void);

/* create and show notification options window */
void
show_notify_window(void);

void
get_fire_text(GtkWidget *popup_label, GtkWidget **radiobuttons,
	      gint *status);

/* show a popup window with 'text' and adjust the
   window buttons according to the 'status' integers */
void
show_popup_window(gchar *text, gint *status);

/* pop up the help window */
void
show_help(gint customization);

/* show the file selection window */
void
show_file_selection(gint save);

/* show the team selection window at the beginning of the game */
void
show_team_selection(void);

void
show_font_sel_window(void);

void
show_stadium_window(void);

/* start the team editor */
void
start_editor(void);

/* show a window with a job offer and some info on the possible new team */
void
show_job_offer_window(gchar text[][SMALL]);

/* show the frontend for the update script */
void
start_update(void);

/* show graph window */
GtkWidget*
show_graph_window(void);

#endif
