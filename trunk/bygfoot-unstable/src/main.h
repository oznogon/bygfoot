#ifndef MAIN_H
#define MAIN_H

#include "bygfoot.h"

/* set some variables that are used but not saved
   in the game */
void
set_variables(void);

void
bygfoot_init(gint argc, gchar *argv[]);

gint
main (gint argc, gchar *argv[]);

#endif
