## Process this file with automake to produce Makefile.in
AM_CFLAGS = -I/usr/include/libxml2 -Wall
INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = bygfoot

bygfoot_SOURCES = \
	callback_func.c callback_func.h callbacks.h game_gui.h game.h gui.h finance.h fixture.h math.h misc.h player.h team.h text_files.h transfer.h treeview.h support.h window.h \
	callbacks.c callbacks.h callback_func.h fixture.h gui.h game_gui.h load_save.h math.h misc.h player.h team.h treeview.h startup.h support.h window.h \
	editor_callback_func.c editor_bygfoot.h callback_func.h editor_callback_func.h gui.h math.h misc.h player.h startup.h support.h team.h text_files.h treeview.h window.h \
	editor_callbacks.c editor_defs.h editor_callback_func.h editor_callbacks.h support.h window.h \
	editor_interface.c  editor_callbacks.h editor_interface.h support.h \
	finance.c finance.h fixture.h math.h team.h \
	fixture.c game.h fixture.h math.h misc.h team.h \
	game.c fixture.h game.h math.h team.h \
	game_gui.c callback_func.h callbacks.h game_gui.h game.h generation.h gui.h finance.h fixture.h load_save.h math.h misc.h player.h support.h team.h transfer.h treeview.h treeview_cell.h window.h \
	generation.c game_gui.h generation.h gui.h math.h misc.h player.h team.h text_files.h window.h \
	gui.c callbacks.h gui.h math.h misc.h support.h team.h treeview.h \
	interface.c callbacks.h interface.h support.h \
	load_save.c game_gui.h load_save.h misc.h xml_read.h xml_write.h \
	main.c callbacks.h generation.h gui.h load_save.h main.h startup.h support.h text_files.h window.h \
	maths.c maths.h \
	misc.c misc.h math.h \
	misc_callbacks.c callback_func.h callbacks.h editor_callback_func.h game_gui.h generation.h gui.h load_save.h math.h misc_callbacks.h player.h startup.h support.h team.h text_files.h window.h \
	misc_interface.c  misc_interface.h misc_callbacks.h support.h \
	misc2_callbacks.c callback_func.h callbacks.h gui.h misc2_callbacks.h support.h team.h \
	misc2_interface.c  misc2_interface.h misc2_callbacks.h support.h \
	options_callbacks.c callback_func.h callbacks.h gui.h options_callbacks.h support.h treeview.h window.h \
	options_interface.c  options_interface.h options_callbacks.h support.h \
	player.c callbacks.h finance.h math.h player.h team.h window.h \
	startup.c callbacks.h fixture.h generation.h math.h misc.h startup.h support.h team.h text_files.h treeview.h window.h \
	support.c support.h \
	team.c finance.h fixture.h game_gui.h math.h misc.h player.h team.h text_files.h window.h \
	text_files.c gui.h math.h misc.h player.h team.h text_files.h window.h \
	transfer.c math.h misc.h player.h team.h transfer.h \
	treeview.c finance.h fixture.h math.h misc.h support.h team.h treeview_cell.h treeview.h \
	treeview_cell.c fixture.h game.h gui.h math.h misc.h team.h treeview_cell.h \
	window.c callback_func.h editor_defs.h editor_callback_func.h finance.h gui.h interface.h math.h misc.h misc_interface.h misc2_interface.h options_interface.h editor_interface.h support.h text_files.h treeview.h window.h \
	xml_read.c xml_read.h \
	xml_write.c xml_write.h

bygfoot_LDADD = @PACKAGE_LIBS@ $(INTLLIBS) -lxml2

