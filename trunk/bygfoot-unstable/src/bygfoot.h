/************************************************************
 * the global definitions file containing everything        *
 * that's needed in all c  files: enums, structures         *
 * and the global variables                                 *
 ***********************************************************/

#ifndef BYGFOOT_H
#define BYGFOOT_H

/* current version number */

#define VERS "1.7.3"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <gtk/gtk.h>

/* 'define's and enums */
#include "enums.h"

/**************************************************
 * Structures                                     *
 **************************************************/

typedef struct _stadium stadium;
typedef struct _player_stat player_stat;
typedef struct _season_stat season_stat;

struct _stadium
{
    gint capacity, average_attendance;
    /* safety is between 0 and 1 */
    gfloat safety;
    gint games;
};

struct _player_stat
{
    gchar name[19], team_name[50];
    gint goals, games;
};

struct _season_stat
{
    gint season_number;
    gint my_league;
    gint my_rank;

    gchar team_names[STAT_END][50];

    /* best three goal scorers and
       goal keepers */
    player_stat best_players[6];

    season_stat *next;
};

#endif
