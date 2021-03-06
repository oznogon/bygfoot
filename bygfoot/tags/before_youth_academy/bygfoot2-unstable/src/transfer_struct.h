#ifndef TRANSFER_STRUCT_H
#define TRANSFER_STRUCT_H

#include "bygfoot.h"
#include "player_struct.h"
#include "team_struct.h"

/** Structure representing a player on the transfer list. */
typedef struct
{
    /** Team of the player. */
    Team *tm;
    /** Id of player in the team. */
    gint id;
    /** Time until player gets removed from the list. */
    gint time;
    /** Estimated fees and wages for different scout qualities. */
    gint fee[QUALITY_END], wage[QUALITY_END];
    /** Offers for the player. */
    GArray *offers;
} Transfer;

typedef struct
{
    /** The team that makes the offer. */
    Team *tm;
    /** Transfer fee and wage offer. */
    gint fee, wage;
    /** Whether the offer got accepted or rejected etc. */
    gint status;
} TransferOffer;

enum TransferOfferStatus
{
    TRANSFER_OFFER_NOT_CONSIDERED = 0,
    TRANSFER_OFFER_ACCEPTED,
    TRANSFER_OFFER_REJECTED,
    TRANSFER_OFFER_REJECTED2,
    TRANSFER_OFFER_END
};

#endif
