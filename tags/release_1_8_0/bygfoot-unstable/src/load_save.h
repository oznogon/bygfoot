#ifndef LOAD_SAVE_H
#define LOAD_SAVE_H

#include "bygfoot.h"
#include "variables.h"
#include "misc.h"
#include "xml.h"

gboolean
find_save_file(gchar *file_name);

void
save_game(gchar *file_name);

gboolean
load_game(gchar *file_name);

#endif
