## Process this file with automake to produce Makefile.in
AM_CFLAGS = -Wall
INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = bygfoot

bygfoot_SOURCES = \
	callbacks.c callbacks.h defs.h \
	callback_func.c callback_func.h callbacks.h support.h \
	editor_callbacks.c editor_callbacks.h  editor_defs.h \
	editor_callback_func.c  defs.h editor_defs.h \
	editor_interface.c  \
	fixtures.c defs.h \
	games.c defs.h \
	generation.c defs.h \
	gui_functions.c gui_functions.h callbacks.h support.h \
	interface.c interface.h support.h \
	load_save.c defs.h \
	main.c main.h defs.h \
	math_functions.c defs.h \
	misc.c defs.h \
	misc_callbacks.c misc_callbacks.h  defs.h \
	misc_interface.c  \
	misc2_callbacks.c misc2_callbacks.h  defs.h \
	misc2_interface.c  \
	options_callbacks.c options_callbacks.h  \
	options_interface.c  \
	player_func.c defs.h \
	startup.c startup.h  defs.h \
	support.c support.h \
	team_func.c team_func.h defs.h \
	text_files.c defs.h \
	treeview_cell.c treeview_cell.h defs.h \
	treeview_func.c treeview_func.h defs.h \
	windows.c interface.h misc_interface.h options_interface.h editor_interface.h defs.h windows.h editor_defs.h 

bygfoot_LDADD = @PACKAGE_LIBS@ $(INTLLIBS)

