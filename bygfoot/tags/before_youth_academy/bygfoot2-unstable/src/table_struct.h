#ifndef TABLE_STRUCT_H
#define TABLE_STRUCT_H

#include "bygfoot.h"
#include "team_struct.h"

/**
   Table element values.
   @see TableElement
   @see Table
*/
enum TableElementValues
{
    TABLE_PLAYED = 0,
    TABLE_WON,
    TABLE_DRAW,
    TABLE_LOST,
    TABLE_GF,
    TABLE_GA,
    TABLE_GD,
    TABLE_PTS,
    TABLE_END
};

/**
   An element representing a team in the tables.
   @see Table
   @see #TableElementValues
*/
typedef struct
{
    Team *team;
    gint team_id;
    /** The rank of the element before the last update. 
	Used to display an arrow if the rank changed. */
    gint old_rank;
    gint values[TABLE_END];
} TableElement;

/**
   A table belonging to a league or a cup with round robin.
   @see TableElement
*/
typedef struct
{
    GString *name;
    gint clid;
    /** The cup round (or -1 if it's a league). */
    gint round;
    GArray *elements;
} Table;

#endif
