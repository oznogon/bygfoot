#include "xml_fixtures.h"

enum
{
    TAG_START = 0,
    TAG_FIXTURES,
    TAG_FIXTURE,
    TAG_FIXTURE_TEAMID,
    TAG_FIXTURE_RESULT,
    TAG_FIXTURE_TYPE,
    TAG_FIXTURE_WEEKNUMBER,
    TAG_END
};

gint state;
gint fix_idx, idx[2];

void
xml_fixtures_read_start_element (GMarkupParseContext *context,
				const gchar         *element_name,
				const gchar        **attribute_names,
				const gchar        **attribute_values,
				gpointer             user_data,
				GError             **error)
{
    gint i;
    gint tag = xml_get_tag_from_name(element_name);
    gboolean valid_tag = FALSE;

    for(i=TAG_START;i<TAG_END;i++)
	if(tag == i)
	{
	    state = i;
	    valid_tag = TRUE;
	}
    
    if(tag == TAG_FIXTURE_TEAMID)
	idx[1] = 0;
    else if(tag == TAG_FIXTURE)
	idx[0] = idx[1] = 0;

    if(!valid_tag)
    {
	g_warning("xml_fixtures_read_start_element: unknown tag: %s\n",
		  element_name);
	g_warning("I'm in state %d", state);
    }
}

void
xml_fixtures_read_end_element    (GMarkupParseContext *context,
				 const gchar         *element_name,
				 gpointer             user_data,
				 GError             **error)
{
    gint tag = xml_get_tag_from_name(element_name);

    if(tag == TAG_FIXTURE)
    {
	fix_idx++;
	state = TAG_FIXTURES;
    }
    else if(tag >= TAG_FIXTURE_TEAMID &&
	    tag <= TAG_FIXTURE_WEEKNUMBER)
	state = TAG_FIXTURE;
    else if(tag !=  TAG_FIXTURES)
    {
	g_warning("xml_fixtures_read_end_element: unknown tag: %s\n",
		  element_name);
	g_warning("I'm in state %d\n", state);
    }

    if(tag == TAG_FIXTURE_RESULT)
    {
	idx[1] = (idx[1] + 1) % 3;
	if(idx[1] == 0)
	    idx[0]++;
    }
}

void
xml_fixtures_read_text         (GMarkupParseContext *context,
			       const gchar         *text,
			       gsize                text_len,  
			       gpointer             user_data,
			       GError             **error)
{
    gchar buf[SMALL];
    gint int_value = -1;

    strncpy(buf, text, text_len);
    buf[text_len] = '\0';

    int_value = (gint)g_ascii_strtod(buf, NULL);

    if(state == TAG_FIXTURE_TEAMID)
	fixtures[fix_idx].team_id[idx[0]] = int_value;
    else if(state == TAG_FIXTURE_RESULT)
	fixtures[fix_idx].result[idx[0]][idx[1]] = int_value;
    else if(state == TAG_FIXTURE_TYPE)
	fixtures[fix_idx].type = int_value;
    else if(state == TAG_FIXTURE_WEEKNUMBER)
	fixtures[fix_idx].week_number = int_value;
}

void
xml_fixtures_read(gchar *file_name)
{
    gint i;
    gchar file[SMALL];
    GMarkupParser parser = {xml_fixtures_read_start_element,
			    xml_fixtures_read_end_element,
			    xml_fixtures_read_text, NULL, NULL};
    GMarkupParseContext *context;
    gchar *file_contents;
    gint length;
    GError *error = NULL;

    sprintf(file, "%s_%s.xml", file_name, XML_FILE_EXT_FIXTURES);

    context = 
	g_markup_parse_context_new(&parser, 0, NULL, NULL);

    if(!g_file_get_contents(file, &file_contents, &length, &error))
    {
	g_critical("xml_fixtures_read: error reading file %s\n", file);
	print_error(error, TRUE);
	return;
    }

    for(i=0;i<FIX_END;i++)
	fixtures[i].type = -1;

    fix_idx = idx[0] = idx[1] = 0;

    progress += PROGRESS_FIX;
    show_progress(progress / PROGRESS_MAX, _("Loading fixtures..."));

    if(g_markup_parse_context_parse(context, file_contents, length, &error))
    {
	g_markup_parse_context_end_parse(context, NULL);	
	g_markup_parse_context_free(context);
	g_free(file_contents);	
    }
    else
    {
	g_critical("xml_fixtures_read: error parsing file %s\n", file);
	print_error(error, TRUE);
    }
}

void
xml_fixtures_write(gchar *file_name)
{
    gint i, j, k;
    gint max = get_first_fixture(-1);
    gchar file[SMALL];
    FILE *xml_file;

    sprintf(file, "%s_%s.xml", file_name, XML_FILE_EXT_FIXTURES);

    my_fopen(file, "w", &xml_file, FALSE);

    progress += PROGRESS_FIX;
    show_progress(progress / PROGRESS_MAX, _("Saving fixtures..."));

    xml_write_init(xml_file, TAG_FIXTURES);
    
    for(i=0;i<max;i++)
    {
	fprintf(xml_file, "%s<_%d>\n", INDENT0, TAG_FIXTURE);

	for(j=0;j<2;j++)
	{
	    fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, 
		    TAG_FIXTURE_TEAMID, fixtures[i].team_id[j], TAG_FIXTURE_TEAMID);

	    for(k=0;k<3;k++)
		fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1, 
			TAG_FIXTURE_RESULT, fixtures[i].result[j][k], TAG_FIXTURE_RESULT);
	}

	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1,
		TAG_FIXTURE_TYPE, fixtures[i].type, TAG_FIXTURE_TYPE);
	fprintf(xml_file, "%s<_%d>%d</_%d>\n", INDENT1,
		TAG_FIXTURE_WEEKNUMBER, fixtures[i].week_number, TAG_FIXTURE_WEEKNUMBER);
	
	fprintf(xml_file, "%s</_%d>\n", INDENT0, TAG_FIXTURE);
    }

    xml_write_end(xml_file, TAG_FIXTURES);

    fclose(xml_file);
}
