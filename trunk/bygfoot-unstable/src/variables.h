#ifndef VARIABLES_H
#define VARIABLES_H

#include "bygfoot.h"
#include "fixture.h"
#include "game.h"
#include "player.h"
#include "team.h"
#include "transfer.h"

/**************************************************
 * Global variables                               *
 **************************************************/

team teams[178];

/* we keep the player names in the memory to avoid 
   time consuming fopen-actions when replacing cpu-players */
gchar player_names[10504][50];

stadium stadiums[178];

/* Season objectives */
objective seasonObjective[MAX_OBJECTIVE];

fixture fixtures[FIX_END];

/* human player's team id */
gint my_team;

/* human player's scout;
   (4 is bad, 3 average, 2 good and 1
   best) */
gint scout;

/* human player's physio */
gint physio;

gint finances[FIN_END];

gint options[OPT_END];

gint counters[COUNT_END];

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

/* font selected for the two main windows */
gchar font_name[SMALL];

/* country_file the user's ... well, using */
gchar country_file_name[SMALL];

/* names for the five leagues and the cups */
gchar league_names[LEAGUE_NAMES_END][50];

/* number of active popup windows */
gint popups_active;

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

/* progress bar window */
GtkWidget *progressbar_window;

/* savegame file */
GString *save_file;

/* this tells us whether we are in debug mode */
gboolean debug;

/* menu status, ie. what's the user looking at currently? */
/*
  current status values ( ranges like a - b include a but not b):

  -100000 - -200000: showing a stats graph
  -60000: the first live game is being shown

  -50000: a live game was shown, there are no more live games

  -1: autosave in progress
  0: main menu; opponent info

  50000: player info is shown on the right

  100000 - 200000: transfers mode
    100000: transfer list is shown
    1100XX: transfer list shown, row XX selected
    120000: the list of the teams in the game is shown
            on the right (browse teams mode)
    130XXX: the player list of team XXX is shown on the right

  200000 - 300000: figures mode (fixtures, tables etc.)
    2100XX: fixtures of week XX are shown
    22000X: tables of league X are shown
    23000X: best player stats of league X are shown
    2400XX: history of season XX is shown
    
  300000 - 400000: finances/stadium
    300000: finances and stadium are shown
    310000: the user wants to get a loan
    320000: the user wants to pay back a loan

  400000 - 500000: preview mode
    400000: the preview is shown

  600000 - 700000: game summary mode
    600000/1: the user's clicked on 'new week'
    600010: preview is shown at the beginning of the new week
    600011: league results are shown at the beginning of the new week

  800000 - 900000: status for the file selection window
    800000: load a game
    800001: save a game
    800002: load a game from the team selection (startup) window
    800010: load a country file from the team selection window
    800100: load a country file when in editor mode
    800110: load a team definitions file when in editor mode
    800120: export a team definition in editor mode
    800121: import a team definition in editor mode

  900000 - 1000000: job offer
    900XXX: team XXX offers a job because the user's successful
    950XXX: team XXX offers a job because the user was fired
*/
gint status;

#endif
