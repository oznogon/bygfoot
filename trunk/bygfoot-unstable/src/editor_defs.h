#ifndef EDITOR_DEFS_H
#define EDITOR_DEFS_H

#include "bygfoot.h"

/* in editor mode we don't need some globale variables
   but need others so we rename them */
#define structure2 save_status
#define selected_player notify_status[0]
#define birth_dates rank

#endif
