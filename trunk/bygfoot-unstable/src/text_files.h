#ifndef TEXT_FILES_H
#define TEXT_FILES_H

#include "bygfoot.h"

/* read the file until the next line that's not a comment or
   a blank line and copy the line into buf; or, if find_text is
   not NULL, read the file until 'find_text' is found */
gboolean
get_next_line(FILE *fil, gchar *buf, gchar *find_text);

/* write the names of the options that will
   identify them in the conf file into the array */
void
write_opt_names(gchar opt_names[][50]);

/* write the game options to the file
   $HOME/.bygfoot/bygfoot.conf */
void
save_conf_file(void);

/* read the bygfoot configurations file */
void
read_conf_file(void);

/* try to make a .bygfoot directory in the user's home directory
   and copy the support files there if possible */
void
check_home_dir(void);

/* check for the files with team and player names
   and help file */
void
check_files(void);

/* write team or player names from 'filename'
   to the array 'names'; both files are read until a '1000' appears
   in a line by itself */
void
get_names(gchar *filename, gchar names[][50]);

/* return filename corresponding to a number defined in defs.h/enums.h */
void
text_file_number_to_char(gint number, gchar *filename, gboolean full_path);

void
read_structures(FILE *fil, gint team_id, gint *structure2);

void
read_player(FILE *fil, gint team_id, gint read, gint player_number, gint *birth_dates);

/* fill in the players of a team from the teams file */
void
read_team(FILE *fil, gint team_id, gint *structure2, gint read, gint *birth_dates);

/* read the teams file which specifies (perhaps);
   some data about the players of a team;
   'read' tells us which parts to read: either
   names and values (read=2);, only names (1); or
   nothing(0);; if 'team_name' isn't NULL, read that
   team to team number 114 */
void
read_teams_file(gint read, const gchar *team_name, gint *structure2, 
		gint *birth_dates);

#endif
