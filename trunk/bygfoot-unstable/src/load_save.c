/*********************************************************
 * Functions for loading and saving the game             *
 *********************************************************/

#include "load_save.h"

/* try to find out which file the user wants to load
   if the input isn't correct (e.g. path omitted) */
gboolean
find_save_file(gchar *file_name)
{
    gint i, j;
    gchar buf[SMALL];
    gchar *pwd = g_get_current_dir();
    gchar paths[2][SMALL];
    gchar *endings[2] = {"", ".zip"};

    if(g_file_test(file_name, G_FILE_TEST_EXISTS))
    {
	g_string_printf(save_file, "%s", file_name);
	return TRUE;
    }

    sprintf(paths[0], "%s/.bygfoot/saves/%s", g_get_home_dir(), file_name);
    sprintf(paths[1], "%s/%s", pwd, file_name);
    g_free(pwd);

    for(j=0;j<2;j++)
	for(i=0;i<2;i++)
	{
	    sprintf(buf, "%s%s", paths[j], endings[i]);

	    if(g_file_test(buf, G_FILE_TEST_EXISTS))
	    {
		strcpy(file_name, buf);
		g_string_printf(save_file, "%s", file_name);
		return TRUE;
	    }
	}

    return FALSE;
}

void
save_game(gchar *file_name)
{
    gchar local_file[SMALL];

    strcpy(local_file, file_name);

    if(g_str_has_suffix(file_name, ".zip"))
	truncate_string(file_name, local_file, 4);
    
    xml_write_save(local_file);
}

gboolean
load_game(gchar *file_name)
{
    if(!find_save_file(file_name))
	return FALSE;

    xml_read_save(file_name);

    return TRUE;
}
