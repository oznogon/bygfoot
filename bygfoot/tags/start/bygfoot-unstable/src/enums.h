/************************************************************
 * define statements and enumerations                       *
 ***********************************************************/

/**************************************************
 * define statements                              *
 **************************************************/

/* the length of the fixture array */
#define FIX_END 3000

/* size of char buffers */
#define BUF_SIZE_BIG 1000000
#define BUF_SIZE_SMALL 10000

/*  */
#define BUDGET finances[FIN_MONEY] + calculate_max_loan_credit(1)

/**************************************************
 * most of the enumerations are used as indices   *
 * in integer arrays                              *
 **************************************************/

/* indices for the text files */
enum
{
    TEXT_FILES_COUNTRY_ENG,
    TEXT_FILES_COUNTRY_DE,
    TEXT_FILES_COUNTRY_IT,
    TEXT_FILES_COUNTRY_FR,
    TEXT_FILES_COUNTRY_ES,
    TEXT_FILES_COUNTRY_RO,
    TEXT_FILES_COUNTRY_BR,
    TEXT_FILES_COUNTRY_PL,
    TEXT_FILES_PLAYER_NAMES,
    TEXT_FILES_DEFINITIONS,
    TEXT_FILES_HELP,
    TEXT_FILES_CONF,
    TEXT_FILES_END
};

/* indices for the league and cup names */
enum
{
    LEAGUE_NAMES_LEAGUE1,
    LEAGUE_NAMES_LEAGUE2,
    LEAGUE_NAMES_LEAGUE3,
    LEAGUE_NAMES_LEAGUE4,
    LEAGUE_NAMES_LEAGUE5,
    LEAGUE_NAMES_CL,
    LEAGUE_NAMES_CWC,
    LEAGUE_NAMES_UEFA,
    LEAGUE_NAMES_FA,
    LEAGUE_NAMES_LEAGUE,
    LEAGUE_NAMES_CHARITY,
    LEAGUE_NAMES_UEFA_SUPER,
    LEAGUE_NAMES_PROMOTION,
    LEAGUE_NAMES_END
};

/* names for the five leagues and the cups */
gchar league_names[LEAGUE_NAMES_END][50];

/* indices of the finances[] global variable:
   money:  the team's money
   ticket:  ticket sale income if the team played at home
   journey:  journey costs if the team played away
   wages:  wages for players
   scout: scout's wage
   stadium: money spent on stadium improvement
   physio: physio costs if players are injured
   transfers: money from/for transfers
   stad_bills: bills for stadium special events
   (e.g crowd violence or fire)
   debts: debts
   prize: prize money at the end of each season for the first 3 teams
   rest: dummies to ensure compatibility of savegames */
enum
{
    FIN_MONEY,
    FIN_TICKETS,
    FIN_JOURNEY,
    FIN_WAGES,
    FIN_SCOUT,
    FIN_PHYSIO,
    FIN_TRANSFERS,
    FIN_STADIUM,
    FIN_STAD_BILLS,
    FIN_PRIZE,
    FIN_DEBTS,
    FIN_DUMMY1,
    FIN_DUMMY2,
    FIN_DUMMY3,
    FIN_DUMMY4,
    FIN_DUMMY5,
    FIN_DUMMY6,
    FIN_DUMMY7,
    FIN_DUMMY8,
    FIN_DUMMY9,
    FIN_DUMMY10,
    FIN_DUMMY11,
    FIN_DUMMY12,
    FIN_DUMMY13,
    FIN_DUMMY14,
    FIN_DUMMY15,
    FIN_DUMMY16,
    FIN_DUMMY17,
    FIN_DUMMY18,
    FIN_DUMMY19,
    FIN_END
};

gint finances[FIN_END];

/* indices of the options[] global variable
   opt_conf*: confirmation options
       new_week: starting a new week
       unfit: warning if there are injured or banned players in the team
       quit: quit without save

   overwrite: whether 'save' overwrites old save file or pops up
              a file selection dialog
   show_my_games: show a summary of the games of the previous week
                 (with injured/booked players, goal scorers etc.)
   show_preview: show the next couple of games at the beginning of
                 the week
   show_live: show live game
   live_duration: duration of a live game
   notify: whether or not transfer list notification is switched on
   notify_*: criteria for notification (age bounds etc.)
   jobs: show job offers from other teams when the
         human player's team is successful
   mess: how messages are displayed */

/* opt_att: indices for the attributes which determine
   the columns that are shown when displaying the player list.
   name: player's name
   pos: position
   cpos: current position
   skill, cskill: likewise
   fit: fitness
   games, goals, age: clear
   etal: estimated talent
   booked: number of yellow cards
   health: if injured, kind of injury and recovery time
   value, wage: clear
   team: team name
   league: team league */

