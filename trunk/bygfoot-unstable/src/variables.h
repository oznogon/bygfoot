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
