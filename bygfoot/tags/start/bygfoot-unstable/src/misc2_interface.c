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

#include "misc2_callbacks.h"
#include "misc2_interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)

GtkWidget*
create_stadium_window (void)
{
  GtkWidget *stadium_window;
  GtkWidget *vbox1;
  GtkWidget *hbox1;
  GtkWidget *vbox2;
  GtkWidget *label1;
  GtkWidget *hseparator1;
  GtkWidget *radiobutton_seat0;
  GSList *radiobutton_seat0_group = NULL;
  GtkWidget *radiobutton_seat1;
  GtkWidget *radiobutton_seat2;
  GtkWidget *radiobutton_seat3;
  GtkWidget *vseparator1;
  GtkWidget *vbox3;
  GtkWidget *label2;
  GtkWidget *hseparator2;
  GtkWidget *label_cost0;
  GtkWidget *label_cost1;
  GtkWidget *label_cost2;
  GtkWidget *label_cost3;
  GtkWidget *vseparator2;
  GtkWidget *vbox4;
  GtkWidget *label3;
  GtkWidget *hseparator3;
  GtkWidget *label8;
  GtkWidget *label_finished0;
  GtkWidget *label_finished1;
  GtkWidget *label_finished2;
  GtkWidget *hseparator4;
  GtkWidget *hbox2;
  GtkWidget *button_ok;
  GtkWidget *alignment1;
  GtkWidget *hbox3;
  GtkWidget *image1;
  GtkWidget *label12;
  GtkWidget *button_cancel;
  GtkWidget *alignment2;
  GtkWidget *hbox4;
  GtkWidget *image2;
  GtkWidget *label13;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  stadium_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (stadium_window), 5);
  gtk_window_set_title (GTK_WINDOW (stadium_window), _("Bygfoot - Stadium improvement"));
  gtk_window_set_position (GTK_WINDOW (stadium_window), GTK_WIN_POS_CENTER);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (stadium_window), vbox1);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox1);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, TRUE, 0);

  vbox2 = gtk_vbox_new (FALSE, 3);
  gtk_widget_show (vbox2);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 0);

  label1 = gtk_label_new (_("Increase (seats)"));
  gtk_widget_show (label1);
  gtk_box_pack_start (GTK_BOX (vbox2), label1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);

  hseparator1 = gtk_hseparator_new ();
  gtk_widget_show (hseparator1);
  gtk_box_pack_start (GTK_BOX (vbox2), hseparator1, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator1, 1, 10);

  radiobutton_seat0 = gtk_radio_button_new_with_mnemonic (NULL, _("500"));
  gtk_widget_show (radiobutton_seat0);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton_seat0, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_seat0), radiobutton_seat0_group);
  radiobutton_seat0_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_seat0));
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (radiobutton_seat0), TRUE);

  radiobutton_seat1 = gtk_radio_button_new_with_mnemonic (NULL, _("1000"));
  gtk_widget_show (radiobutton_seat1);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton_seat1, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_seat1), radiobutton_seat0_group);
  radiobutton_seat0_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_seat1));

  radiobutton_seat2 = gtk_radio_button_new_with_mnemonic (NULL, _("1500"));
  gtk_widget_show (radiobutton_seat2);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton_seat2, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_seat2), radiobutton_seat0_group);
  radiobutton_seat0_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_seat2));

  radiobutton_seat3 = gtk_radio_button_new_with_mnemonic (NULL, _("2000"));
  gtk_widget_show (radiobutton_seat3);
  gtk_box_pack_start (GTK_BOX (vbox2), radiobutton_seat3, FALSE, FALSE, 0);
  gtk_radio_button_set_group (GTK_RADIO_BUTTON (radiobutton_seat3), radiobutton_seat0_group);
  radiobutton_seat0_group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (radiobutton_seat3));

  vseparator1 = gtk_vseparator_new ();
  gtk_widget_show (vseparator1);
  gtk_box_pack_start (GTK_BOX (hbox1), vseparator1, FALSE, TRUE, 0);
  gtk_widget_set_size_request (vseparator1, 10, 1);

  vbox3 = gtk_vbox_new (FALSE, 3);
  gtk_widget_show (vbox3);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox3, TRUE, TRUE, 0);

  label2 = gtk_label_new (_("Costs"));
  gtk_widget_show (label2);
  gtk_box_pack_start (GTK_BOX (vbox3), label2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);

  hseparator2 = gtk_hseparator_new ();
  gtk_widget_show (hseparator2);
  gtk_box_pack_start (GTK_BOX (vbox3), hseparator2, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator2, 1, 10);

  label_cost0 = gtk_label_new (_("label4"));
  gtk_widget_show (label_cost0);
  gtk_box_pack_start (GTK_BOX (vbox3), label_cost0, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_cost0), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_cost0), 0, 4);

  label_cost1 = gtk_label_new (_("label5"));
  gtk_widget_show (label_cost1);
  gtk_box_pack_start (GTK_BOX (vbox3), label_cost1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_cost1), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_cost1), 0, 4);

  label_cost2 = gtk_label_new (_("label6"));
  gtk_widget_show (label_cost2);
  gtk_box_pack_start (GTK_BOX (vbox3), label_cost2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_cost2), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_cost2), 0, 4);

  label_cost3 = gtk_label_new (_("label7"));
  gtk_widget_show (label_cost3);
  gtk_box_pack_start (GTK_BOX (vbox3), label_cost3, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_cost3), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_cost3), 0, 4);

  vseparator2 = gtk_vseparator_new ();
  gtk_widget_show (vseparator2);
  gtk_box_pack_start (GTK_BOX (hbox1), vseparator2, FALSE, TRUE, 0);
  gtk_widget_set_size_request (vseparator2, 10, 1);

  vbox4 = gtk_vbox_new (FALSE, 3);
  gtk_widget_show (vbox4);
  gtk_box_pack_start (GTK_BOX (hbox1), vbox4, TRUE, TRUE, 0);

  label3 = gtk_label_new (_("Finished (weeks)"));
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (vbox4), label3, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label3), GTK_JUSTIFY_LEFT);

  hseparator3 = gtk_hseparator_new ();
  gtk_widget_show (hseparator3);
  gtk_box_pack_start (GTK_BOX (vbox4), hseparator3, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator3, 1, 10);

  label8 = gtk_label_new (_("Before next match"));
  gtk_widget_show (label8);
  gtk_box_pack_start (GTK_BOX (vbox4), label8, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label8), 0, 3);

  label_finished0 = gtk_label_new (_("1-2"));
  gtk_widget_show (label_finished0);
  gtk_box_pack_start (GTK_BOX (vbox4), label_finished0, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_finished0), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_finished0), 0, 3);

  label_finished1 = gtk_label_new (_("3-4"));
  gtk_widget_show (label_finished1);
  gtk_box_pack_start (GTK_BOX (vbox4), label_finished1, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_finished1), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_finished1), 0, 3);

  label_finished2 = gtk_label_new (_("5-6"));
  gtk_widget_show (label_finished2);
  gtk_box_pack_start (GTK_BOX (vbox4), label_finished2, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label_finished2), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label_finished2), 0, 3);

  hseparator4 = gtk_hseparator_new ();
  gtk_widget_show (hseparator4);
  gtk_box_pack_start (GTK_BOX (vbox1), hseparator4, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator4, 1, 10);

  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox2, FALSE, TRUE, 0);

  button_ok = gtk_button_new ();
  gtk_widget_show (button_ok);
  gtk_box_pack_start (GTK_BOX (hbox2), button_ok, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button_ok), 2);
  gtk_tooltips_set_tip (tooltips, button_ok, _("Return"), NULL);
  gtk_widget_add_accelerator (button_ok, "clicked", accel_group,
                              GDK_Return, 0,
                              GTK_ACCEL_VISIBLE);

  alignment1 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment1);
  gtk_container_add (GTK_CONTAINER (button_ok), alignment1);

  hbox3 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox3);
  gtk_container_add (GTK_CONTAINER (alignment1), hbox3);

  image1 = gtk_image_new_from_stock ("gtk-apply", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image1);
  gtk_box_pack_start (GTK_BOX (hbox3), image1, FALSE, FALSE, 0);

  label12 = gtk_label_new_with_mnemonic (_("OK"));
  gtk_widget_show (label12);
  gtk_box_pack_start (GTK_BOX (hbox3), label12, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label12), GTK_JUSTIFY_LEFT);

  button_cancel = gtk_button_new ();
  gtk_widget_show (button_cancel);
  gtk_box_pack_start (GTK_BOX (hbox2), button_cancel, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button_cancel), 2);
  gtk_tooltips_set_tip (tooltips, button_cancel, _("Esc"), NULL);
  gtk_widget_add_accelerator (button_cancel, "clicked", accel_group,
                              GDK_Escape, 0,
                              GTK_ACCEL_VISIBLE);

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (button_cancel), alignment2);

  hbox4 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox4);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox4);

  image2 = gtk_image_new_from_stock ("gtk-cancel", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image2);
  gtk_box_pack_start (GTK_BOX (hbox4), image2, FALSE, FALSE, 0);

  label13 = gtk_label_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (label13);
  gtk_box_pack_start (GTK_BOX (hbox4), label13, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label13), GTK_JUSTIFY_LEFT);

  g_signal_connect ((gpointer) stadium_window, "delete_event",
                    G_CALLBACK (on_button_cancel_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_ok, "clicked",
                    G_CALLBACK (on_button_ok_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_cancel, "clicked",
                    G_CALLBACK (on_button_cancel_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (stadium_window, stadium_window, "stadium_window");
  GLADE_HOOKUP_OBJECT (stadium_window, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (stadium_window, hbox1, "hbox1");
  GLADE_HOOKUP_OBJECT (stadium_window, vbox2, "vbox2");
  GLADE_HOOKUP_OBJECT (stadium_window, label1, "label1");
  GLADE_HOOKUP_OBJECT (stadium_window, hseparator1, "hseparator1");
  GLADE_HOOKUP_OBJECT (stadium_window, radiobutton_seat0, "radiobutton_seat0");
  GLADE_HOOKUP_OBJECT (stadium_window, radiobutton_seat1, "radiobutton_seat1");
  GLADE_HOOKUP_OBJECT (stadium_window, radiobutton_seat2, "radiobutton_seat2");
  GLADE_HOOKUP_OBJECT (stadium_window, radiobutton_seat3, "radiobutton_seat3");
  GLADE_HOOKUP_OBJECT (stadium_window, vseparator1, "vseparator1");
  GLADE_HOOKUP_OBJECT (stadium_window, vbox3, "vbox3");
  GLADE_HOOKUP_OBJECT (stadium_window, label2, "label2");
  GLADE_HOOKUP_OBJECT (stadium_window, hseparator2, "hseparator2");
  GLADE_HOOKUP_OBJECT (stadium_window, label_cost0, "label_cost0");
  GLADE_HOOKUP_OBJECT (stadium_window, label_cost1, "label_cost1");
  GLADE_HOOKUP_OBJECT (stadium_window, label_cost2, "label_cost2");
  GLADE_HOOKUP_OBJECT (stadium_window, label_cost3, "label_cost3");
  GLADE_HOOKUP_OBJECT (stadium_window, vseparator2, "vseparator2");
  GLADE_HOOKUP_OBJECT (stadium_window, vbox4, "vbox4");
  GLADE_HOOKUP_OBJECT (stadium_window, label3, "label3");
  GLADE_HOOKUP_OBJECT (stadium_window, hseparator3, "hseparator3");
  GLADE_HOOKUP_OBJECT (stadium_window, label8, "label8");
  GLADE_HOOKUP_OBJECT (stadium_window, label_finished0, "label_finished0");
  GLADE_HOOKUP_OBJECT (stadium_window, label_finished1, "label_finished1");
  GLADE_HOOKUP_OBJECT (stadium_window, label_finished2, "label_finished2");
  GLADE_HOOKUP_OBJECT (stadium_window, hseparator4, "hseparator4");
  GLADE_HOOKUP_OBJECT (stadium_window, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (stadium_window, button_ok, "button_ok");
  GLADE_HOOKUP_OBJECT (stadium_window, alignment1, "alignment1");
  GLADE_HOOKUP_OBJECT (stadium_window, hbox3, "hbox3");
  GLADE_HOOKUP_OBJECT (stadium_window, image1, "image1");
  GLADE_HOOKUP_OBJECT (stadium_window, label12, "label12");
  GLADE_HOOKUP_OBJECT (stadium_window, button_cancel, "button_cancel");
  GLADE_HOOKUP_OBJECT (stadium_window, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (stadium_window, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (stadium_window, image2, "image2");
  GLADE_HOOKUP_OBJECT (stadium_window, label13, "label13");
  GLADE_HOOKUP_OBJECT_NO_REF (stadium_window, tooltips, "tooltips");

  gtk_window_add_accel_group (GTK_WINDOW (stadium_window), accel_group);

  return stadium_window;
}

GtkWidget*
create_job_offer_window (void)
{
  GtkWidget *job_offer_window;
  GtkWidget *vbox5;
  GtkWidget *label20;
  GtkWidget *hseparator5;
  GtkWidget *hbox5;
  GtkWidget *vbox6;
  GtkWidget *label15;
  GtkWidget *label16;
  GtkWidget *label17;
  GtkWidget *label18;
  GtkWidget *label19;
  GtkWidget *label14;
  GtkWidget *vseparator3;
  GtkWidget *vbox7;
  GtkWidget *label21;
  GtkWidget *label22;
  GtkWidget *label23;
  GtkWidget *label24;
  GtkWidget *label25;
  GtkWidget *label26;
  GtkWidget *hseparator6;
  GtkWidget *label28;
  GtkWidget *scrolledwindow1;
  GtkWidget *treeview_player_list;
  GtkWidget *hseparator7;
  GtkWidget *label27;
  GtkWidget *hbox6;
  GtkWidget *button_ok;
  GtkWidget *alignment5;
  GtkWidget *hbox9;
  GtkWidget *image5;
  GtkWidget *label31;
  GtkWidget *button_cancel;
  GtkWidget *alignment4;
  GtkWidget *hbox8;
  GtkWidget *image4;
  GtkWidget *label30;

  job_offer_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (job_offer_window), 6);
  gtk_window_set_title (GTK_WINDOW (job_offer_window), _("Job offer"));
  gtk_window_set_position (GTK_WINDOW (job_offer_window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW (job_offer_window), -1, 600);

  vbox5 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox5);
  gtk_container_add (GTK_CONTAINER (job_offer_window), vbox5);

  label20 = gtk_label_new (_("label20"));
  gtk_widget_show (label20);
  gtk_box_pack_start (GTK_BOX (vbox5), label20, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label20), GTK_JUSTIFY_LEFT);

  hseparator5 = gtk_hseparator_new ();
  gtk_widget_show (hseparator5);
  gtk_box_pack_start (GTK_BOX (vbox5), hseparator5, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator5, 1, 10);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (vbox5), hbox5, FALSE, TRUE, 0);

  vbox6 = gtk_vbox_new (FALSE, 3);
  gtk_widget_show (vbox6);
  gtk_box_pack_start (GTK_BOX (hbox5), vbox6, FALSE, TRUE, 0);

  label15 = gtk_label_new (_("Name"));
  gtk_widget_show (label15);
  gtk_box_pack_start (GTK_BOX (vbox6), label15, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label15), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label15), 2.23517e-10, 0.5);

  label16 = gtk_label_new (_("League"));
  gtk_widget_show (label16);
  gtk_box_pack_start (GTK_BOX (vbox6), label16, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label16), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label16), 2.23517e-10, 0.5);

  label17 = gtk_label_new (_("Rank"));
  gtk_widget_show (label17);
  gtk_box_pack_start (GTK_BOX (vbox6), label17, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label17), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label17), 2.23517e-10, 0.5);

  label18 = gtk_label_new (_("Money (approx.)"));
  gtk_widget_show (label18);
  gtk_box_pack_start (GTK_BOX (vbox6), label18, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label18), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label18), 2.23517e-10, 0.5);

  label19 = gtk_label_new (_("Stadium capacity"));
  gtk_widget_show (label19);
  gtk_box_pack_start (GTK_BOX (vbox6), label19, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label19), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label19), 2.23517e-10, 0.5);

  label14 = gtk_label_new (_("Stadium safety"));
  gtk_widget_show (label14);
  gtk_box_pack_start (GTK_BOX (vbox6), label14, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label14), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (label14), 0, 0.5);

  vseparator3 = gtk_vseparator_new ();
  gtk_widget_show (vseparator3);
  gtk_box_pack_start (GTK_BOX (hbox5), vseparator3, FALSE, TRUE, 0);
  gtk_widget_set_size_request (vseparator3, 10, 1);

  vbox7 = gtk_vbox_new (FALSE, 3);
  gtk_widget_show (vbox7);
  gtk_box_pack_start (GTK_BOX (hbox5), vbox7, TRUE, TRUE, 0);

  label21 = gtk_label_new (_("label21"));
  gtk_widget_show (label21);
  gtk_box_pack_start (GTK_BOX (vbox7), label21, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label21), GTK_JUSTIFY_LEFT);

  label22 = gtk_label_new (_("label22"));
  gtk_widget_show (label22);
  gtk_box_pack_start (GTK_BOX (vbox7), label22, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label22), GTK_JUSTIFY_LEFT);

  label23 = gtk_label_new (_("label23"));
  gtk_widget_show (label23);
  gtk_box_pack_start (GTK_BOX (vbox7), label23, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label23), GTK_JUSTIFY_LEFT);

  label24 = gtk_label_new (_("label24"));
  gtk_widget_show (label24);
  gtk_box_pack_start (GTK_BOX (vbox7), label24, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label24), GTK_JUSTIFY_LEFT);

  label25 = gtk_label_new (_("label25"));
  gtk_widget_show (label25);
  gtk_box_pack_start (GTK_BOX (vbox7), label25, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label25), GTK_JUSTIFY_LEFT);

  label26 = gtk_label_new (_("label26"));
  gtk_widget_show (label26);
  gtk_box_pack_start (GTK_BOX (vbox7), label26, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label26), GTK_JUSTIFY_LEFT);

  hseparator6 = gtk_hseparator_new ();
  gtk_widget_show (hseparator6);
  gtk_box_pack_start (GTK_BOX (vbox5), hseparator6, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator6, 1, 10);

  label28 = gtk_label_new (_("Player list"));
  gtk_widget_show (label28);
  gtk_box_pack_start (GTK_BOX (vbox5), label28, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label28), GTK_JUSTIFY_LEFT);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox5), scrolledwindow1, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow1), 4);

  treeview_player_list = gtk_tree_view_new ();
  gtk_widget_show (treeview_player_list);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), treeview_player_list);

  hseparator7 = gtk_hseparator_new ();
  gtk_widget_show (hseparator7);
  gtk_box_pack_start (GTK_BOX (vbox5), hseparator7, FALSE, TRUE, 0);
  gtk_widget_set_size_request (hseparator7, 1, 10);

  label27 = gtk_label_new (_("label27"));
  gtk_widget_show (label27);
  gtk_box_pack_start (GTK_BOX (vbox5), label27, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label27), GTK_JUSTIFY_LEFT);
  gtk_misc_set_padding (GTK_MISC (label27), 0, 5);

  hbox6 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox6);
  gtk_box_pack_start (GTK_BOX (vbox5), hbox6, FALSE, TRUE, 0);

  button_ok = gtk_button_new ();
  gtk_widget_show (button_ok);
  gtk_box_pack_start (GTK_BOX (hbox6), button_ok, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button_ok), 2);

  alignment5 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment5);
  gtk_container_add (GTK_CONTAINER (button_ok), alignment5);

  hbox9 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox9);
  gtk_container_add (GTK_CONTAINER (alignment5), hbox9);

  image5 = gtk_image_new_from_stock ("gtk-apply", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image5);
  gtk_box_pack_start (GTK_BOX (hbox9), image5, FALSE, FALSE, 0);

  label31 = gtk_label_new_with_mnemonic (_("OK"));
  gtk_widget_show (label31);
  gtk_box_pack_start (GTK_BOX (hbox9), label31, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label31), GTK_JUSTIFY_LEFT);

  button_cancel = gtk_button_new ();
  gtk_widget_show (button_cancel);
  gtk_box_pack_start (GTK_BOX (hbox6), button_cancel, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (button_cancel), 2);

  alignment4 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_show (alignment4);
  gtk_container_add (GTK_CONTAINER (button_cancel), alignment4);

  hbox8 = gtk_hbox_new (FALSE, 2);
  gtk_widget_show (hbox8);
  gtk_container_add (GTK_CONTAINER (alignment4), hbox8);

  image4 = gtk_image_new_from_stock ("gtk-cancel", GTK_ICON_SIZE_BUTTON);
  gtk_widget_show (image4);
  gtk_box_pack_start (GTK_BOX (hbox8), image4, FALSE, FALSE, 0);

  label30 = gtk_label_new_with_mnemonic (_("Cancel"));
  gtk_widget_show (label30);
  gtk_box_pack_start (GTK_BOX (hbox8), label30, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label30), GTK_JUSTIFY_LEFT);

  g_signal_connect ((gpointer) button_ok, "clicked",
                    G_CALLBACK (on_button_offer_ok_clicked),
                    NULL);
  g_signal_connect ((gpointer) button_cancel, "clicked",
                    G_CALLBACK (on_button_offer_cancel_clicked),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (job_offer_window, job_offer_window, "job_offer_window");
  GLADE_HOOKUP_OBJECT (job_offer_window, vbox5, "vbox5");
  GLADE_HOOKUP_OBJECT (job_offer_window, label20, "label20");
  GLADE_HOOKUP_OBJECT (job_offer_window, hseparator5, "hseparator5");
  GLADE_HOOKUP_OBJECT (job_offer_window, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (job_offer_window, vbox6, "vbox6");
  GLADE_HOOKUP_OBJECT (job_offer_window, label15, "label15");
  GLADE_HOOKUP_OBJECT (job_offer_window, label16, "label16");
  GLADE_HOOKUP_OBJECT (job_offer_window, label17, "label17");
  GLADE_HOOKUP_OBJECT (job_offer_window, label18, "label18");
  GLADE_HOOKUP_OBJECT (job_offer_window, label19, "label19");
  GLADE_HOOKUP_OBJECT (job_offer_window, label14, "label14");
  GLADE_HOOKUP_OBJECT (job_offer_window, vseparator3, "vseparator3");
  GLADE_HOOKUP_OBJECT (job_offer_window, vbox7, "vbox7");
  GLADE_HOOKUP_OBJECT (job_offer_window, label21, "label21");
  GLADE_HOOKUP_OBJECT (job_offer_window, label22, "label22");
  GLADE_HOOKUP_OBJECT (job_offer_window, label23, "label23");
  GLADE_HOOKUP_OBJECT (job_offer_window, label24, "label24");
  GLADE_HOOKUP_OBJECT (job_offer_window, label25, "label25");
  GLADE_HOOKUP_OBJECT (job_offer_window, label26, "label26");
  GLADE_HOOKUP_OBJECT (job_offer_window, hseparator6, "hseparator6");
  GLADE_HOOKUP_OBJECT (job_offer_window, label28, "label28");
  GLADE_HOOKUP_OBJECT (job_offer_window, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (job_offer_window, treeview_player_list, "treeview_player_list");
  GLADE_HOOKUP_OBJECT (job_offer_window, hseparator7, "hseparator7");
  GLADE_HOOKUP_OBJECT (job_offer_window, label27, "label27");
  GLADE_HOOKUP_OBJECT (job_offer_window, hbox6, "hbox6");
  GLADE_HOOKUP_OBJECT (job_offer_window, button_ok, "button_ok");
  GLADE_HOOKUP_OBJECT (job_offer_window, alignment5, "alignment5");
  GLADE_HOOKUP_OBJECT (job_offer_window, hbox9, "hbox9");
  GLADE_HOOKUP_OBJECT (job_offer_window, image5, "image5");
  GLADE_HOOKUP_OBJECT (job_offer_window, label31, "label31");
  GLADE_HOOKUP_OBJECT (job_offer_window, button_cancel, "button_cancel");
  GLADE_HOOKUP_OBJECT (job_offer_window, alignment4, "alignment4");
  GLADE_HOOKUP_OBJECT (job_offer_window, hbox8, "hbox8");
  GLADE_HOOKUP_OBJECT (job_offer_window, image4, "image4");
  GLADE_HOOKUP_OBJECT (job_offer_window, label30, "label30");

  return job_offer_window;
}

