#ifndef FIXTURES_H
#define FIXTURES_H

/* return the index 0 or 1 for the variables like
   stadium_facts[] or goals[] */
gint
get_statistics_variable_index(fixture fix);

/* find out if the human player's team is involved
   in the fixture */
gboolean
my_team_involved(fixture fix);

/* find out whether the human player's team
   plays (or played); the given week */
gboolean
my_team_played(gint week_number);

/* return the index of the first fixture with type 'type' */
gint
get_first_fixture(gint type);

gboolean
exists_fixture(gint fixture_type, gint places);

/* get the next week from 'week' up- or downwards
   featuring games of the human player's team or
   a certain league */
gint
get_my_fixture_week(gint week_number, gint direction);

/* write one fixture */
void
write_fixture(gint idx, gint team0, gint team1,
		   gint type, gint week_number);

/* write one round of round robin games */
void
write_round_robin_round(gint *ids, gint max, gint special,
			     gint type, gint week_number);

/* write round robin fixtures for the teams with ids
   between 'first_team_id' and 'last_team_id'.
   first_week: the week the first round is played
   week_leap: the number of weeks between two rounds
   the way it's done is difficult to explain, so there
   won't be many comments below */
void
write_round_robin(gint first_team_id,
		       gint last_team_id,
		       gint type, gint first_week,
		       gint week_leap);

/* write rndomly home-away-pairings for the teams
   with ids team_ids[0] - team_ids[max - 1]
   used in UEFA and Cup Winners Cup */
void
write_pairings(gint *team_ids, gint max,
		    gint first_type, gint type_leap,
		    gint week_number, gint week_leap,
		    gint home_away);

void
write_fixtures_euro_cups(void);

void
write_fixtures_english_cups(void);

/* write the fixtures for the round robin games of
   a season and the first rounds of the cups */
void
write_season_fixtures(void);

void
find_left_out(gint type, gint *left_out);

/* return winner or runner-up of the game
   with fixture type 'type' */
gint
get_winner_runner_up(gint type, gint winner);

/* return the id of the winner of fix
   without taking into account whether there
   was a first leg; write 'L' or 'W' or 'D' from the
   point of view of team 'team_id' into buf if buf isn't NULL  */
gint
winner_of_regular(fixture fix, gchar *buf, gint team_id);

/* return the id of the winner of fix */
gint
winner_of(fixture fix);

/* find the winners of the fixtures with appropriate types */
void
find_winners(gint first_type, gint leap, gint max, gint *winners);

void
update_fixtures_english_cups(gint type);

/* write quarter-final fixtures after the group phase */
void
update_champions_league_group(void);

void
update_home_away_fixtures(gint first_type, gint max, gint new_type);

void
update_final(gint first_type, gint new_type);

void
update_uefa_supercup(void);

void
update_charity_shield(void);

void
update_relegation_second(void);

void
update_relegation_first(void);

/* weekly update of the fixtures, i.e. new fixtures are
   generated when a round of a cup is over */
void
update_fixtures(void);

#endif
