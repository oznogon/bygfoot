#ifndef HISTORY_H
#define HISTORY_H

#include <gtk/gtk.h>

struct _player;

/***************** interface functions **********************/

/* player */
void reset_player_history(struct _player * pl);
void update_player_history(struct _player * pl);
void print_player_history(struct _player * pl);

/* finances */
void reset_finances_history(void);
void update_finances_history(void);
void print_finances_history(void);

/*********************** player *****************************/

struct player_history
{
	gint season;
	gint week;
	gint wage;
	/* warning : cskill and fitness are coded as gint */
	gint cskill;
	gint fitness;
};

struct player_history_list
{
	gint size;
	struct player_history * list;
};

/* helper functions */
void reset_player_history_list(struct player_history_list * plist);
void print_player_history_list(struct player_history_list * plist);

/********************* finances ****************************/

struct finances_history
{
	gint season;
	gint week;
	gint money;
};

struct finances_history_list
{
	gint size;
	struct finances_history * list;
};

/* helper functions */
void reset_finances_history_list(struct finances_history_list * flist);
void update_finances_history_list(struct finances_history_list * flist);
void print_finances_history_list(struct finances_history_list * flist);

/* write a player's history for attribute 'type'
   into the garray which will be used for drawing the graph then */
void
get_history_player(gint team_id, gint player_number, 
		   gint *veclen, GArray *array, gint type);

#endif
