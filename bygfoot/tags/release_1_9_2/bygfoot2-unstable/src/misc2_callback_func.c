#include "callbacks.h"
#include "callback_func.h"
#include "file.h"
#include "finance.h"
#include "free.h"
#include "game_gui.h"
#include "maths.h"
#include "misc.h"
#include "misc2_callback_func.h"
#include "option.h"
#include "player.h"
#include "support.h"
#include "team.h"
#include "treeview.h"
#include "treeview2.h"
#include "treeview_helper.h"
#include "transfer.h"
#include "user.h"
#include "window.h"

/** Transfer a user player. */
void
misc2_callback_transfer_user_player(void)
{
    Team *new_team = transoff(stat2, 0).tm;

    if(team_is_user(new_team) != -1)
    {
	if(new_team->players->len < const_int("int_team_max_players") &&
	   BUDGET(team_is_user(new_team)) >= transoff(stat2, 0).fee)
	{
	    current_user.money += transoff(stat2, 0).fee;
	    current_user.money_in[1][MON_IN_TRANSFERS] += transoff(stat2, 0).fee;
	    user_from_team(new_team)->money -= transoff(stat2, 0).fee;
	    user_from_team(new_team)->money_out[1][MON_OUT_TRANSFERS] -= 
		transoff(stat2, 0).fee;
	    player_copy(player_of_id_team(trans(stat2).tm, trans(stat2).id),
			new_team, new_team->players->len);
	    player_of_idx_team(new_team, new_team->players->len - 1)->contract = 
		(gfloat)math_rndi(const_int("int_transfer_contract_lower"),
				  const_int("int_transfer_contract_upper"));
	    player_of_idx_team(new_team, new_team->players->len - 1)->wage = 
		transoff(stat2, 0).wage;
	    g_array_remove_index(current_user.tm->players, 
				 player_id_index(current_user.tm, trans(stat2).id));
	    transfer_remove_player(stat2);
	}
	else
	{
	    game_gui_show_warning(_("%s couldn't afford to buy %s or his roster was full."),
				  user_from_team(new_team)->name, 
				  player_of_id_team(trans(stat2).tm, trans(stat2).id)->name);
	    user_event_add(user_from_team(new_team), EVENT_TYPE_WARNING, -1, -1, NULL, 
			   _("You didn't have enough money to buy %s or your roster was full."),
			   player_of_id_team(trans(stat2).tm, trans(stat2).id)->name);
	    g_array_remove_index(trans(stat2).offers, 0);
	    if(trans(stat2).offers->len > 0 && 
	       transoff(stat2, 0).status == TRANSFER_OFFER_NOT_CONSIDERED)
		transoff(stat2, 0).status = TRANSFER_OFFER_ACCEPTED;
	}
    }
    else
    {		
	current_user.money += transoff(stat2, 0).fee;
	current_user.money_in[1][MON_IN_TRANSFERS] += transoff(stat2, 0).fee;
	player_remove_from_team(current_user.tm, 
				player_id_index(current_user.tm, trans(stat2).id));
    }

    treeview_show_user_player_list();
    game_gui_set_main_window_header();
    on_button_transfers_clicked(NULL, NULL);    
}

/** Transfer a cpu player to a user team. */
void
misc2_callback_transfer_cpu_player(void)
{
    if(current_user.tm->players->len > const_int("int_team_max_players") ||
       BUDGET(cur_user) < transoff(stat2, 0).fee)
    {
	game_gui_show_warning(_("Your player roster is full or you don't enough money."));
	return;
    }	

    player_copy(player_of_id_team(trans(stat2).tm, trans(stat2).id),
		current_user.tm, current_user.tm->players->len);
    player_of_idx_team(current_user.tm, current_user.tm->players->len - 1)->contract =
	(gfloat)math_rndi(const_int("int_transfer_contract_lower"),
			  const_int("int_transfer_contract_upper"));
    player_of_idx_team(current_user.tm, current_user.tm->players->len - 1)->wage =
	transoff(stat2, 0).wage;
    user_from_team(current_user.tm)->money -= transoff(stat2, 0).fee;
    user_from_team(current_user.tm)->money_out[1][MON_OUT_TRANSFERS] -=
	transoff(stat2, 0).fee;
    player_replace_by_new(player_of_id_team(trans(stat2).tm, trans(stat2).id), FALSE);
    transfer_remove_player(stat2);

    treeview_show_user_player_list();
    game_gui_set_main_window_header();

    if(week < transfer_get_deadline() || transfer_list->len > 0 )
	on_button_transfers_clicked(NULL, NULL);
    else
	on_button_back_to_main_clicked(NULL, NULL);
}

/** Change the user team's structure to a value he's specified. */
gboolean
misc2_callback_change_structure(gint structure)
{
    gint poss_struct = team_find_appropriate_structure(current_user.tm);

    if(math_get_place(structure, 1) + math_get_place(structure, 2) + 
       math_get_place(structure, 3) != 
       math_get_place(poss_struct, 1) + math_get_place(poss_struct, 2) + 
       math_get_place(poss_struct, 3))
    {
	game_gui_show_warning(_("The structure value %d is invalid."), structure);
	return FALSE;
    }

    team_change_structure(current_user.tm, structure);
    treeview_show_user_player_list();

    setsav0;

    return TRUE;
}

