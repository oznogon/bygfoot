#ifndef TRANSFERS_H
#define TRANSFERS_H

#include "bygfoot.h"
#include "variables.h"

/* 'time' is the number of weeks
   the player stays on the list. it's
   randomised between 2 and 5.
   time = -1 means the transfer is empty */
typedef struct _transfer transfer;
struct _transfer
{
    gint team_id, player_number;
    gint time;
    /* the scout's estimates for price and
       wage of the player */
    gint estimates[2];
};

/* return number of players on list */
gint
players_on_transferlist(void);

/* return whether a player's on the list */
gint
is_on_transferlist(gint team_id, gint player_number);

/* sort the transfer list */
void
sort_transferlist(gint sort_attrib);

/* add the specified player to the transferlist; he'll stay there
   for 'time' weeks */
void
add_transfer(gint team_id, gint player_number, gint time);

/* add a rndom number of new players to the transferlist */
void
add_new_transfers(void);

/* remove a player from the transfer list */
void
remove_transfer(gint number, gboolean resort);

/* each week some players are placed for a rndom
   number of weeks (2-5); on the transfer list.
   week 35 is transfer deadline, all players are removed
   and no new players appear until the following season */
void
update_transferlist(void);

/* move a player from the transferlist to the human player's team
   and replace him */
void
execute_transfer(gint idx);

/* find out whether the human player
   has to be notified of a new player on the
   transfer list */
void
check_notify(gint idx);

#endif
