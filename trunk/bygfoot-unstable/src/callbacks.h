#include "defs.h"

/* show a popup telling the user that some players' careers
   had to stop because of a severe injury */
void
callback_notify_injury(void);

/* fire the selected player */
void
callback_fire_player(GtkTreeSelection *selection,
		     GdkEventButton *event);

/* remove the selected player from the transfer list */
void
callback_rm_pl_transfer(void);

/* show the window with the stadium capacity increase options */
void
show_stadium_window(void);

/* show player list of the opposing team */
void
callback_show_opponent_team(gint row_idx);

/* undo the swapping of two players */
void
callback_undo(void);

/* pick a new structure from the list of common structures
   for the human player's team */
void
cycle_through_structures(void);

/* get the next week from 'week' up- or downwards
   featuring games of the human player's team */
gint
get_my_fixture_week(gint week_number, gint direction);

/* show the history of the given season */
void
callback_show_history(gint season_number);

/* return 1 if there are injured/banned players
   in the human player's team, 0 otherwise */
gint
unfit_players(void);

/* show the best 10 goal scorers and goalkeepers
   in the league */
void
callback_show_best_players(gint league);

/* get or pay back loan */
gint
callback_get_loan(void);
gint
callback_pay_loan(void);
gint
callback_get_the_loan(void);
gint
callback_pay_the_loan(void);

/* improve stadium capacity or safety */
void
callback_stadium_improve(void);

/* show finances and stadium info */
void
show_fin_stad(void);

/* show tables of an english division or CL group */
void
show_tables_league(gint league);

/* show the human player's league results of the season
   by team */
void
show_league_results(void);

/* show the fixtures for the given week and leagues */
void
show_fixtures(gint week_number);

/* find out which player's been clicked on and
   show the detailed info */
void
callback_show_player_info(GtkTreeSelection *selection,
			       GdkEventButton *event);

/* put a player onto the transfer list when the user
   double-clicked */
void
callback_activate_player(GtkTreeSelection *selection,
			      GdkEventButton *event);

/* handle a left-click on the player-list.
   swap two players, if necessary */
void
callback_select_player(GtkTreeSelection *selection,
			    GdkEventButton *event);

/* handle a left-click on the player_list */
gint
callback_transfer_select(gint row_idx);

/* handle the click on the 'ok' button the human player
   makes a transfer offer with */
void
callback_make_transfer_offer(GtkWidget *widget);

/* handle a click on the 'browse teams' button */
void
callback_transfer_team_select(gint row_idx);

/* show the next or previous team in browse mode */
void
callback_transfers_browse_teams(gint direction);

/* handle a left-click in the player_info when 
   the human player browses the teams to buy players */
void
callback_transfer_buy_player(gint row_idx);

/* display the fixtures in the leagues the human player's team
   participates in */
void
callback_show_fixtures(gint week_number);

/* show the table of the specified league */
void
callback_show_tables(gint league);

/* show the human player's league results of the season
   by team */
void
callback_show_league_results(void);

/* show the options window */
void
show_opt_window(void);

void
on_menu_open_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_save_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_save_as_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_options_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_quit_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_menu_about_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_style_aodefend_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_style_defend_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_style_balanced_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_style_attack_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_style_aoattack_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_player_list_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_player_list_selection_get           (GtkWidget       *widget,
                                        GtkSelectionData *data,
                                        guint            info,
                                        guint            time,
                                        gpointer         user_data);

void
on_scout_best_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_scout_good_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_scout_average_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_scout_bad_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_style_optionmenu_changed            (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);

void
on_structure_entry_activate            (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_scout_optionmenu_changed            (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);

void
on_structure_button_clicked            (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_player_list_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_button_rearrange_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_transfers_clicked           (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_player_info_button_press_event      (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_button_transfer_ok_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_browse_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_browse_back_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_browse_forward_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_optionmenu_figures_changed          (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);

void
on_button_fix_team_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_league_results_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_fin_stad_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_stad_capacity_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_stad_safety_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_get_loan_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_pay_loan_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_spin_fee_activate                   (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_button_options_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_load_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_save_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_help_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_help_close_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_fixtures_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_tables_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_optionmenu_quick_opt_changed        (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);
void
on_button_select_country_file_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_radiobutton_country_toggled        (GtkToggleButton *togglebutton,
				       gpointer         user_data);

void
on_button_undo_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_optionmenu_fixtures_changed         (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);

void
on_menu_team_editor_help_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_optionmenu_physio_changed           (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);

void
on_show_info_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_put_on_transfer_list_activate       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_remove_from_transfer_list_activate  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_fire_activate                       (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
