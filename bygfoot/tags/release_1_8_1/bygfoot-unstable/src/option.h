#ifndef OPTION_H
#define OPTION_H

#include "bygfoot.h"
#include "variables.h"

void
get_option_widgets(GtkWidget *opt_window,
		   gint **zero_one_options,
		   GtkWidget **zero_one_widgets,
		   GtkWidget **radio_buttons,
		   GtkWidget **radiobutton_sort,
		   GtkWidget **spinbuttons_history);

/* set up the content of the options window: 
   if read is 1, we read the global options variables
   and set the checkbuttons etc. accordingly;
   else we read the states of the buttons and 
   write the variables accordingly */
void
set_up_options_window(GtkWidget *opt_window, gint read);

void
get_check_widgets(GtkWidget *notify_window, GtkWidget **notify_check);

void
get_spin_widgets(GtkWidget *notify_window, 
		      GtkWidget **notify_spin);

/* set up all the criterie for transfer list notification */
void
set_up_transfer_notify(GtkWidget *opt_window, gint read);


#endif
