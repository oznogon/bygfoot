#ifndef XML_FIXTURES_H
#define XML_FIXTURES_H


#include "bygfoot.h"
#include "variables.h"
#include "xml.h"

void
xml_fixtures_read_start_element (GMarkupParseContext *context,
				const gchar         *element_name,
				const gchar        **attribute_names,
				const gchar        **attribute_values,
				gpointer             user_data,
				GError             **error);

void
xml_fixtures_read_end_element    (GMarkupParseContext *context,
				 const gchar         *element_name,
				 gpointer             user_data,
				 GError             **error);

void
xml_fixtures_read_text         (GMarkupParseContext *context,
			       const gchar         *text,
			       gsize                text_len,  
			       gpointer             user_data,
			       GError             **error);

void
xml_fixtures_read(gchar *file_name);

void
xml_fixtures_write(gchar *file_name);

void
xml_fixtures_read(gchar *file_name);

#endif
