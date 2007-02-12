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

#include "training_callbacks.h"
#include "training_interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_window_training_camp (void)
{
  GtkWidget *window_training_camp;
  GtkWidget *panel_camp;
  GtkWidget *tf_costs;
  GtkWidget *hs_camp1;
  GtkWidget *b_cancel;
  GtkWidget *b_ok;
  GtkWidget *hs_camp2;
  GtkWidget *l_camp_points;
  GtkWidget *b_inc_recreation;
  GtkWidget *image2;
  GtkWidget *l_recreation;
  GtkWidget *l_costs;
  GtkWidget *rb_camp2;
  GSList *rb_camp2_group = NULL;
  GtkWidget *rb_camp3;
  GtkWidget *b_dec_training;
  GtkWidget *image3;
  GtkWidget *l_training;
  GtkWidget *b_inc_training;
  GtkWidget *image4;
  GtkWidget *hs_camp_points;
  GtkWidget *hs_training;
  GtkWidget *hs_recreation;
  GtkWidget *b_dec_recreation;
  GtkWidget *image1;
  GtkWidget *rb_camp1;

  window_training_camp = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window_training_camp), _("Training camp"));
  gtk_window_set_position (GTK_WINDOW (window_training_camp), GTK_WIN_POS_CENTER);
  gtk_window_set_modal (GTK_WINDOW (window_training_camp), TRUE);
  gtk_window_set_resizable (GTK_WINDOW (window_training_camp), FALSE);

  panel_camp = gtk_fixed_new ();
  gtk_widget_show (panel_camp);
  gtk_container_add (GTK_CONTAINER (window_training_camp), panel_camp);
  gtk_widget_set_size_request (panel_camp, 500, 290);

  tf_costs = gtk_entry_new ();
  gtk_widget_show (tf_costs);
  gtk_fixed_put (GTK_FIXED (panel_camp), tf_costs, 336, 40);
  gtk_widget_set_size_request (tf_costs, 130, 22);
  GTK_WIDGET_UNSET_FLAGS (tf_costs, GTK_CAN_FOCUS);
  gtk_editable_set_editable (GTK_EDITABLE (tf_costs), FALSE);
  gtk_entry_set_has_frame (GTK_ENTRY (tf_costs), FALSE);
  gtk_entry_set_invisible_char (GTK_ENTRY (tf_costs), 9679);

  hs_camp1 = gtk_hseparator_new ();
  gtk_widget_show (hs_camp1);
  gtk_fixed_put (GTK_FIXED (panel_camp), hs_camp1, 25, 235);
  gtk_widget_set_size_request (hs_camp1, 450, 16);

  b_cancel = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (b_cancel);
  gtk_fixed_put (GTK_FIXED (panel_camp), b_cancel, 376, 254);
  gtk_widget_set_size_request (b_cancel, 110, 25);

  b_ok = gtk_button_new_from_stock ("gtk-apply");
  gtk_widget_show (b_ok);
  gtk_fixed_put (GTK_FIXED (panel_camp), b_ok, 256, 254);
  gtk_widget_set_size_request (b_ok, 110, 25);

  hs_camp2 = gtk_hseparator_new ();
  gtk_widget_show (hs_camp2);
  gtk_fixed_put (GTK_FIXED (panel_camp), hs_camp2, 25, 90);
  gtk_widget_set_size_request (hs_camp2, 450, 16);

  l_camp_points = gtk_label_new (_("Camp points"));
  gtk_widget_show (l_camp_points);
  gtk_fixed_put (GTK_FIXED (panel_camp), l_camp_points, 24, 192);
  gtk_widget_set_size_request (l_camp_points, 140, 20);

  b_inc_recreation = gtk_button_new ();
  gtk_widget_show (b_inc_recreation);
  gtk_fixed_put (GTK_FIXED (panel_camp), b_inc_recreation, 425, 125);
  gtk_widget_set_size_request (b_inc_recreation, 30, 26);

  image2 = create_pixmap (window_training_camp, "increase_green.png");
  gtk_widget_show (image2);
  gtk_container_add (GTK_CONTAINER (b_inc_recreation), image2);

  l_recreation = gtk_label_new (_("Recreation"));
  gtk_widget_show (l_recreation);
  gtk_fixed_put (GTK_FIXED (panel_camp), l_recreation, 24, 128);
  gtk_widget_set_size_request (l_recreation, 140, 20);

  l_costs = gtk_label_new (_("Costs"));
  gtk_widget_show (l_costs);
  gtk_fixed_put (GTK_FIXED (panel_camp), l_costs, 255, 41);
  gtk_widget_set_size_request (l_costs, 90, 20);

  rb_camp2 = gtk_radio_button_new_with_mnemonic (NULL, _("First-Class Hotel"));
  gtk_widget_show (rb_camp2);
  gtk_fixed_put (GTK_FIXED (panel_camp), rb_camp2, 24, 40);
  gtk_widget_set_size_request (rb_camp2, 200, 21);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rb_camp2), rb_camp2_group);
  rb_camp2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rb_camp2));

  rb_camp3 = gtk_radio_button_new_with_mnemonic (NULL, _("Premium Hotel"));
  gtk_widget_show (rb_camp3);
  gtk_fixed_put (GTK_FIXED (panel_camp), rb_camp3, 24, 64);
  gtk_widget_set_size_request (rb_camp3, 200, 21);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rb_camp3), rb_camp2_group);
  rb_camp2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rb_camp3));

  b_dec_training = gtk_button_new ();
  gtk_widget_show (b_dec_training);
  gtk_fixed_put (GTK_FIXED (panel_camp), b_dec_training, 176, 157);
  gtk_widget_set_size_request (b_dec_training, 30, 26);

  image3 = create_pixmap (window_training_camp, "decrease_red.png");
  gtk_widget_show (image3);
  gtk_container_add (GTK_CONTAINER (b_dec_training), image3);

  l_training = gtk_label_new (_("Training"));
  gtk_widget_show (l_training);
  gtk_fixed_put (GTK_FIXED (panel_camp), l_training, 24, 160);
  gtk_widget_set_size_request (l_training, 140, 20);

  b_inc_training = gtk_button_new ();
  gtk_widget_show (b_inc_training);
  gtk_fixed_put (GTK_FIXED (panel_camp), b_inc_training, 425, 157);
  gtk_widget_set_size_request (b_inc_training, 30, 26);

  image4 = create_pixmap (window_training_camp, "increase_green.png");
  gtk_widget_show (image4);
  gtk_container_add (GTK_CONTAINER (b_inc_training), image4);

  hs_camp_points = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 1, 10, 1, 0, 0)));
  gtk_widget_show (hs_camp_points);
  gtk_fixed_put (GTK_FIXED (panel_camp), hs_camp_points, 220, 184);
  gtk_widget_set_size_request (hs_camp_points, 200, 36);
  GTK_WIDGET_UNSET_FLAGS (hs_camp_points, GTK_CAN_FOCUS);
  gtk_scale_set_value_pos (GTK_SCALE (hs_camp_points), GTK_POS_RIGHT);

  hs_training = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 1, 10, 1, 0, 0)));
  gtk_widget_show (hs_training);
  gtk_fixed_put (GTK_FIXED (panel_camp), hs_training, 220, 152);
  gtk_widget_set_size_request (hs_training, 200, 36);
  GTK_WIDGET_UNSET_FLAGS (hs_training, GTK_CAN_FOCUS);
  gtk_scale_set_value_pos (GTK_SCALE (hs_training), GTK_POS_RIGHT);

  hs_recreation = gtk_hscale_new (GTK_ADJUSTMENT (gtk_adjustment_new (0, 1, 10, 1, 0, 0)));
  gtk_widget_show (hs_recreation);
  gtk_fixed_put (GTK_FIXED (panel_camp), hs_recreation, 220, 120);
  gtk_widget_set_size_request (hs_recreation, 200, 36);
  GTK_WIDGET_UNSET_FLAGS (hs_recreation, GTK_CAN_FOCUS);
  gtk_scale_set_value_pos (GTK_SCALE (hs_recreation), GTK_POS_RIGHT);

  b_dec_recreation = gtk_button_new ();
  gtk_widget_show (b_dec_recreation);
  gtk_fixed_put (GTK_FIXED (panel_camp), b_dec_recreation, 176, 125);
  gtk_widget_set_size_request (b_dec_recreation, 30, 26);

  image1 = create_pixmap (window_training_camp, "decrease_red.png");
  gtk_widget_show (image1);
  gtk_container_add (GTK_CONTAINER (b_dec_recreation), image1);

  rb_camp1 = gtk_radio_button_new_with_mnemonic (NULL, _("Good Hotel"));
  gtk_widget_show (rb_camp1);
  gtk_fixed_put (GTK_FIXED (panel_camp), rb_camp1, 24, 16);
  gtk_widget_set_size_request (rb_camp1, 200, 21);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (rb_camp1), rb_camp2_group);
  rb_camp2_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rb_camp1));

  g_signal_connect ((gpointer) b_cancel, "clicked",
                    G_CALLBACK (on_b_cancel_clicked),
                    NULL);
  g_signal_connect ((gpointer) b_ok, "clicked",
                    G_CALLBACK (on_b_ok_clicked),
                    NULL);
  g_signal_connect ((gpointer) b_inc_recreation, "clicked",
                    G_CALLBACK (on_b_inc_recreation_clicked),
                    NULL);
  g_signal_connect ((gpointer) rb_camp2, "clicked",
                    G_CALLBACK (on_rb_camp2_clicked),
                    NULL);
  g_signal_connect ((gpointer) rb_camp3, "clicked",
                    G_CALLBACK (on_rb_camp3_clicked),
                    NULL);
  g_signal_connect ((gpointer) b_dec_training, "clicked",
                    G_CALLBACK (on_b_dec_training_clicked),
                    NULL);
  g_signal_connect ((gpointer) b_inc_training, "clicked",
                    G_CALLBACK (on_b_inc_training_clicked),
                    NULL);
  g_signal_connect ((gpointer) b_dec_recreation, "clicked",
                    G_CALLBACK (on_b_dec_recreation_clicked),
                    NULL);
  g_signal_connect ((gpointer) rb_camp1, "clicked",
                    G_CALLBACK (on_rb_camp1_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (window_training_camp, window_training_camp, "window_training_camp");
  GLADE_HOOKUP_OBJECT (window_training_camp, panel_camp, "panel_camp");
  GLADE_HOOKUP_OBJECT (window_training_camp, tf_costs, "tf_costs");
  GLADE_HOOKUP_OBJECT (window_training_camp, hs_camp1, "hs_camp1");
  GLADE_HOOKUP_OBJECT (window_training_camp, b_cancel, "b_cancel");
  GLADE_HOOKUP_OBJECT (window_training_camp, b_ok, "b_ok");
  GLADE_HOOKUP_OBJECT (window_training_camp, hs_camp2, "hs_camp2");
  GLADE_HOOKUP_OBJECT (window_training_camp, l_camp_points, "l_camp_points");
  GLADE_HOOKUP_OBJECT (window_training_camp, b_inc_recreation, "b_inc_recreation");
  GLADE_HOOKUP_OBJECT (window_training_camp, image2, "image2");
  GLADE_HOOKUP_OBJECT (window_training_camp, l_recreation, "l_recreation");
  GLADE_HOOKUP_OBJECT (window_training_camp, l_costs, "l_costs");
  GLADE_HOOKUP_OBJECT (window_training_camp, rb_camp2, "rb_camp2");
  GLADE_HOOKUP_OBJECT (window_training_camp, rb_camp3, "rb_camp3");
  GLADE_HOOKUP_OBJECT (window_training_camp, b_dec_training, "b_dec_training");
  GLADE_HOOKUP_OBJECT (window_training_camp, image3, "image3");
  GLADE_HOOKUP_OBJECT (window_training_camp, l_training, "l_training");
  GLADE_HOOKUP_OBJECT (window_training_camp, b_inc_training, "b_inc_training");
  GLADE_HOOKUP_OBJECT (window_training_camp, image4, "image4");
  GLADE_HOOKUP_OBJECT (window_training_camp, hs_camp_points, "hs_camp_points");
  GLADE_HOOKUP_OBJECT (window_training_camp, hs_training, "hs_training");
  GLADE_HOOKUP_OBJECT (window_training_camp, hs_recreation, "hs_recreation");
  GLADE_HOOKUP_OBJECT (window_training_camp, b_dec_recreation, "b_dec_recreation");
  GLADE_HOOKUP_OBJECT (window_training_camp, image1, "image1");
  GLADE_HOOKUP_OBJECT (window_training_camp, rb_camp1, "rb_camp1");

  gtk_widget_grab_focus (rb_camp1);
  return window_training_camp;
}

