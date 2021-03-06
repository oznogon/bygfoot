#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "bygfoot.h"
#include "variables.h"

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

void
on_shoots_penalties_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_button_quit_clicked                 (GtkButton       *button,
                                        gpointer         user_data);
void
on_menu_new_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_start_editor_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
void
on_button_back_to_main_clicked         (GtkButton       *button,
                                        gpointer         user_data);
void
on_button_preview_clicked              (GtkButton       *button,
                                        gpointer         user_data);
void
on_button_new_week_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_start_update_activate               (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_player_list2_button_press_event     (GtkWidget       *widget,
                                        GdkEventButton  *event,
                                        gpointer         user_data);

void
on_show_statistics_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_optionmenu_finstad_changed          (GtkOptionMenu   *optionmenu,
                                        gpointer         user_data);

void
on_checkbutton_boost_toggled           (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_figures_fixtures_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_figures_tables_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);
#endif
