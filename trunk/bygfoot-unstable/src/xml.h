#ifndef XML_WRITE_H
#define XML_WRITE_H

#include <gtk/gtk.h>
#include "bygfoot.h"
#include "files.h"
#include "gui.h"
#include "history.h"
#include "load_save.h"
#include "misc.h"
#include "variables.h"
#include "xml_fixtures.h"
#include "xml_general.h"
#include "xml_teams.h"

#define XML_FILE_EXT_GENERAL "general"
#define XML_FILE_EXT_TEAMS "teams"
#define XML_FILE_EXT_FIXTURES "fixtures"

#define PROGRESS_MAX 200.0
#define PROGRESS_TEAM 1.0
#define PROGRESS_FIX 10.0
#define PROGRESS_GENERAL 7.0
#define PROGRESS_COMPRESS 3.0

#define INDENT0 ""
#define INDENT1 "\t"
#define INDENT2 "\t\t"
#define INDENT3 "\t\t\t"
#define INDENT4 "\t\t\t\t"
#define INDENT5 "\t\t\t\t\t"
#define INDENT6 "\t\t\t\t\t\t"

gfloat progress;

void
xml_write_save(gchar * file_name);
void
xml_write_init(FILE *xml_file, gint tag);
void 
xml_write_end(FILE * xml_file, gint tag);

void
xml_read_save(gchar *file_name);

void
xml_remove_files(gchar *file_name);

gchar*
xml_file_name_from_num(gint number);

void
xml_compress_files(gchar *file_name, gboolean decompress);

gint
xml_get_tag_from_name(const gchar *element_name);

#endif
