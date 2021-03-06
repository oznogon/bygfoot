#include "name.h"
#include "option.h"
#include "variables.h"
#include "xml_name.h"

/** Get a random player name from the given
    names list. If the names list is not found, create
    it from file. If the file can't be found, either,
    make some fuss and take one from the general names. */
GString*
name_get(const gchar *names_file)
{
    gint i;
    NameList new;

    if(name_lists->len > 1 && 
       math_rnd(0, 1) < const_float("float_name_random_list_prob"))
	return name_get_from_random_list();

    for(i=0;i<name_lists->len;i++)
	if(strcmp(names_file, nli(i).sid->str) == 0)
	    return name_get_from_list(&nli(i));

    /** Create new name list. */
    new.sid = NULL;
    new.first_names = new.last_names = NULL;

    xml_name_read(names_file, &new);

    if(new.sid == NULL)
    {
	g_warning("name_get_new: names file with sid '%s' not found, taking general names file.\n",
		  names_file);
	return name_get(opt_str("string_opt_player_names_file"));
    }

    if(stat5 != STATUS_GENERATE_TEAMS)
	name_shorten_list(&new);

    g_array_append_val(name_lists, new);

    return name_get_from_list(&nli(name_lists->len - 1));
}

/** Return a newly allocated GString with a randomly
    picked combined name from the list. */
GString*
name_get_from_list(const NameList *namelist)
{
    gchar buf[SMALL];

    sprintf(buf, "%s %s", name_get_random_first_name(namelist),
	    name_get_random_last_name(namelist));

    return g_string_new(buf);
}

/** Shorten a name list so that it doesn't occupy
    too much memory. */
void
name_shorten_list(NameList *namelist)
{
    gint idx;

    while(namelist->first_names->len * namelist->last_names->len >
	  const_int("int_name_max_product"))
    {
	if((gfloat)(namelist->first_names->len) /
	   (gfloat)(namelist->last_names->len) >
	   const_float("float_name_first_last_ratio"))
	{
	    idx = math_rndi(0, namelist->first_names->len - 1);
	    g_string_free((GString*)g_ptr_array_index(namelist->first_names, idx), TRUE);
	    g_ptr_array_remove_index_fast(namelist->first_names, idx);
	}
	else
	{
	    idx = math_rndi(0, namelist->last_names->len - 1);
	    g_string_free((GString*)g_ptr_array_index(namelist->last_names, idx), TRUE);
	    g_ptr_array_remove_index_fast(namelist->last_names, idx);
	}
    }
}
