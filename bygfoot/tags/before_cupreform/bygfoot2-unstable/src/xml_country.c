#include "file.h"
#include "free.h"
#include "misc.h"
#include "variables.h"
#include "xml_cup.h"
#include "xml_country.h"
#include "xml_league.h"

/**
 * The tags used in the XML files defining countries.
 */
#define TAG_COUNTRY "country"
#define TAG_NAME "name"
#define TAG_SYMBOL "symbol"
#define TAG_SID "sid"
#define TAG_LEAGUES "leagues"
#define TAG_LEAGUE "league"
#define TAG_CUPS "cups"
#define TAG_CUP "cup"
#define TAG_SUPERCUPS "supercups"

/**
 * Enum with the states used in the XML parser functions.
 */
enum XmlCountryStates
{
    STATE_COUNTRY = 0,
    STATE_NAME,
    STATE_SYMBOL,
    STATE_SID,
    STATE_LEAGUES,
    STATE_LEAGUE,
    STATE_CUPS,
    STATE_CUP,
    STATE_SUPERCUPS,
    STATE_END
};

/**
 * The state variable used in the XML parsing functions.
 */
gint state;
gboolean in_supercups;

/**
 * The function called by the parser when an opening tag is read.
 * The state variable is changed in this function and
 * sometimes memory allocated for the information that's going to be read.
 * @see The GLib manual (Simple XML parser).
 */
void
xml_country_read_start_element (GMarkupParseContext *context,
				const gchar         *element_name,
				const gchar        **attribute_names,
				const gchar        **attribute_values,
				gpointer             user_data,
				GError             **error)
{
    if(strcmp(element_name, TAG_NAME) == 0)
	state = STATE_NAME;
    else if(strcmp(element_name, TAG_SYMBOL) == 0)
	state = STATE_SYMBOL;
    else if(strcmp(element_name, TAG_SID) == 0)
	state = STATE_SID;
    else if(strcmp(element_name, TAG_LEAGUES) == 0)
    {
	state = STATE_LEAGUES;
	if(ligs == NULL)
	    ligs = g_array_new(FALSE, FALSE, sizeof(League));
    }
    else if(strcmp(element_name, TAG_LEAGUE) == 0)
	state = STATE_LEAGUE;
    else if(strcmp(element_name, TAG_CUPS) == 0)
    {
	state = STATE_CUPS;
	if(cps == NULL)
	    cps = g_array_new(FALSE, FALSE, sizeof(Cup));
	in_supercups = FALSE;
    }
    else if(strcmp(element_name, TAG_SUPERCUPS) == 0)
    {
	state = STATE_SUPERCUPS;
	if(scps == NULL)
	    scps = g_array_new(FALSE, FALSE, sizeof(Cup));
	in_supercups = TRUE;
    }
    else if(strcmp(element_name, TAG_CUP) == 0)
	state = STATE_CUP;
    else if(strcmp(element_name, TAG_COUNTRY) != 0)
	g_warning("xml_country_read_start_element: unknown tag: %s; I'm in state %d\n",
		  element_name, state);
}

/**
 * The function called by the parser when a closing tag is read.
 * The state variable is changed in this function.
 * @see The GLib manual (Simple XML parser).
 */
void
xml_country_read_end_element    (GMarkupParseContext *context,
				 const gchar         *element_name,
				 gpointer             user_data,
				 GError             **error)
{
    if(strcmp(element_name, TAG_NAME) == 0 ||
       strcmp(element_name, TAG_SYMBOL) == 0 ||
       strcmp(element_name, TAG_SID) == 0 ||
       strcmp(element_name, TAG_LEAGUES) == 0 ||
       strcmp(element_name, TAG_CUPS) == 0 ||
       strcmp(element_name, TAG_SUPERCUPS) == 0)
	state = STATE_COUNTRY;
    else if(strcmp(element_name, TAG_LEAGUE) == 0)
	state = STATE_LEAGUES;
    else if(strcmp(element_name, TAG_CUP) == 0)
	state = (in_supercups) ? STATE_SUPERCUPS : STATE_CUPS;

    else if(strcmp(element_name, TAG_COUNTRY) != 0)
	g_warning("xml_country_read_start_element: unknown tag: %s; I'm in state %d\n",
		  element_name, state);
}

/**
 * The function called by the parser when the text between tags is read.
 * This function is responsible for filling in the variables (e.g. team names)
 * when a file gets loaded.
 * @see The GLib manual (Simple XML parser).
 */
void
xml_country_read_text         (GMarkupParseContext *context,
			       const gchar         *text,
			       gsize                text_len,  
			       gpointer             user_data,
			       GError             **error)
{
    gchar buf[text_len + 1];

    strncpy(buf, text, text_len);
    buf[text_len] = '\0';

    if(state == STATE_NAME)
	country.name = g_string_new(buf);
    else if(state == STATE_SYMBOL)
	country.symbol = g_string_new(buf);
    else if(state == STATE_SID)
	country.sid = g_string_new(buf);
    else if(state == STATE_LEAGUE)
	xml_league_read(buf, ligs);
    else if(state == STATE_CUP)
	xml_cup_read(buf, (in_supercups) ? scps : cps);
}


/**
 * Function reading an XML file specifying a country.
 * The variable #country gets freed and overwritten afterwards.
 * @param country_name name of the xml file (e.g. 'country_england.xml')
 * to be read. Full path is not necessary, if the file is located in
 * one of the suppport directories; neither are the prefix 'country_'
 * or the suffix '.xml'.
 */
void
xml_country_read(const gchar *country_name)
{
    gint i;
    gchar *file_name = file_find_support_file(country_name, FALSE);
    GMarkupParser parser = {xml_country_read_start_element,
			    xml_country_read_end_element,
			    xml_country_read_text, NULL, NULL};
    GMarkupParseContext *context;
    gchar *file_contents;
    gint length;
    GError *error = NULL;
    gchar buf[SMALL];

    context = 
	g_markup_parse_context_new(&parser, 0, NULL, NULL);

    if(file_name == NULL)
    {
	sprintf(buf, "country_%s.xml", country_name);
	file_name = file_find_support_file(buf, TRUE);
    }
	
    if(!g_file_get_contents(file_name, &file_contents, &length, &error))
    {
	g_warning("xml_country_read: error reading file %s\n", file_name);
	misc_print_error(&error, FALSE);
	return;
    }

    state = STATE_COUNTRY;
    strcpy(buf, file_name);
    g_free(file_name);

    free_country(TRUE);

    if(g_markup_parse_context_parse(context, file_contents, length, &error))
    {
	g_markup_parse_context_end_parse(context, NULL);	
	g_markup_parse_context_free(context);
	g_free(file_contents);
    }
    else
    {
	g_critical("xml_country_read: error parsing file %s\n", buf);
	misc_print_error(&error, TRUE);
    }

    free_g_ptr_array(&acps);
    acps = g_ptr_array_new();
    for(i=0;i<cps->len;i++)
	g_ptr_array_add(acps, &cp(i));
}
