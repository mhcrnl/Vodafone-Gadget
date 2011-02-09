/*
 * Copyright © 2011
 * Andrea Costa <nukemup@hotmail.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * or write to the Free Software Foundation, Inc., 51 Franklin St
 * Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "vodafone_gadget.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern void vodafone_gadget_fix_text_before_sending (GtkButton *button, vodafone_gadget_core_s *vodafone_gadget_core);


/*
  Create GtkStatusIcon menu
*/
void
vodafone_gadget_create_icon_menu (vodafone_gadget_core_s *core)
{
  core->icon_menu = gtk_menu_new ();
  core->about_item = gtk_menu_item_new_with_label ("About");
  core->quit_item = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_shell_append (GTK_MENU_SHELL(core->icon_menu), core->about_item);
  gtk_menu_shell_append (GTK_MENU_SHELL(core->icon_menu), core->quit_item);
  gtk_widget_show_all (core->icon_menu);
}


/*
  Count many available chars are left in the messagge
*/
void
vodafone_gadget_func_count_char (GtkButton *button,vodafone_gadget_core_s *core)
{
  GdkColor color;
  gchar *string;
  int i;

  i = gtk_text_buffer_get_char_count (core->buffer);

  color.green = 0;
  color.blue = 0;
	
  if (i < 360)
    color.red = 0;
	else
    color.red = 0.9 * 65535;

  gtk_widget_modify_fg (core->label_char_left, GTK_STATE_NORMAL, &color);

	string = g_strdup_printf ("Caratteri: %d", 360 - i);
	gtk_label_set_text (GTK_LABEL(core->label_char_left), string);
}



/*
  Send message (thread)
*/
void
vodafone_gadget_send_thread_send (vodafone_gadget_core_s *core)
{
  gchar *string;
  char *ptr;
  int i = 0;
  GdkColor color;

  color.blue = 0;

  gdk_threads_enter ();
    gtk_label_set_text (GTK_LABEL(core->label_notify), "Invio messaggio...");	
  gdk_threads_leave ();

  g_print ("* Thread sending message\n");
  g_print ("* number:  %s\n", core->number);
  g_print ("* subject: %s\n", core->mms_subject);
  g_print ("* text:    %s\n", core->text_message);

	string = (gchar *) gtk_combo_box_get_active_text(GTK_COMBO_BOX(core->combobox_send));

	if ((ptr = strstr (string, "sms")) != NULL)
		/* Send SMS */
		i = vodafone_gadget_send_sms (core);

	else if ((ptr = strstr (string, "mms")) != NULL)
		/* Send MMS */
		i = vodafone_gadget_send_mms (core);

	
  gdk_threads_enter ();
	
    if (i == 0)
    {
      color.red = 0;
      color.green = 0.9 * 65535;

      gtk_widget_modify_fg (core->label_notify, GTK_STATE_NORMAL, &color);

      /* Setting entries and buffer */
      gtk_label_set_text (GTK_LABEL(core->label_notify), "Messaggio Inviato");
      gtk_entry_set_text (GTK_ENTRY(core->entry_subject), "Oggetto");
      gtk_entry_set_text (GTK_ENTRY(core->entry_mobile_number), "Destinatario");
      gtk_text_buffer_set_text (core->buffer, "", -1);

      /* Set send button sensitive */
      gtk_widget_set_sensitive (core->button_send, TRUE);

      /* Notify message */
      vodafone_gadget_notify (TRUE, "Messaggio inviato correttamente.");

      vodafone_gadget_notify (TRUE, NULL);
    }
    else
    {
      color.red = 0.9 * 65535;
      color.green = 0;

      gtk_widget_modify_fg (core->label_notify, GTK_STATE_NORMAL, &color);

      /* Setting entries and buffer */
      gtk_label_set_text (GTK_LABEL(core->label_notify), decode_error (i));

      /* Set send button sensitive */
      gtk_widget_set_sensitive (core->button_send, TRUE);

      /* Notify message */
      vodafone_gadget_notify (FALSE, g_strdup_printf ("Errore invio messaggio - %s.", decode_error (i)));
    }
	
	gdk_threads_leave ();
}


