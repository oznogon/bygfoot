#ifndef USER_H
#define USER_H

#include "bygfoot.h"
#include "user_struct.h"
#include "variables.h"

/** Convenience abbrevs. */
#define usr(i) g_array_index(users, User, i)
#define user_games_this_week_round() user_games_in_week_round(week, week_round)
#define current_user usr(cur_user)

void
user_set_up_team_new_game(User *user);

void
user_set_up_team(User *user);

void
user_set_up_finances(User *user);

User
user_new(void);

void
user_remove(gint idx, gboolean regenerate_team);

void
user_set_player_list_attributes(const User *user, PlayerListAttribute *attribute, gint list_number);

gboolean
user_games_in_week_round(gint week_number, gint week_round_number);

User*
user_from_team(const Team *tm);

void
user_weekly_update_counters(User *user);

Event
user_event_new(void);

void
user_event_add(User *user, gint type, gint value1, gint value2, 
	       gpointer pointer_value, gchar *string_value);

void
user_event_remove(User *user, gint idx);

void
user_event_show_next(void);

void
user_change_team(User *user, Team *tm);

void
user_set_up_counters(User *user);

#endif
