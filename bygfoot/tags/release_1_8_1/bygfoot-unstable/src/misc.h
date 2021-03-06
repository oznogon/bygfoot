#ifndef MISC_H
#define MISC_H

#include <time.h>
#include "bygfoot.h"
#include "variables.h"
#include "maths.h"
#include "free.h"

void
print_error(GError *error, gboolean abort_program);

/* remove some of the first or last characters from src and copy
   the rest to dest; no error-checking is done */
void
truncate_string(const gchar *src, gchar *dest, gint number_of_chars);

/* check whether 'item' is in array 'list' between
   'min' and 'max' */
gint
is_in_list(gint *list, gint min, gint max, gint item);

/* check whether 'item' is in array 'list' between
   'min' and 'max'; this time we compare strings */
gint
is_in_list_char(gchar list[][50], gint min,
		     gint max, gchar *item);

/* swap two integers */
void
swap_int(gint *first, gint *second);

/* swap two floats */
void
swap_float(gfloat *first, gfloat *second);

/* swap two strings */
void
swap_char(gchar **first, gchar **second);

/* print a thousands-grouped output of 'number' into 'buf',
   like 2 234 345 instead of 2234345 */
void
print_grouped_int(gint number, gchar *buf, gint append);

/* get a float representation of someone's age
   based on his birth year and month written as 
   an integer, e.g. 196510 for 1965, Oct. */
gfloat
get_age_from_birth(gint date);

#endif