enum
{
    OPT_CONF_NEW_WEEK = 0,
    OPT_CONF_UNFIT,
    OPT_CONF_QUIT,
    OPT_OVERWRITE,
    OPT_SHOW_MY_GAMES,
    OPT_SHOW_LIVE,
    OPT_LIVE_DURATION,
    OPT_LIVE_TENDENCY,
    OPT_NOTIFY,
    OPT_NOTIFY_POS0,
    OPT_NOTIFY_POS1,
    OPT_NOTIFY_POS2,
    OPT_NOTIFY_POS3,
    OPT_NOTIFY_LEAGUE1,
    OPT_NOTIFY_LEAGUE2,
    OPT_NOTIFY_LEAGUE3,
    OPT_NOTIFY_LEAGUE4,
    OPT_NOTIFY_LEAGUE5,
    OPT_NOTIFY_LEAGUE6,
    OPT_NOTIFY_AGE,
    OPT_NOTIFY_SKILL,
    OPT_NOTIFY_ETAL,
    OPT_NOTIFY_VALUE,
    OPT_NOTIFY_POS,
    OPT_NOTIFY_LEAGUE,
    OPT_JOBS,
    OPT_MESS,
    OPT_PREFER_MESS,
    OPT_AUTOSAVE,
    OPT_SKIP_WEEKS,
    OPT_SORT_TRANSFERS,
    OPT_REARRANGE,
    OPT_SWAP,
    OPT_FIT_COLORS,
    OPT_ATT_NAME,
    OPT_ATT_CPOS,
    OPT_ATT_POS,
    OPT_ATT_CSKILL,
    OPT_ATT_SKILL,
    OPT_ATT_TALENT,
    OPT_ATT_FIT,
    OPT_ATT_GAMES,
    OPT_ATT_GOALS,
    OPT_ATT_STATUS,
    OPT_ATT_AGE,
    OPT_ATT_ETAL,
    OPT_ATT_BOOKED,
    OPT_ATT_VALUE,
    OPT_ATT_WAGE,
    OPT_ATT_TEAM,
    OPT_ATT_LEAGUE,
    OPT_DUMMY1,
    OPT_DUMMY2,
    OPT_DUMMY3,
    OPT_DUMMY4,
    OPT_DUMMY5,
    OPT_DUMMY6,
    OPT_DUMMY7,
    OPT_DUMMY8,
    OPT_DUMMY9,
    OPT_DUMMY10,
    OPT_DUMMY11,
    OPT_DUMMY12,
    OPT_DUMMY13,
    OPT_DUMMY14,
    OPT_DUMMY15,
    OPT_DUMMY16,
    OPT_DUMMY17,
    OPT_DUMMY18,
    OPT_DUMMY19,
    OPT_DUMMY20,
    OPT_DUMMY21,
    OPT_DUMMY22,
    OPT_DUMMY23,
    OPT_DUMMY24,
    OPT_DUMMY25,
    OPT_DUMMY26,
    OPT_DUMMY27,
    OPT_DUMMY28,
    OPT_DUMMY29,
    OPT_DUMMY30,
    OPT_DUMMY31,
    OPT_DUMMY32,
    OPT_DUMMY33,
    OPT_DUMMY34,
    OPT_END
};

gint options[OPT_END];

/* indices for the counters[] global variable
   loan: number of weeks until the human player has to pay
         back a part of his loan
   overdrawn: how many times the human player's overdrawn the 
              bank account (too often results in dismissal)
   positive: how many weeks until the bank account has to be positive 
   success: the human player gets points if his team is successful or
            unsuccessful. depending on the points other teams might
	    want to hire him or his own team fires him
   warning: whether a warning that he might be fired has 
            already be shown */

enum
{
    COUNT_LOAN = 0,
    COUNT_OVERDRAWN,
    COUNT_POSITIVE,
    COUNT_SUCCESS,
    COUNT_WARNING,
    COUNT_INC_CAP,
    COUNT_INC_SAF,
    COUNT_AUTOSAVE,
    COUNT_OLD_WAGE1,
    COUNT_OLD_WAGE2,
    COUNT_OLD_WAGE3,
    COUNT_OLD_WAGE4,
    COUNT_OLD_WAGE5,
    COUNT_DUMMY1,
    COUNT_DUMMY2,
    COUNT_DUMMY3,
    COUNT_DUMMY4,
    COUNT_DUMMY5,
    COUNT_DUMMY6,
    COUNT_DUMMY7,
    COUNT_DUMMY8,
    COUNT_DUMMY9,
    COUNT_DUMMY10,
    COUNT_DUMMY11,
    COUNT_DUMMY12,
    COUNT_DUMMY13,
    COUNT_DUMMY14,
    COUNT_DUMMY15,
    COUNT_DUMMY16,
    COUNT_DUMMY17,
    COUNT_DUMMY18,
    COUNT_DUMMY19,
    COUNT_DUMMY20,
    COUNT_DUMMY31,
    COUNT_DUMMY32,
    COUNT_END
};

gint counters[COUNT_END];

/* transfer list sorting criterium */
enum
{
    SORT_POS = 1,
    SORT_SKILL,
    SORT_AGE,
    SORT_ETAL,
    SORT_LEAGUE,
    SORT_END
};

/* player attributes */
enum
{
    ATT_NAME = 0,
    ATT_CPOS,
    ATT_POS,
    ATT_CSKILL,
    ATT_SKILL,
    ATT_TALENT,
    ATT_FIT,
    ATT_GAMES,
    ATT_GOALS,
    ATT_STATUS,
    ATT_AGE,
    ATT_ETAL,
    ATT_BOOKED,
    ATT_VALUE,
    ATT_WAGE,
    ATT_TEAM,
    ATT_LEAGUE,
    ATT_DUMMY1,
    ATT_DUMMY2,
    ATT_DUMMY3,
    ATT_DUMMY4,
    ATT_END
};

/* string indices for the job offer window */
enum
{
    JOB_OFFER_TEXT = 0,
    JOB_OFFER_NAME,
    JOB_OFFER_LEAGUE,
    JOB_OFFER_RANK,
    JOB_OFFER_MONEY,
    JOB_OFFER_CAP,
    JOB_OFFER_SAF,
    JOB_OFFER_ACCEPT,
    JOB_OFFER_END
};