/*
  Called by button_send. Check/fix text and subject before opening the sending thread
*/
void
vodafone_gadget_func_send (GtkButton *button, vodafone_gadget_core_s *core)
{	
  GError **thread_error;
  GtkTextIter start;
  GtkTextIter end;
  gchar *string;
  int i;
  char *data;
  GdkColor color;	

  color.red = 65535;
  color.green = 65535;
  color.blue = 65535;
  gtk_widget_modify_fg (core->label_notify, GTK_STATE_NORMAL, &color);


  /* Set send button not sensitive */  
  gtk_widget_set_sensitive (core->button_send, FALSE);

  /* Save mobile phone number */
  core->number = (gchar *) gtk_entry_get_text (GTK_ENTRY(core->entry_mobile_number));

  /* Save subject mms message */
  data = (gchar *) gtk_entry_get_text (GTK_ENTRY(core->entry_subject));
  snprintf (core->mms_subject, 30, "%s", data);
	
  /* Verify subject text -> + and letter with accent */
  for (i = 0; i < strlen (core->mms_subject); i++)
  {
    if (core->mms_subject [i] == ' ')
				core->mms_subject [i] = '+';
    else if (((int) core->mms_subject [i] == -61) && ((int) core->mms_subject [i+1] < 0))
    {
			if ((int) core->mms_subject [i+1] == -96)
				core->mms_subject [i] = 'a';
			else if ((int) core->mms_subject [i+1] == -88)
				core->mms_subject [i] = 'e';
			else if ((int) core->mms_subject [i+1] == -87)
				core->mms_subject [i] = 'e';
			else if ((int) core->mms_subject [i+1] == -84)
				core->mms_subject [i] = 'i';
			else if ((int) core->mms_subject [i+1] == -78)
				core->mms_subject [i] = 'o';
			else if ((int) core->mms_subject [i+1] == -71)
				core->mms_subject [i] = 'u';

      core->mms_subject [i+1] = (char) 39;
    }
  }
	
  /* Save sending text */
  gtk_text_buffer_get_start_iter (core->buffer, &start);

  i = gtk_text_buffer_get_char_count (core->buffer);

  if (i < 360)
  {
    gtk_text_buffer_get_end_iter (core->buffer, &end);
    data = gtk_text_buffer_get_text (core->buffer, &start, &end, FALSE);
    snprintf (core->text_message, 360, "%s", data);
  }
  else
  {
    gtk_text_buffer_get_iter_at_offset (core->buffer, &end, 360);
    data = gtk_text_buffer_get_slice (core->buffer, &start, &end, FALSE);
    snprintf (core->text_message, 360, "%s", data);
  }

  /* Check text message */
  for (i = 0; i < strlen (core->text_message); i++)
  {
    if (core->text_message [i] == ' ')
      core->text_message [i] = '+';
		else if (((int) core->text_message [i] == -61) && ((int) core->text_message [i+1] < 0))
    {
      if ((int) core->text_message [i+1] == -96)
        core->text_message [i] = 'a';
      else if ((int) core->text_message [i+1] == -88)
        core->text_message [i] = 'e';
      else if ((int) core->text_message [i+1] == -87)
        core->text_message [i] = 'e';
      else if ((int) core->text_message [i+1] == -84)
        core->text_message [i] = 'i';
      else if ((int) core->text_message [i+1] == -78)
        core->text_message [i] = 'o';
      else if ((int) core->text_message [i+1] == -71)
        core->text_message [i] = 'u';

      core->text_message [i+1] = (char) 39;
    }
  }

  /* Open thread in order to send the message */
  g_thread_create ((GThreadFunc) vodafone_gadget_send_thread_send, core, 0, thread_error);
}


