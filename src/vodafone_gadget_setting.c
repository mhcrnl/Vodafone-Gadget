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


/*
  Create and runs dialog setting
*/
void
vodafone_gadget_setting_dialog_run (vodafone_gadget_core_s *core)
{	
  GtkWidget *dialog;
  GtkWidget *h_box;
  GtkWidget *v_box;
  GtkWidget *separator;
  GtkWidget *scroll;
  GtkWidget *entry_proxy;
  GtkWidget *entry_proxy_port;
  GtkWidget *check_proxy;
  GtkWidget *check_remember;
  GtkWidget *label;
  GdkColor color;
  gchar *data;
  gboolean check;

  /* Create dialog */	
  dialog = gtk_dialog_new_with_buttons ("Setting", NULL, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);
  gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW(dialog), 570, 500);
  gtk_window_set_title (GTK_WINDOW(dialog), " Setting ");
  gtk_dialog_set_has_separator (GTK_DIALOG(dialog), FALSE);
  gtk_window_set_resizable (GTK_WINDOW (dialog), TRUE);

  /* Set color */
  color.red = 0.9 * 65535;
  color.green = 0;
  color.blue = 0;

  label = libui_gtk_label_new_with_descriptor ("- PROXY -", "sans center bold 9", FALSE, 0, 0, FALSE, NULL);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), label, FALSE, FALSE, 5);

  /* Proxy h_box */
  h_box = gtk_hbox_new (FALSE, 0);

    check_proxy = gtk_check_button_new ();

    if (core->proxy_check == 1)
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(check_proxy), TRUE);
    else if (core->proxy_check == 0)
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(check_proxy), FALSE);

    gtk_box_pack_start (GTK_BOX(h_box), check_proxy, FALSE, FALSE, 10);
	
    label = libui_gtk_label_new_with_descriptor ("HTTP Proxy:", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);


    entry_proxy = libui_gtk_entry_new (TRUE,
                                       20,
                                       core->proxy,
                                       TRUE,
                                       200,
                                       20,
                                       FALSE,
                                       NULL);
    gtk_box_pack_start (GTK_BOX(h_box), entry_proxy, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("Port", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    entry_proxy_port = libui_gtk_entry_new (TRUE,
                                            4,
                                            core->proxy_port,
                                            TRUE,
                                            50,
                                            20,
                                            FALSE,
                                            NULL);

    gtk_box_pack_start (GTK_BOX(h_box), entry_proxy_port, TRUE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Remember me h_box */
  h_box = gtk_hbox_new (FALSE, 0);

    check_remember = gtk_check_button_new ();

    if (core->remember_me == 1)
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(check_remember), TRUE);
    else if (core->remember_me == 0)
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(check_remember), FALSE);

    gtk_box_pack_start (GTK_BOX(h_box), check_remember, FALSE, FALSE, 10);
	
    label = libui_gtk_label_new_with_descriptor ("Ricorda l'ultimo account utilizzato", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

	/* Separator */
  h_box = gtk_hbox_new(FALSE, 0);
    separator = gtk_hseparator_new ();
    gtk_widget_set_size_request (separator, 350, 2);
    gtk_box_pack_start (GTK_BOX(h_box), separator, TRUE, FALSE, 15);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 10);
  gtk_widget_show_all (h_box);

  label = libui_gtk_label_new_with_descriptor ("- LEGENDA ERRORI -", "sans center bold 9", FALSE, 0, 0, FALSE, NULL);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), label, FALSE, FALSE, 5);
  gtk_widget_show (label);

  /* Create scrolled window */
  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_widget_set_size_request (scroll, 300, 275);
	
  /* Create vertical_box error */
  v_box = gtk_vbox_new (FALSE, 0);
	
	h_box = gtk_hbox_new (FALSE, 0);
  label = libui_gtk_label_new_with_descriptor ("Couldn't resolve host\nFailed to connect ()\nWriting recv data to local file\nAuthData non trovato - Riprova", "sans center 9", TRUE, 180, -1, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
	
  label = libui_gtk_label_new_with_descriptor ("- connessione internet non attiva\n- problema momentaneo sito Vodafone\n- e' necessario impostare un proxy\n- il proxy impostato non e' corretto", "sans center 9", FALSE, 0, 0, FALSE, NULL);
  gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Timeout operazione - Riprova", "sans center 9", TRUE, 180, 25, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
	
   label = libui_gtk_label_new_with_descriptor ("- si e' verificata un'attesa di tempo superiore a 2 minuti in fase connessione o in ricezione dati", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_label_set_line_wrap (GTK_LABEL(label), TRUE);
    gtk_label_set_justify (GTK_LABEL(label), GTK_JUSTIFY_LEFT);

    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Errore invio - Riprova\nErrore unknown - Riprova", "sans center 9", TRUE, 180, 25, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
	
   label = libui_gtk_label_new_with_descriptor ("- si e' verificato un errore sconosciuto\n", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Testo Vuoto", "sans center 9", TRUE, 180, -1, TRUE, &color);
  	gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("- non e' possibile inviare sms senza testo", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);


  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Destinatario non valido\nDestinatario non Vodafone", "sans center 9", TRUE, 180, -1, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);
	
    label = libui_gtk_label_new_with_descriptor ("- numero del destinatario non esistente / non Vodafone", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_label_set_line_wrap (GTK_LABEL(label), TRUE);
    gtk_label_set_justify (GTK_LABEL(label), GTK_JUSTIFY_LEFT);

    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

	gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
	gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

	h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Superata soglia sms disponibili", "sans center 9", TRUE, 180, -1, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("- attualmente e' possibile inviare fino ad un  massimo di 10 sms gratis al giorno", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_label_set_line_wrap (GTK_LABEL(label), TRUE);
    gtk_label_set_justify (GTK_LABEL(label), GTK_JUSTIFY_LEFT);

    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

	h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Init curl non riuscita", "sans center 9", TRUE, 180, 25, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("- non e' stato possibile inizializzare libCURL", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

	h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Autenticazione fallita", "sans center 9", TRUE, 180, 25, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("- account/password non validi", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

	h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Captcha sbagliato - Riprova", "sans center 9", TRUE, 180, 25, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("- il codice captcha inserito non e' corretto", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  /* Separator */
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX(v_box), separator, FALSE, TRUE, 2);
  gtk_widget_show (separator);

  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("Servizio non disponibile causa\nlavori manutenzione", "sans center 9", TRUE, 180, 25, TRUE, &color);
    gtk_misc_set_alignment (GTK_MISC(label), 0.0, 0.5);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

    label = libui_gtk_label_new_with_descriptor ("- Il sito e' chiuso per manutenzione\n", "sans center 9", FALSE, 0, 0, FALSE, NULL);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 10);

  gtk_box_pack_start (GTK_BOX(v_box), h_box, FALSE, FALSE, 5);
  gtk_widget_show_all (h_box);

  gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(scroll), v_box);
  gtk_widget_show_all (v_box);
	
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), scroll, FALSE, FALSE, 5);	
  gtk_widget_show (scroll);
	
  h_box = gtk_hbox_new (FALSE, 0);
    label = libui_gtk_label_new_with_descriptor ("NB: l'applicazione sfrutta il servizio online offerto dalla Vodafone - possono essere frequenti errori nell'invio di messaggi, in particolare degli mms. Il sovraccarico del server puo' causare lentezza!", "sans 9", TRUE, 500, -1, FALSE, NULL);
    gtk_label_set_line_wrap (GTK_LABEL(label), TRUE);
    gtk_label_set_justify (GTK_LABEL(label), GTK_JUSTIFY_FILL);
    gtk_label_set_line_wrap_mode (GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
    gtk_label_set_single_line_mode (GTK_LABEL(label), FALSE);
    gtk_box_pack_start (GTK_BOX(h_box), label, FALSE, FALSE, 5);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 10);
  gtk_widget_show_all (h_box);

  /* Dialog run */
  gint result = gtk_dialog_run (GTK_DIALOG (dialog));

  switch (result)
  {
    case GTK_RESPONSE_DELETE_EVENT:
      gtk_widget_destroy (dialog);
      break;
    case GTK_RESPONSE_OK:
      /* Read check_proxy state */
      check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(check_proxy));
      if (check == TRUE)
       core->proxy_check = 1;
      else if (check == FALSE)
       core->proxy_check = 0;

    /* Read check_remember state */
    check = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(check_remember));
    if (check == TRUE)
      core->remember_me = 1;
    else if (check == FALSE)
      core->remember_me = 0;
			
    /* Read proxy */
    data = (gchar *) gtk_entry_get_text (GTK_ENTRY(entry_proxy));
    sprintf(core->proxy, "%s", data);

    /* Read proxy port */
    data = (gchar *) gtk_entry_get_text (GTK_ENTRY(entry_proxy_port));
    sprintf(core->proxy_port, "%s", data);

    vodafone_gadget_xml_update_setting (XML_FILE, core->proxy_check, core->proxy, core->proxy_port, core->remember_me, core->account);

    gtk_widget_destroy (dialog);
    break;
  } 
  return;
}
