#include <gtk/gtk.h>
#include <maths.h>
#include <stdlib.h>

#include "maths.h"

/*generate a gauss-distributed random nr
  ("by Box and Muller, and recommended by Knuth")*/
gfloat
gaussrand(void)
{
    static gfloat V1, V2, S;
    static gint phase = 0;
    gfloat X;

    if(phase == 0) {
	do {
	    gfloat U1 = (gfloat)rand() / RAND_MAX;
	    gfloat U2 = (gfloat)rand() / RAND_MAX;

	    V1 = 2 * U1 - 1;
	    V2 = 2 * U2 - 1;
	    S = V1 * V1 + V2 * V2;
	} while(S >= 1 || S == 0);

	X = V1 * sqrt(-2 * log(S) / S);
    } else
	X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X;
}

/* Generate a gauss-distributed random nr using 'gaussrand';
   it'll be with a probability of 99.7% between b and c
   and at any case between a and d.
   If b<a (d<c), there are no such restrictions */
gfloat
gauss_dist(gfloat a, gfloat b, gfloat c, gfloat d)
{						   
    gfloat result;				   
     	
    result = (gfloat)(c-b) / 6 * gaussrand()+(gfloat)(c+b) / 2;

    if(result < a && a <= b) result = a;
    if(result > d && d >= c) result = d;

    return result;
}

/* write a permutation of the integers between 'min' and 'max'
   into the array 'perm' */
gint
write_permutation(gint *perm, gint min, gint max)
{
    /*c*/
    gdouble x[max - min + 1];
    gint i, j;

    for(i=0;i<max - min + 1;i++)
    {
	perm[i] = min - 1;
	x[i] = rnd(0,1);
    }
    
    for(i=0;i<max - min + 1;i++)
    {
	for(j=i;j<max - min + 1;j++)
	{
	    if(x[i] >= x[j])
		perm[i]++;
	    else
		perm[j]++;
	}
    }

    return 0;
}

/* sum up the elements of an integer array between indices
   'min' and 'max' */
gint
sum(gint *array, gint min, gint max)
{
    gint i;
    gint result = 0;
    
    for(i=min;i<=max;i++)
	result += array[i];

    return result; 
}

/* sort a gfloat array between indices 'first_element'
   and 'last_element';
   the function assumes that 'order' can hold
   'last_element' - 'first_element' + 1 entries */
gint
sort_float_array(gfloat *array, gint *order, 
		      gint first_element, gint last_element)
{
    gint i, j, entries = last_element - first_element + 1;
    gint temporary_order[ last_element - first_element + 1];
    
    for(i=0;i<entries;i++)
	temporary_order[i] = 0;
    
    for(i=0;i<entries;i++)
	for(j=i+1;j<entries;j++)
	    if(array[first_element + i] > array[first_element + j])
		temporary_order[j]++;
	    else
		temporary_order[i]++;
    
    for(i=0;i<entries;i++)
    {
	for(j=0;j<entries;j++)
	    if(temporary_order[i] == j)
		order[j] = first_element + i;
    }    

    return 0;
}

/* get the 'place'th digit of the integer beginning from
   the right; if place is greater than 10, return
   the first few digits; if place is greater than 20,
   return the last few digits.
   examples: value = 1345, place = 3 --> return value 3;
   value = 1345, place = 13 --> return value 134;
   value = 1345, place = 23 --> return value 345 */
gint
get_place(gint value, gint place)
{
    if(place < 10)
	return (value % (gint)powf(10, place) -
		value % (gint)powf(10, place - 1)) /
	    (gint)powf(10, place - 1);

    else if(place < 20)
    {
	while(value >= (gint)powf(10, place % 10))
	    value = (value - value % 10) / 10;
	
	return value;
    }
    
    return value % (gint)powf(10, place % 10);
}

/* round an integer with precision 'places' if places > 0
   and with precision length - places otherwise */
gint
round_integer(gint number, gint places)
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