/*
  Create logged area
*/
void
vodafone_gadget_func_create_logged_v_box (vodafone_gadget_core_s *core)
{
  GtkWidget *separator;
  GtkWidget *h_box;
  GtkWidget *label;
  GdkColor color;
  gchar *string;
	
  core->logged_vbox = gtk_vbox_new (FALSE, 0);

	/* Username label */
	string = g_strdup_printf ("Ciao %s", core->name);
	label = gtk_label_new (string);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), label, FALSE, FALSE, 3);
	gtk_widget_show (label);

	separator = gtk_hseparator_new ();
	gtk_widget_set_size_request (separator, 150, 1);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), separator, FALSE, TRUE, 2);
	gtk_widget_show (separator);

	/* Entry subject mms - Max 19 char */
	h_box = gtk_hbox_new (FALSE, 0);

    core->entry_subject = (GtkWidget *) libui_gtk_entry_new_with_descriptor (TRUE,
                                                                             19,
                                                                             "Oggetto",
                                                                             "sans 9",
                                                                             TRUE,
                                                                             130,
                                                                             17,
                                                                             FALSE,
                                                                             NULL);

		gtk_box_pack_start (GTK_BOX(h_box), core->entry_subject, TRUE, FALSE, 0);
		gtk_widget_show (core->entry_subject);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), h_box, FALSE, FALSE, 1);
	gtk_widget_show (h_box);
	
	/* Entry mobile phone number - Max 15 char */
	h_box = gtk_hbox_new (FALSE, 0);	
		core->entry_mobile_number =  (GtkWidget *) libui_gtk_entry_new_with_descriptor (TRUE,
                                                                                    15,
                                                                                    "Destinatario",
                                                                                    "sans 9",
                                                                                    TRUE,
                                                                                    130,
                                                                                    17,
                                                                                    FALSE,
                                                                                    NULL);

		gtk_box_pack_start (GTK_BOX(h_box), core->entry_mobile_number, TRUE, FALSE, 0);
		gtk_widget_show (core->entry_mobile_number);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), h_box, FALSE, FALSE, 1);
	gtk_widget_show(h_box);
	
	/* Combobox type message */
	h_box = gtk_hbox_new (FALSE, 0);	
		core->combobox_send = gtk_combo_box_new_text ();
		gtk_combo_box_append_text (GTK_COMBO_BOX(core->combobox_send), "sms");
		gtk_combo_box_append_text (GTK_COMBO_BOX(core->combobox_send), "mms");
		gtk_combo_box_set_active (GTK_COMBO_BOX(core->combobox_send), 0);
		gtk_box_pack_start (GTK_BOX(h_box), core->combobox_send, TRUE, TRUE, 10);
		gtk_widget_show (core->combobox_send);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), h_box, FALSE, FALSE, 1);
	gtk_widget_show (h_box);
	
	/* Scrolled_win + textview h_box */
	h_box = gtk_hbox_new (FALSE, 0);	

		/* Textview */
		core->textview = gtk_text_view_new ();

		/* Setting textview borders */
		gtk_text_view_set_border_window_size (GTK_TEXT_VIEW(core->textview), GTK_TEXT_WINDOW_TOP, 1);
		gtk_text_view_set_border_window_size (GTK_TEXT_VIEW(core->textview), GTK_TEXT_WINDOW_RIGHT, 1);
		gtk_text_view_set_border_window_size (GTK_TEXT_VIEW(core->textview), GTK_TEXT_WINDOW_LEFT, 1);
		gtk_text_view_set_border_window_size (GTK_TEXT_VIEW(core->textview), GTK_TEXT_WINDOW_BOTTOM, 1);
	
		/* Setting textview background -> white */
		gdk_color_parse ("white", &color);
		gtk_widget_modify_base (core->textview, GTK_STATE_NORMAL, &color);

		gtk_widget_grab_focus (core->textview);
		gtk_widget_set_size_request (core->textview, 120, 100);
		gtk_text_view_set_editable (GTK_TEXT_VIEW (core->textview), TRUE);
		gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (core->textview), TRUE);
		gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (core->textview), GTK_WRAP_WORD_CHAR);
		gtk_text_view_set_justification (GTK_TEXT_VIEW (core->textview), GTK_JUSTIFY_LEFT);

		/* Set space between two lines */
		gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (core->textview), 1);
		gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (core->textview), 1);

		/* Set margins */
		gtk_text_view_set_left_margin (GTK_TEXT_VIEW (core->textview), 2);
		gtk_text_view_set_right_margin (GTK_TEXT_VIEW (core->textview), 2);

		/* Set text color -> black */
		gdk_color_parse ("black", &color);
		gtk_widget_modify_text (core->textview, GTK_STATE_NORMAL, &color);

		gtk_widget_show (core->textview);
	
		core->buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(core->textview));

		core->scrolled_win = gtk_scrolled_window_new (NULL, NULL);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (core->scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
		gtk_container_add (GTK_CONTAINER (core->scrolled_win), core->textview);
		gtk_widget_show (core->scrolled_win);
		gtk_box_pack_start (GTK_BOX(h_box), core->scrolled_win, TRUE, FALSE, 0);
	
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), h_box, FALSE, FALSE, 5);
	gtk_widget_show (h_box);
	
	/* Label available char */
	core->label_char_left = gtk_label_new ("Caratteri: 360");
	gtk_widget_show (core->label_char_left);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), core->label_char_left, FALSE, FALSE, 1);
	
	/* Button send */
	h_box = gtk_hbox_new (FALSE, 0);
    core->button_send = libgadget_button_new_with_label ("Invia", TRUE, 80, 20, NULL);
		gtk_box_pack_start (GTK_BOX(h_box), core->button_send, TRUE, FALSE, 0);
		gtk_widget_show (core->button_send);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), h_box, FALSE, FALSE, 5);
	gtk_widget_show (h_box);
	


  core->label_notify = libui_gtk_label_new_with_descriptor ("", "sans 7", FALSE, 0, 0, FALSE, NULL);
	gtk_widget_show (core->label_notify);
	gtk_box_pack_start (GTK_BOX(core->logged_vbox), core->label_notify, FALSE, FALSE, 1);

	g_signal_connect (G_OBJECT (core->buffer), "changed", G_CALLBACK (vodafone_gadget_func_count_char), core);
	g_signal_connect (G_OBJECT (core->button_send), "clicked", G_CALLBACK (vodafone_gadget_func_send), core);
}