/** Handle a click on the contract offer button. */
void
misc2_callback_contract_offer(void)
{
    gint i;
    gchar buf[SMALL];
    GtkSpinButton *spinbutton;
    gint value = 0;
    Player *pl = (Player*)statp;

    gtk_widget_set_sensitive(lookup_widget(window.contract, "button_contract_cancel"), FALSE);

    for(i=0;i<4;i++)
    {
	sprintf(buf, "radiobutton_contract%d", i + 1);

	if(gtk_toggle_button_get_active(
	       GTK_TOGGLE_BUTTON(lookup_widget(window.contract, buf))))
	{
	    sprintf(buf, "spinbutton_contract%d", i + 1);
	    spinbutton = GTK_SPIN_BUTTON(lookup_widget(window.contract, buf));
	    value = gtk_spin_button_get_value_as_int(spinbutton);

	    if(value >= (gint)rint((gfloat)stat1 * 
				   (1 + (i * const_float("float_contract_scale_factor") *
					 powf(-1, (pl->age > pl->peak_age))))))
	    {
		pl->contract += (i + 1);
		pl->offers = 0;
		pl->wage = value;
		game_gui_show_warning(_("%s accepts your offer."), pl->name);
		window_destroy(&window.contract, FALSE);
	    }
	    else
	    {
		pl->offers++;
		if(pl->offers < const_int("int_contract_max_offers"))
		    game_gui_show_warning(_("%s rejects your offer. You may still make %d offers."), 
					  pl->name, 
					  const_int("int_contract_max_offers") - pl->offers);
		else
		{
		    game_gui_show_warning(_("%s rejects your offer and won't negotiate with you anymore. You should sell him before his contract expires (he'll simply leave your team otherwise)."), 
					  pl->name);
		    window_destroy(&window.contract, FALSE);
		}
	    }

	    break;
	}
    }
}

/** Add a user to the users array. */
void
misc2_callback_add_user(void)
{
    GtkTreeView *treeview_user_management_teams =
	GTK_TREE_VIEW(lookup_widget(window.user_management, "treeview_user_management_teams"));
    GtkEntry *entry_user_management = 
	GTK_ENTRY(lookup_widget(window.user_management, "entry_user_management"));
    const gchar *user_name = gtk_entry_get_text(entry_user_management);
    User new_user = user_new();
    Team *tm = (Team*)treeview_helper_get_pointer(treeview_user_management_teams, 2);
    
    if(strlen(user_name) > 0)
	misc_string_assign(&new_user.name, user_name);
    
    gtk_entry_set_text(entry_user_management, "");

    new_user.tm = tm;
    new_user.team_id = tm->id;

    user_history_add(&new_user, USER_HISTORY_START_GAME, tm->id, tm->clid, -1, "");

    g_array_append_val(users, new_user);

    user_set_up_team(&usr(users->len - 1));

    file_load_user_conf_file(&usr(users->len - 1));

    treeview_show_users(GTK_TREE_VIEW(lookup_widget(window.user_management, "treeview_user_management_users")));
    treeview_show_team_list(treeview_user_management_teams, FALSE, FALSE);

    setsav0;
}

/** Handle a click in the MM window treeview.
 @param widget The treeview widget.
 @param row_num The row that's been clicked on.
 @param col_num The column number. */
void
misc2_callback_mmatches_button_press(GtkWidget *widget, gint row_num, gint col_num)
{    
    if(row_num < current_user.mmatches->len)
    {
	if(col_num == TREEVIEW_MMATCH_COL_REPLAY)
	{
	    stat1 = STATUS_SHOW_LAST_MATCH;
	    stat3 = 0;
	    callback_show_last_match(TRUE, 
				     &g_array_index(current_user.mmatches, MemMatch, row_num).lg);
	}
	else if(col_num == TREEVIEW_MMATCH_COL_REMOVE)
	{
	    gtk_widget_hide(widget);
	    free_g_string(&g_array_index(current_user.mmatches, MemMatch, row_num).competition_name);
	    free_gchar_ptr(g_array_index(current_user.mmatches, MemMatch, row_num).country_name);
	    free_live_game(&g_array_index(current_user.mmatches, MemMatch, row_num).lg);
	    g_array_remove_index(current_user.mmatches, row_num);
	    treeview2_show_mmatches();
	    gtk_widget_show(widget);
	}
	else if(col_num == TREEVIEW_MMATCH_COL_EXPORT)
	{
	    stat5 = STATUS_SELECT_MM_FILE_EXPORT;
	    stat4 = row_num;
	    window_show_file_sel();
	}
    }
    else if(row_num == current_user.mmatches->len && col_num == 1)
    {
	if(current_user.live_game.units->len == 0)
	{
	    game_gui_show_warning(_("No match stored."));
	    return;
	}

	user_mm_add_last_match(FALSE, FALSE);
	treeview2_show_mmatches();
    }
}
