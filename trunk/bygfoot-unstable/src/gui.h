#ifndef GUI_H
#define GUI_H

#include "bygfoot.h"

/* add 'directory' plus subdirectories recursively to the list
   we browse when looking for files (e.g. pixmaps); */
void
add_support_directory_recursive                   (const gchar     *directory);

/* add 'directory' both to our directory list and
   GLADE's list */
void
add_support_directory(const gchar *directory);

gchar*
find_support_file                       (const gchar     *filename);

/* keep track whether the game is saved or not */
void
set_save(gboolean status);

/* get an integer from a label */
gint
get_int_from_label(GtkLabel *label);

/* set or append 'text' into 'label' */
void
label_set_text(GtkLabel *label, gchar *text, gint append);

/* set or append 'number' as text into 'label' */
void
label_set_text_from_int(GtkLabel *label, gint number, gint append);

/* set or append 'number' as text into 'label' */
void
label_set_text_from_float(GtkLabel *label, gfloat number,
			       gint append, gint precision);

/* change the 'popups_active' variable. this determines whether
   the main window is set sensitive or not */
void
change_popups_active(gint difference);

/* get the back- and foreground colors
   of a table row depending on the rank
   it represents */
void
get_table_colors(gint team_id,
		      gchar *background_color,
		      gchar *foreground_color);

/* print a message into the message window */
void
print_message(gchar *text);

/* print a number into an entry field */
void
entry_set_text_from_int(GtkEntry *entry, gint number);

/* return the entry content as an integer */
gint
entry_get_int(GtkEntry *entry);

/* set the week number, season number, team name, team league
   and team rank labels */
void
set_header(void);

/* adjust the widget properties in the main window
   (mainly the buttons); depending on the window status
   (i.e. in which sub-menu the human player's in); */
void
set_buttons(void);

/* show the main window with the human player's team in the
   player list */
void
initialize_main_window(void);

#endif