/*
  Verify account's connection (thread) and updates gadget
*/
static gboolean
vodafone_gadget_verify_connection (vodafone_gadget_core_s *core)
{	
  int i;
	
  /* Try to log in (in order to check account and password) */
  i = vodafone_gadget_verify_session (core);

  if (i == 0)
  {
    /* Log in successful */
    gdk_threads_enter ();

      /* Stop spinner EOG */		
      ephy_spinner_stop (EPHY_SPINNER (core->spinner));

      /* Delete init_vertical_box */
      gtk_widget_destroy (core->init_vbox);

      core->height = core->height + 10;
    	gtk_window_resize (GTK_WINDOW(core->window), core->width, core->height);
      gtk_widget_queue_draw (core->window);

      /* Create and show logged_vertical_box */
      vodafone_gadget_func_create_logged_v_box (core);
      gtk_fixed_put (GTK_FIXED(core->fixed), core->logged_vbox, 10, 20);
      gtk_widget_show (core->logged_vbox);
		
    gdk_threads_leave ();
	}
  else
  {
    /* Log in failed */
    gdk_threads_enter ();

      /* Stop spinner EOG */
      ephy_spinner_stop (EPHY_SPINNER (core->spinner));

      /* Set login button sensitive */
      gtk_widget_set_sensitive (core->button_login, TRUE);

      /* Display error */
      gtk_label_set_text (GTK_LABEL(core->label_error), decode_error (i));

      /* Notify message */
      vodafone_gadget_notify (FALSE, g_strdup_printf ("Errore Log In - %s.", decode_error (i)));

    gdk_threads_leave ();
  }
}


/*
  Save account's data and check it!
*/
void
vodafone_gadget_set_session_data (GtkButton *button, vodafone_gadget_core_s *core)
{
  char *data;
  FILE *file;
  GError **thread_error;

  /* Get and save username */
  data = (gchar *) gtk_entry_get_text (GTK_ENTRY(core->entry_account));
  snprintf (core->account, strlen (data) + 1, "%s", data);

  /* Get and save password */
  data = (gchar *) gtk_entry_get_text(GTK_ENTRY(core->entry_password));
  snprintf (core->password, strlen (data) + 1, "%s", data);

	/* Set login button not sensitive */
	gtk_widget_set_sensitive (core->button_login, FALSE);
	
	/* Recreate GADGET_CONF_FILE */
	if (core->remember_me == 1)
    vodafone_gadget_xml_update_remember_me (XML_FILE, core->account);

	/* Start spinner */
	ephy_spinner_start (EPHY_SPINNER (core->spinner));
	
	/* Verify if the authentification data are correct */
	g_thread_create ((GThreadFunc) vodafone_gadget_verify_connection, core, 0, thread_error);
}


