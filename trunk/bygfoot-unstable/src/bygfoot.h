/************************************************************
 * the global definitions file containing everything        *
 * that's needed in all c  files: enums, structures         *
 * and the global variables                                 *
 ***********************************************************/

#ifndef BYGFOOT_H
#define BYGFOOT_H

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

typedef struct _player player;
typedef struct _team team;
typedef struct _stadium stadium;
typedef struct _goal goal;
typedef struct _fixture fixture;
typedef struct _transfer transfer;
typedef struct _player_stat player_stat;
typedef struct _season_stat season_stat;

struct _player
{
    gchar name[19];
    
    /* pos: position
       0: goalie,
       1: defender,
       2: midfielder,
       3: forward
       cpos: current position
       skill_update: number of weeks since last
                     skill update (which occurs
		     appr. every 12-16 weeks)
       peak_age: age at which the player's skill reaches his talent value
                 (between 28 and 32) */
    gint pos, cpos, health, goals, booked, games;

    gfloat last_skill_update;
    
    gfloat age, peak_age;
    
    gint value, wage;

    /* cskill: current skill
       etal: a scout's estimation of
       a player's talent */
    gfloat skill, cskill,
	talent, etal, fitness;

    gint team_id;
};

struct _team
{
    gchar name[50];

    /* id: team id
       structure: structure of the team (number of defenders,
                  midfielders and forwards) */
    gint id, structure, results[RES_END];

    /* playing style from -2 to 2: (all out) defense,
       normal, (all out) attack */
    gint style;    
    
    player players[20];
};

struct _stadium
{
    gint capacity, average_attendance;
    /* safety is between 0 and 1 */
    gfloat safety;
    gint games;
};

struct _goal
{
    gint minute;    
    gint team_id;
    gint scorer;
    gint time;
    gint type;
};

struct _fixture
{
    gint team_id[2];
    gint result[2][3], type,
	week_number;
};

/* 'time' is the number of weeks
   the player stays on the list. it's
   randomised between 2 and 5.
   time = -1 means the transfer is empty */
struct _transfer
{
    gint team_id, player_number;
    gint time;
    /* the scout's estimates for price and
       wage of the player */
    gint estimates[2];
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

/**************************************************
 * Global variables                               *
 **************************************************/

team teams[178];

/* we keep the player names in the memory to avoid 
   time consuming fopen-actions when replacing cpu-players */
gchar player_names[10504][50];

stadium stadiums[178];

fixture fixtures[FIX_END];

/* human player's team id */
gint my_team;

/* human player's scout;
   (4 is bad, 3 average, 2 good and 1
   best) */
gint scout;

/* human player's physio */
gint physio;

/* rank_ids is an array of team ids;
   e.g. ranks[0] to ranks[19] give us
   the table of the premier division.
   114 to 129 are the tables of the
   four champions' league groups */
gint rank_ids[130];

/* rank[i] gives us the rank of the team with
   id i in the respective league */
gint rank[130];

/* the goals, injuries and bookings in the human player's
   games during one week (at most 2 games; players only get
   injured or booked in league games, so those variables are
   simple arrays) */
goal goals[2][50];
gint injuries[2][12];
gint booked[2][12];

/* attendance and special events for two games */
gint stadium_facts[2][2];

/* number of current season and week */
gint season, week;

/* the transfer list. it can contain 20 players
   at most (including those from the human player's
   team) */
transfer transferlist[20];

/* statistics of past seasons */
season_stat *history;

/* number of active popup windows */
gint popups_active;

/* menu status, ie. what's the user looking at currently? */
/*
  current status values ( ranges like a - b include a but not b):
  0: main menu; opponent info
  50000: player info is shown on the right
  100000 - 200000: transfers mode
  200000 - 300000: figures mode (fixtures, tables etc.)
  300000 - 400000: finances/stadium
  400000 - 500000: preview mode
  500000 - 600000: opponent player list mode
  600000 - 700000: game summary mode
  800000 - 900000: status for the file selection window
  900000 - 1000000: job offer
  -100000: the first live game is being shown
  -50000: a live game was shown, there are no more live games
*/
gint status;

/* whether the game's saved or not */
gboolean save_status;

/* information whether some popup notification has to be shown */
gint notify_status[NOTIFY_END];

/* ages of the two players that were swapped lastly */
gfloat undo_info[2];

/* bookmaker's tips for the outcome of games */
gint bookmaker_tips[2][2];

/* currently selected rows in the two treeviews */
gint selected_rows[2];

/* main window of the game */
GtkWidget *main_window;

/* savegame file */
GString *save_file;

/* font selected for the two main windows */
gchar font_name[BUF_SIZE_SMALL];

/* country_file the user's ... well, using */
gchar country_file_name[BUF_SIZE_SMALL];

/* this tells us whether we are in debug mode */
gboolean debug;

/* names for the five leagues and the cups */
gchar league_names[LEAGUE_NAMES_END][50];

gint finances[FIN_END];

gint options[OPT_END];

gint counters[COUNT_END];

#endif
