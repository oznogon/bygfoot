## Process this file with automake to produce Makefile.in
AM_CFLAGS = -Wall

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = bygfoot

bygfoot_SOURCES = \
	callback_func.c callback_func.h callbacks.h cup.h finance.h fixture.h game_gui.h league.h live_game.h maths.h misc.h option.h player.h start_end.h team.h transfer.h treeview.h user.h window.h \
	callbacks.c callbacks.h callback_func.h debug.h free.h game.h game_gui.h gui.h load_save.h main.h option.h player.h team.h transfer.h treeview.h treeview_helper.h user.h window.h \
	cup.c cup.h fixture.h free.h league.h main.h maths.h misc.h option.h team.h variables.h xml_league.h \
	debug.c debug.h game_gui.h option.h user.h variables.h \
	file.c file.h free.h main.h misc.h option.h support.h variables.h \
	finance.c callbacks.h finance.h game_gui.h maths.h option.h player.h team.h user.h \
	fixture.c cup.h fixture.h free.h league.h main.h maths.h misc.h option.h user.h table.h team.h variables.h \
	free.c free.h user.h variables.h \
	game.c cup.h finance.h fixture.h game.h game_gui.h league.h live_game.h maths.h misc.h option.h player.h table.h team.h treeview.h user.h variables.h \
	game_gui.c file.h callbacks.h file.h game_gui.h gui.h league.h live_game.h maths.h misc.h option.h treeview.h support.h team.h user.h variables.h window.h \
	gui.c gui.h misc.h support.h variables.h window.h \
	interface.c callbacks.h interface.h support.h \
	lg_commentary.c free.h lg_commentary.h live_game.h misc.h option.h player.h variables.h \
	league.c cup.h league.h maths.h misc.h option.h player.h stat.h table.h team.h user.h variables.h \
	live_game.c fixture.h free.h game.h game_gui.h lg_commentary.h live_game.h maths.h misc_callback_func.h option.h player.h support.h team.h treeview.h user.h variables.h window.h \
	load_save.c callbacks.h file.h game_gui.h gui.h load_save.h option.h support.h user.h variables.h xml_loadsave_misc.h xml_loadsave_cup.h xml_loadsave_league.h xml_loadsave_season_stats.h xml_loadsave_transfers.h xml_loadsave_users.h xml.h \
	main.c cup_struct.h misc_callbacks.h file.h free.h league_struct.h live_game.h main.h stat_struct.h transfer_struct.h variables.h window.h \
	maths.c maths.h misc.h \
	misc.c main.h maths.h misc.h \
	misc_callbacks.c callbacks.h callback_func.h game.h game_gui.h live_game.h load_save.h main.h misc_callback_func.h misc_callbacks.h option.h user.h variables.h window.h xml_country.h \
	misc_callback_func.c callbacks.h finance.h game.h game_gui.h gui.h load_save.h maths.h misc_callback_func.h option.h start_end.h support.h treeview.h user.h variables.h window.h xml_country.h xml_name.h \
	misc_interface.c  misc_interface.h misc_callbacks.h support.h \
	misc2_callbacks.c callback_func.h debug.h finance.h game_gui.h load_save.h main.h misc2_callbacks.h misc2_callback_func.h misc2_interface.h player.h support.h transfer.h treeview.h user.h window.h \
	misc2_callback_func.c file.h finance.h game_gui.h maths.h misc2_callback_func.h option.h player.h support.h team.h treeview.h transfer.h user.h window.h \
	misc2_interface.c  misc2_interface.h misc2_callbacks.h support.h \
	option.c option.h variables.h \
	option_gui.c file.h game_gui.h callbacks.h option_gui.h option.h support.h user.h variables.h \
	options_callbacks.c file.h options_callbacks.h options_interface.h option_gui.h support.h user.h variables.h window.h \
	options_interface.c  options_interface.h options_callbacks.h support.h \
	player.c cup.h fixture.h free.h game_gui.h league.h maths.h misc.h option.h player.h team.h user.h \
	start_end.c cup.h file.h finance.h fixture.h free.h game_gui.h gui.h league.h load_save.h live_game.h main.h maths.h misc.h option.h start_end.h stat.h table.h team.h transfer.h user.h variables.h xml_name.h \
	stat.c cup.h free.h league.h option.h player.h stat.h table_struct.h team.h variables.h \
	support.c support.h \
	table.c cup.h league.h table.h variables.h \
	team.c cup.h fixture.h game.h game_gui.h league.h maths.h misc.h option.h player.h team.h transfer.h user.h \
	transfer.c cup.h finance.h free.h game_gui.h maths.h option.h player.h support.h team.h transfer.h treeview.h user.h \
	treeview.c cup.h finance.h fixture.h free.h game.h league.h live_game.h maths.h misc.h player.h option.h support.h team.h transfer.h treeview.h treeview_helper.h user.h \
	treeview_helper.c cup.h file.h fixture.h free.h league.h misc.h option.h player.h support.h team.h treeview.h treeview_helper.h user.h variables.h \
	user.c cup.h fixture.h free.h game_gui.h league.h live_game.h maths.h misc.h option.h player.h team.h transfer.h treeview.h user.h window.h \
	window.c file.h finance.h free.h game_gui.h gui.h interface.h main.h misc_interface.h misc2_interface.h option.h support.h treeview.h treeview_helper.h user.h window.h \
	xml.c cup.h file.h free.h gui.h league.h misc.h option.h support.h table.h transfer_struct.h user.h variables.h xml.h xml_loadsave_cup.h xml_loadsave_league.h xml_loadsave_teams.h xml_loadsave_fixtures.h xml_loadsave_table.h xml_loadsave_transfers.h xml_loadsave_users.h \
	xml_loadsave_misc.c cup.h file.h misc.h variables.h xml.h xml_loadsave_misc.h xml_loadsave_cup.h xml_loadsave_league.h \
	xml_loadsave_cup.c cup.h file.h misc.h table.h team.h xml.h xml_loadsave_cup.h xml_loadsave_fixtures.h xml_loadsave_table.h xml_loadsave_teams.h \
	xml_loadsave_fixtures.c file.h fixture.h misc.h team.h xml.h xml_loadsave_fixtures.h \
	xml_loadsave_league.c file.h league.h misc.h xml.h xml_loadsave_fixtures.h xml_loadsave_league.h xml_loadsave_table.h xml_loadsave_teams.h \
	xml_loadsave_league_stat.c file.h misc.h stat.h team.h xml_loadsave_league_stat.h xml.h \
	xml_loadsave_live_game.c cup.h file.h fixture.h league.h live_game.h misc.h option.h variables.h xml.h xml_loadsave_live_game.h \
	xml_loadsave_season_stats.c file.h misc.h stat.h variables.h xml.h xml_loadsave_league_stat.h xml_loadsave_season_stats.h \
	xml_loadsave_table.c file.h misc.h team.h xml.h xml_loadsave_table.h \
	xml_loadsave_teams.c file.h misc.h player.h team.h xml.h xml_loadsave_teams.h \
	xml_loadsave_transfers.c file.h misc.h team.h transfer.h xml.h xml_loadsave_transfers.h \
	xml_loadsave_users.c file.h misc.h team.h user.h variables.h xml.h xml_loadsave_users.h \
	xml_cup.c cup.h file.h main.h misc.h option.h variables.h xml_cup.h \
	xml_country.c file.h free.h misc.h variables.h xml_cup.h xml_country.h xml_league.h \
	xml_league.c cup_struct.h file.h free.h league.h misc.h team.h table.h variables.h xml_league.h xml_cup.h \
	xml_name.c file.h free.h maths.h misc.h variables.h xml_name.h

bygfoot_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

