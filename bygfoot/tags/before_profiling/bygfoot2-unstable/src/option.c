#include "option.h"
#include "variables.h"

/** Return the string going with the option
    named 'name'.
    @param name The name of the option.
    @return The string_value of the option.
    @see #Option */
gchar*
option_string(const gchar *name, OptionList *optionlist)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_string: option named %s not found\n", name);
    else
	return ((Option*)element)->string_value->str;

    return NULL;
}

/** Return the GString pointer going with the option. */
GString*
option_string_pointer(const gchar *name, OptionList *optionlist)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_string: option named %s not found\n", name);
    else
	return ((Option*)element)->string_value;

    return NULL;
}

/** Return the integer going with the option
    named 'name'.
    @param name The name of the option.
    @return The value of the option.
    @see #Option */
gint
option_int(const gchar *name, OptionList *optionlist)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_int: option named %s not found\n", name);
    else
	return ((Option*)element)->value;

    return -1;
}

/** Return the address of an options variable. */
gint*
option_int_pointer(const gchar *name, OptionList *optionlist)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_int: option named %s not found\n", name);
    else
	return &((Option*)element)->value;

    return NULL;
}

/** Return the int going with the option named 'name'
    cast to float and divided by 1000.
    @param name The name of the option.
    @return The value of the option cast to float and divided by 1000.
    @see #Option */
gfloat
option_float(const gchar *name, OptionList *optionlist)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_float: option named %s not found\n", name);
    else
	return (gfloat)((Option*)element)->value / 10000;

    return -1;
}

/** Change the value of a string option in the array.
    @param name The name of the option.
    @param option_array The option array.
    @param new_value The value we set. */
void
option_set_string(const gchar *name, OptionList *optionlist, const gchar *new_value)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_set_string: option named %s not found\n", name);
    else
	g_string_printf(((Option*)element)->string_value, "%s", new_value);
}

/** Change the value of an int option in the array.
    @param name The name of the option.
    @param option_array The option array.
    @param new_value The value we set. */
void
option_set_int(const gchar *name, OptionList *optionlist, gint new_value)
{
    gpointer element = g_datalist_get_data(&optionlist->datalist, name);
    
    if(element == NULL)
	g_warning("option_set_int: option named %s not found\n", name);
    else
	((Option*)element)->value = new_value;
}
