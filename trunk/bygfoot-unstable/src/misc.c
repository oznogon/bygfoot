#include <time.h>

#include "misc.h"
#include "maths.h"

/* check whether 'item' is in array 'list' between
   'min' and 'max' */
gint
is_in_list(gint *list, gint min, gint max, gint item)
{
    gint i;

    for(i=min;i<=max;i++)
	if(item == list[i])
	    return 1;
    
    return 0;
}

/* check whether 'item' is in array 'list' between
   'min' and 'max'; this time we compare strings */
gint
is_in_list_char(gchar list[][50], gint min,
		     gint max, gchar *item)
{
    gint i;
    
    for(i=min;i<=max;i++)
	if(!strcmp(item, list[i]))
	    return 1;
    
    return 0;
}

/* swap two integers */
void
swap_int(gint *first, gint *second)
{
    gint swap = *first;

    *first = *second;
    *second = swap;
}

/* swap two floats */
void
swap_float(gfloat *first, gfloat *second)
{
    gfloat swap = *first;

    *first = *second;
    *second = swap;
}

/* swap two strings */
void
swap_char(gchar **first, gchar **second)
{
    gchar swap[BIG];
    
    strcpy(swap, *first);
    strcpy(*first, *second);
    strcpy(*second, swap);
}

/* print a thousands-grouped output of 'number' into 'buf',
   like 2 234 345 instead of 2234345 */
void
print_grouped_int(gint number, gchar *buf, gint append)
{
    gint i;
    gchar buf2[SMALL];
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

/* get a float representation of someone's age
   based on his birth year and month written as 
   an integer, e.g. 196510 for 1965, Oct. */
gfloat
get_age_from_birth(gint date)
{
    GDate *current_date = g_date_new();
    GDate *birth_date = g_date_new();

    g_date_set_time(current_date, time(NULL));
    g_date_set_dmy(birth_date, 15, get_place(date, 22),
		   get_place(date, 14));

    return (gfloat)g_date_days_between(birth_date, current_date) / 365.25;
}

/* remove some of the first or last characters from src and copy
   the rest to dest; no error-checking is done */
void
truncate_string(const gchar *src, gchar *dest, gint number_of_chars)
{
    gint i;
    gint num = ABS(number_of_chars);
    
    if(number_of_chars >= 0)
    {
	strncpy(dest, src, strlen(src) - num);
	dest[strlen(src) - num] = '\0';
	return;
    }

    for(i=0;i<strlen(src);i++)
	if(i >= num)
	    dest[i - num] = src[i];
    
    dest[i - num] = '\0';
}

void
print_error(GError *error)
{
    if(error == NULL)
	return;
    
    g_warning("error message: %s\n", error->message);
    g_error_free(error);
}
