#ifndef MATH_H
#define MATH_H

#include <math.h>
#include "bygfoot.h"
#include "variables.h"

/* macros */
#define rnd(lower,upper) ((gfloat)random()/(gfloat)0x7fffffff*((upper)-(lower))+(lower))
#define rndi(lower,upper) ((gint)rint( rnd((gfloat)(lower) - 0.499, (gfloat)(upper) + 0.499) ))

/*generate a gauss-distributed random nr
  ("by Box and Muller, and recommended by Knuth");*/
gfloat
gaussrand(void);

/* Generate a gauss-distributed random nr using 'gaussrand';
   it'll be with a probability of 99.7% between b and c
   and at any case between a and d.
   If b<a (d<c);, there are no such restrictions */
gfloat
gauss_dist(gfloat a, gfloat b, gfloat c, gfloat d);

/* write a permutation of the integers between 'min' and 'max'
   into the array 'perm' */
gint
write_permutation(gint *perm, gint min, gint max);

/* sum up the elements of an integer array between indices
   'min' and 'max' */
gint
sum(gint *array, gint min, gint max);

/* sort a gfloat array between indices 'first_element'
   and 'last_element';
   the function assumes that 'order' can hold
   'last_element' - 'first_element' + 1 entries */
gint
sort_float_array(gfloat *array, gint *order, 
		      gint first_element, gint last_element);

/* get the 'place'th digit of the integer beginning from
   the right; if place is greater than 10, return
   the first few digits; if place is greater than 20,
   return the last few digits.
   examples: value = 1345, place = 3 --> return value 3;
   value = 1345, place = 13 --> return value 134;
   value = 1345, place = 23 --> return value 345 */
gint
get_place(gint value, gint place);

/* round an integer with precision 'places' if places > 0
   and with precision length - places otherwise */
gint
round_integer(gint number, gint places);

/* return maximal or minimal value */
gfloat
max_float_array(gfloat *array, gint last, gboolean min);

#endif
