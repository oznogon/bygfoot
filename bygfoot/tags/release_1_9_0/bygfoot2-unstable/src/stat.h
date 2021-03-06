#ifndef STAT_H
#define STAT_H

#include "bygfoot.h"
#include "league_struct.h"
#include "stat_struct.h"

SeasonStat
stat_season_stat_new(gint season_number);

LeagueStat
stat_league_new(gint clid);

GArray*
stat_update_league_teams(const GArray *teams, gint compare_type);

void
stat_update_leagues(void);

void
stat_update_league_players(League *league);

void
stat_create_season_stat(void);

void
stat_show_av_league_goals(void);

#endif
