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

/* team results
  results[0]: games
  results[1]: won
  results[2]: lost
  results[3]: draw
  results[4]: points
  results[5]: goals for
  results[6]: goals against */

enum
{
    RES_GAM = 0,
    RES_WON,
    RES_DRAW,
    RES_LOST,
    RES_GF,
    RES_GA,
    RES_PTS,
    RES_DUMMY1,
    RES_DUMMY2,
    RES_DUMMY3,
    RES_DUMMY4,
    RES_DUMMY5,
    RES_END
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

/* the indices for a season statistic
   element:
   my_team: name of the human player's team
   prem - nat_conf: the five english league champions
   fa, league, charity: fa cup and league cup and charity 
                        shield winners
   cl, cwc, uefa: champions' league, cup winners' cup
                  and uefa cup winners
   supercup: uefa supercup winner */

enum
{
    STAT_MY_TEAM = 0,
    STAT_PREM,
    STAT_DIV1,
    STAT_DIV2,
    STAT_DIV3,
    STAT_NAT_CONF,
    STAT_CL,
    STAT_CWC,
    STAT_UEFA,
    STAT_FA,
    STAT_LEAGUE,
    STAT_CHARITY,
    STAT_SUPERCUP,
    STAT_END
};

struct _season_stat
{
    gint season_number;
    gint my_league;
    gint my_rank;

    gchar team_names[STAT_END][50];

    /* best three goal scorers and
       goal keepers for each league */
    player_stat best_players[30];
    
    season_stat *next;
};
