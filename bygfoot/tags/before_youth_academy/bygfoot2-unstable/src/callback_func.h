#ifndef CALLBACK_FUNC_H
#define CALLBACK_FUNC_H

#include "bygfoot.h"

void
callback_show_next_live_game(void);

void
callback_player_clicked(gint idx, GdkEventButton *event);

void
callback_show_last_match(gboolean start);

void
callback_show_fixtures(gint type);

void
callback_show_tables(gint type);

void
callback_pay_loan(void);

void
callback_get_loan(void);

void
callback_transfer_list_clicked(gint button, gint idx);

void
callback_offer_new_contract(gint idx);

void
callback_show_team(gint type);

void
callback_show_player_list(gint type);

void
callback_fire_player(gint idx);

void
callback_show_last_match_stats(void);

void
callback_show_fixtures_week(gint type);

void
callback_show_league_stats(gint type);

void
callback_show_season_history(gint type);

void
callback_show_next_opponent(void);

void
callback_show_player_team(void);

#endif
