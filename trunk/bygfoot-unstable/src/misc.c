#include <time.h>
#include "defs.h"

/* check whether 'item' is in array 'list' between
   'min' and 'max' */
gint is_in_list(gint *list, gint min, gint max, gint item)
{
    gint i;

    for(i=min;i<=max;i++)
	if(item == list[i])
	    return 1;
    
    return 0;
}

/* check whether 'item' is in array 'list' between
   'min' and 'max'; this time we compare strings */
gint is_in_list_char(gchar list[][50], gint min,
		     gint max, gchar *item)
{
    gint i;
    
    for(i=min;i<=max;i++)
	if(!strcmp(item, list[i]))
	    return 1;
    
    return 0;
}

/* swap two integers */
void swap_int(gint *first, gint *second)
{
    gint swap = *first;

    *first = *second;
    *second = swap;
}

/* swap two floats */
void swap_float(gfloat *first, gfloat *second)
{
    gfloat swap = *first;

    *first = *second;
    *second = swap;
}

/* swap two strings */
void swap_char(gchar **first, gchar **second)
{
    gchar swap[BUF_SIZE_BIG];
    
    strcpy(swap, *first);
    strcpy(*first, *second);
    strcpy(*second, swap);
}

/* round an integer with precision 'places' if places > 0
   and with precision length - places otherwise */
gint round_integer(gint number, gint places)
{
    gint length = 0;
    gfloat copy = (gfloat)number;

    if(places > 0)
	return (gint)rint( (gfloat)number / powf(10, places) ) *
	    powf(10, places);

    while(copy >= 1)
    {
	copy /= 10;
	length++;
    }

    return (gint)rint( (gfloat)number / powf(10, length + places) ) *
	powf(10, length + places);
}

/* return the index 0 or 1 for the variables like
   stadium_facts[] or goals[] */
gint get_statistics_variable_index(fixture fix)
{
    if(fix.type < 6000)
	return 1;

    if(fix.type > 50000 &&
       fix.type % 10 == 5)
	return 1;
    
    return 0;
}

/* print a thousands-grouped output of 'number' into 'buf',
   like 2 234 345 instead of 2234345 */
void print_grouped_int(gint number, gchar *buf, gint append)
{
    gint i;
    gchar buf2[BUF_SIZE_SMALL];
    gint length = 0;
    gfloat copy = (gfloat)(abs(number));
    gint number2 = abs(number);

    if(append == 0)
      strcpy(buf, "");

    while(copy >= 1)
    {
	copy /= 10;
	length++;
    }

    if(length > 9)
    {
	sprintf(buf2, "%d", number);
	strcat(buf, buf2);
	return;
    }

    for(i = length; i > 0; i--)
    {
	sprintf(buf2, "%d", get_place(number2, i));
	strcat(buf, buf2);
	if(i % 3 == 1)
	    strcat(buf, " ");
    }

    if(number < 0)
    {
	sprintf(buf2, "- ");
	strcat(buf2, buf);
	sprintf(buf, "%s", buf2);
    }
    else if(number == 0)
	strcat(buf, "0");
}

/* allocate the memory needed
   for 'dest' and copy 'source' into it */
void print_string(gchar **dest, const gchar *source)
{                                       
    if(*dest != NULL)
	g_free(*dest);

    *dest=(gchar*)g_malloc(strlen(source) + 1);
    strcpy(*dest, source);
}

/* free the history pointers */
void free_history(void)
{
    season_stat *stat = history;
    season_stat *stat2;

    if(history == NULL)
	return;

    stat2 = stat->next;
    while(stat2 != NULL)
    {
	g_free(stat);
	stat = stat2;
	stat2 = stat->next;
    }
    
    g_free(stat);

    history = NULL;
}

/* get a float representation of someone's age
   based on his birth year and month written as 
   an integer, e.g. 196510 for 1965, Oct. */
gfloat get_age_from_birth(gint date)
{
    GDate *current_date = g_date_new();
    GDate *birth_date = g_date_new();

    g_date_set_time(current_date, time(NULL));
    g_date_set_dmy(birth_date, 15, get_place(date, 22),
		   get_place(date, 14));

    return (gfloat)g_date_days_between(birth_date, current_date) / 365.25;
}
