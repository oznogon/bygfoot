#include "xml.h"

void
xml_read_save(gchar *file_name)
{
    gchar prefix[SMALL];

    progress = 0;

    xml_compress_files(file_name, TRUE);

    xml_get_save_file_prefix(file_name, prefix);

    xml_teams_read(prefix);
    xml_fixtures_read(prefix);
    xml_general_read(prefix);
    xml_remove_files(prefix);

    show_progress(-1, "");
}

void
xml_write_save(gchar *file_name)
{	
    progress = 0;

    xml_teams_write(file_name);
    xml_fixtures_write(file_name);
    xml_general_write(file_name);
    xml_compress_files(file_name, FALSE);

    show_progress(-1, "");
}

void
xml_write_init(FILE *xml_file, gint tag)
{
    /* TODO : an assert on the xml_file. I don't remember how to test if the FILE ptr is valid */
    fprintf(xml_file, "<?xml version=\"1.0\"?>\n");
	
    /* I'm not sure making a doctype is necessary. This file will be used only in bygfoot */
    /* and a DOCTYPE will be only a xml parsing test. */
    /* fprintf(xml_file, "<!DOCTYPE >\n"); */
    fprintf(xml_file, "%s<_%d>\n", INDENT0, tag);
}

void 
xml_write_end(FILE * xml_file, gint tag)
{
    fprintf(xml_file, "%s</_%d>\n", INDENT0, tag);
}


void
xml_remove_files(gchar *file_name)
{
    gchar buf[SMALL];
    GError *error = NULL;

    if(options[OPT_COMPRESS] == 0)
	return;
    
    sprintf(buf, "rm -f %s_%s.xml %s_%s.xml %s_%s.xml", 
	    file_name, XML_FILE_EXT_GENERAL,
	    file_name, XML_FILE_EXT_TEAMS,
	    file_name, XML_FILE_EXT_FIXTURES);
    g_spawn_command_line_sync(buf, NULL, NULL, NULL, &error);
    print_error(error, FALSE);
}

void
xml_compress_files(gchar *file_name, gboolean decompress)
{
    gint i;
    gchar buf[SMALL];
    gchar progress_text[SMALL];
    gchar command[SMALL];
    gchar directory[SMALL];
    GError *error = NULL;
    
    progress += PROGRESS_COMPRESS;

    if((!decompress && options[OPT_COMPRESS] == 0)
       || (decompress && !g_str_has_suffix(file_name, ".zip")))
	return;

    strcpy(directory, file_name);
    for(i=strlen(directory) - 1; i >= 0; i--)
    {	
	if(directory[i] == '/')
	{
	    directory[i] = '\0';
	    break;
	}
    }

    sprintf(progress_text, _("Compressing savegame..."));

    if(decompress)
    {
	sprintf(command, "unzip -qq %s -d %s", file_name, directory);
	sprintf(progress_text, _("Uncompressing savegame..."));
    }
    else
    {
	sprintf(buf, "zip -m -q -j %s.zip", file_name);
	sprintf(command, "%s %s_%s.xml %s_%s.xml %s_%s.xml", buf,
		file_name, XML_FILE_EXT_GENERAL,
		file_name, XML_FILE_EXT_TEAMS,
		file_name, XML_FILE_EXT_FIXTURES);
    }

    show_progress(progress / PROGRESS_MAX, progress_text);
    g_spawn_command_line_sync(command, NULL, NULL, NULL, &error);
    print_error(error, FALSE);
}


gint
xml_get_tag_from_name(const gchar *element_name)
{
    gchar buf[SMALL];

    truncate_string(element_name, buf, -1);

    return (gint)g_ascii_strtod(buf, NULL);
}

void
xml_get_save_file_prefix(gchar *file_name, gchar *prefix)
{
    gint i;

    if(g_str_has_suffix(file_name, ".zip"))
       truncate_string((const gchar*)file_name, prefix, 4);
    else if(g_str_has_suffix(file_name, ".xml"))
    {
	for(i=strlen(file_name) - 1; i >= 0; i--)
	    if(file_name[i] == '_')
		break;

	truncate_string(file_name, prefix, strlen(file_name) - i);
    }
    else
	g_warning("xml_get_save_file_prefix: unknown file type: %s\n", file_name);
}