/*
  Create init area
*/
void
vodafone_gadget_func_create_init_v_box (vodafone_gadget_core_s *core)
{
  GtkWidget *separator;
  GtkWidget *h_box;
  GtkWidget *label;
  GtkWidget *image;
  GdkColor color;
  gchar *string;
	
  core->init_vbox = gtk_vbox_new (FALSE, 0);

  /* Create color - Red */
  color.red   = 0.9 * 65535;
  color.green = 0;
  color.blue  = 0;

  /* Title label */
  label = (GtkWidget *) libui_gtk_label_new_with_descriptor ("Linux Vodafone Gadget", "sans 10", FALSE, 0, 0, TRUE, &color);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), label, FALSE, FALSE, 10);
  gtk_widget_show (label);

  /* Image logo */
  h_box = gtk_hbox_new (FALSE, 0);	
    image = gtk_image_new_from_pixbuf ((GdkPixbuf *)libui_gtk_image_create_pixbuf_from_file (IMAGE_LOGO));
    gtk_box_pack_start (GTK_BOX(h_box), image, TRUE, FALSE, 0);
    gtk_widget_show (image);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), h_box, FALSE, FALSE, 0);
  gtk_widget_show (h_box);	

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_widget_set_size_request (separator, 150, 1);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), separator, FALSE, TRUE, 10);
  gtk_widget_show (separator);

  /* Username label */
  label = (GtkWidget *) libui_gtk_label_new_with_descriptor ("username:", "sans 9", FALSE, 0, 0, TRUE, &color);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), label, FALSE, FALSE, 3);
  gtk_widget_show (label);

  /* Username entry - Max 20 char */
  h_box = gtk_hbox_new (FALSE, 0);

    core->entry_account = (GtkWidget *) libui_gtk_entry_new_with_descriptor (TRUE, 20, "", "sans 9", TRUE, 130, 17, FALSE, NULL);

    if (core->remember_me == 1)
      gtk_entry_set_text (GTK_ENTRY(core->entry_account), g_strdup_printf ("%s", core->account));

    gtk_box_pack_start (GTK_BOX(h_box), core->entry_account, TRUE, FALSE, 0);
    gtk_widget_show (core->entry_account);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), h_box, FALSE, FALSE, 3);
  gtk_widget_show (h_box);

  /* Password label */
  label = (GtkWidget *) libui_gtk_label_new_with_descriptor ("password:", "sans 9", FALSE, 0, 0, TRUE, &color);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), label, FALSE, FALSE, 3);
  gtk_widget_show (label);

  /* Password entry */
  h_box = gtk_hbox_new (FALSE, 0);

    core->entry_password = (GtkWidget *) libui_gtk_entry_new_with_descriptor (TRUE, 20, "", "sans 9", TRUE, 130, 17, FALSE, NULL);
    gtk_entry_set_visibility (GTK_ENTRY (core->entry_password), FALSE);
    gtk_entry_set_invisible_char (GTK_ENTRY (core->entry_password), '*');
    gtk_box_pack_start (GTK_BOX(h_box), core->entry_password, TRUE, FALSE, 0);
    gtk_widget_show (core->entry_password);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), h_box, FALSE, FALSE, 3);
  gtk_widget_show (h_box);

  /* Login button */
  h_box = gtk_hbox_new (FALSE, 0);
    core->button_login = (GtkWidget*) libgadget_button_new_with_label ("Log in", TRUE, 80, 20, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), core->button_login, TRUE, FALSE, 0);
    gtk_widget_show (core->button_login);
  gtk_widget_show (h_box);
  gtk_box_pack_start (GTK_BOX(core->init_vbox), h_box, FALSE, FALSE, 5);
	
  /* Error label */
  core->label_error = (GtkWidget *) libui_gtk_label_new_with_descriptor ("",
                                                                         "sans center bold 6",
                                                                         TRUE,
                                                                         135,
                                                                         20,
                                                                         TRUE,
                                                                         &color);

  gtk_box_pack_start (GTK_BOX(core->init_vbox), core->label_error, FALSE, FALSE, 0);
  gtk_widget_show (core->label_error);

	g_signal_connect (G_OBJECT(core->button_login), "clicked", G_CALLBACK (vodafone_gadget_set_session_data), core);
}
