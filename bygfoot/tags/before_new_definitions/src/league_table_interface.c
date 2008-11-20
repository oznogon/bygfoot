/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "league_table_callbacks.h"
#include "league_table_interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_window_league_table (void)
{
  GtkWidget *window_league_table;
  GtkWidget *panel_table;
  GtkWidget *l_headline;
  GtkWidget *hs_table2;
  GtkWidget *hs_table1;
  GtkWidget *b_ok;

  window_league_table = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window_league_table), _("League table"));
  gtk_window_set_position (GTK_WINDOW (window_league_table), GTK_WIN_POS_CENTER);
  gtk_window_set_modal (GTK_WINDOW (window_league_table), TRUE);
  gtk_window_set_resizable (GTK_WINDOW (window_league_table), FALSE);

  panel_table = gtk_fixed_new ();
  gtk_widget_show (panel_table);
  gtk_container_add (GTK_CONTAINER (window_league_table), panel_table);
  gtk_widget_set_size_request (panel_table, 500, 290);

  l_headline = gtk_label_new (_("Headline"));
  gtk_widget_show (l_headline);
  gtk_fixed_put (GTK_FIXED (panel_table), l_headline, 208, 32);
  gtk_widget_set_size_request (l_headline, 90, 20);

  hs_table2 = gtk_hseparator_new ();
  gtk_widget_show (hs_table2);
  gtk_fixed_put (GTK_FIXED (panel_table), hs_table2, 25, 88);
  gtk_widget_set_size_request (hs_table2, 450, 16);

  hs_table1 = gtk_hseparator_new ();
  gtk_widget_show (hs_table1);
  gtk_fixed_put (GTK_FIXED (panel_table), hs_table1, 25, 235);
  gtk_widget_set_size_request (hs_table1, 450, 16);

  b_ok = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (b_ok);
  gtk_fixed_put (GTK_FIXED (panel_table), b_ok, 376, 254);
  gtk_widget_set_size_request (b_ok, 110, 25);

  g_signal_connect ((gpointer) window_league_table, "delete_event",
                    G_CALLBACK (on_window_league_table_delete_event),
                    NULL);
  g_signal_connect ((gpointer) b_ok, "clicked",
                    G_CALLBACK (on_lt_b_ok_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window_league_table, window_league_table, "window_league_table");
  GLADE_HOOKUP_OBJECT (window_league_table, panel_table, "panel_table");
  GLADE_HOOKUP_OBJECT (window_league_table, l_headline, "l_headline");
  GLADE_HOOKUP_OBJECT (window_league_table, hs_table2, "hs_table2");
  GLADE_HOOKUP_OBJECT (window_league_table, hs_table1, "hs_table1");
  GLADE_HOOKUP_OBJECT (window_league_table, b_ok, "b_ok");

  return window_league_table;
}

