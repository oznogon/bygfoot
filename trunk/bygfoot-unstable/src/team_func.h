/* assign value of a player depending on
   skill, talent, age and league */
gint
assign_value(player pl);

/* assign wage depending on league and value */
gint
assign_wage(player pl);

/* determine on which position a given player plays
   depending on his number in the team and on the structure
   the team plays; if 'generation' is 1, this is position
   assignment, else just a query */
gint
get_position_from_structure(gint team_id, gint structure,
				 gint player_number,
				 gint generation);

void
adapt_ability(gint team_id, gint player_number);

/* estimate a player's talent value. depends on how
   good the human player's scout is.
   the estimate is between the player's skill
   and 9.9 */
gfloat
estimate_talent(player pl);

/* fill in a player's data depending on league and
   the player's number in his team */
void
generate_player(gint team_id, gfloat team_factor,
		     gint player_number);

/* calculate a player's current skill depending on position,
   health etc. */
gfloat
calculate_cskill(player pl);

/* update a player's skill depending on age, talent
   and how many weeks ago the last update was made */
void
update_skill(gint team_id, gint player_number);

/* calculate the fitness increase or decrease of a player */
gfloat calculate_new_fitness(player pl, gint increase);

/* find a rndom substitute player who's not injured or
   banned for a given position */
gint
find_substitute(gint team_id, gint position);

/* copy player source to player dest */
void
copy_player(player source, player *dest);

void
replace_player_by_new(gint team_id, gint player_number);

/* swap two players */
void
swap_players(gint team_id1, gint player_number1,
		  gint team_id2, gint player_number2);

/* move the player from team1 to team2 and replace
   him by a new one */
void
move_player(gint team1, gint player_number, gint team2);

/* rndomly substitute the goalie of a cpu-team;
   the better goalie is favoured quadratically depending
   on the skills, however. */
void
substitute_goalie(gint team_id);

/* estimate value and wage of the player
   on the transferlist with index idx */
void
estimate_value_wage(gint idx);

/* remove a player from a team */
void
remove_player(gint team_id, gint player_number);

/* return 1 if there are injured/banned players
   in the human player's team, 0 otherwise */
gint
unfit_players(void);

/* fire a player and deduce either a one-time compensation
   or set a counter for paying the wage for some months */
void
fire_player(gboolean one_time);

/* return the number of players in the human player's team */
gint
players_in_team(void);
