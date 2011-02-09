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
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <glib.h>


struct FtpFile
{
  const char *filename;
  FILE *stream;
};


/*
  Splits a string
*/
char *
double_split (char *str, char split1, char split2, int ret)
{
  char *ptr;
  int a, b, c = 0;

  /* Find the first split-char */
  if (ret != 0)
  {
    for (a = 0; a <= strlen (str); a++)
    {
      if (str[a] == split1 && ++c == ret)
      {
        a++;
        break;
      }
      if (a == strlen (str))
        return NULL;
    }
  }
  else
    a = 0;

  /* Find the second split-char */
  for (b = a; b <= strlen(str); b++)
    if (str[b] == split2)
      break;

  ptr = (char *) malloc (sizeof(char) * (b - a + 1));
  strncpy (ptr, &str[a], b - a);
  ptr[b - a] = 0x0;

  return (char *) ptr;
}


/*
  Create and show captcha dialog
*/
static void
vodafone_gadget_send_show_dialog_captcha (char *string)
{
  GtkWidget *dialog;
  GtkWidget *label;
  GtkWidget *image;
  GtkWidget *h_box;
  GtkWidget *entry;
	
  char *data;
  int i;

  dialog = gtk_dialog_new_with_buttons ("Setting", NULL, GTK_DIALOG_MODAL, GTK_STOCK_OK, GTK_RESPONSE_OK, NULL);

  /* Setting dialog */
  gtk_window_set_position (GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size (GTK_WINDOW(dialog), 200, 100);
  gtk_window_set_title (GTK_WINDOW(dialog), " Captcha Code ");
  gtk_window_set_icon (GTK_WINDOW(dialog), (GdkPixbuf *) libui_gtk_image_create_pixbuf_from_file (IMAGE_LOGO));
  gtk_dialog_set_has_separator (GTK_DIALOG(dialog), FALSE);
  gtk_window_set_resizable (GTK_WINDOW (dialog), TRUE);

  /* Captcha image */
  h_box = gtk_hbox_new (FALSE, 0);
    image = gtk_image_new_from_pixbuf ((GdkPixbuf *) libui_gtk_image_create_pixbuf_from_file (IMAGE_VERIFY));
    gtk_box_pack_start (GTK_BOX(h_box), image, TRUE, FALSE, 10);
    gtk_widget_show (image);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
  gtk_widget_show (h_box);
	
  /* Captcha entry */
  h_box = gtk_hbox_new (FALSE, 0);
    entry = gtk_entry_new_with_max_length (10);
    gtk_widget_modify_font (entry, pango_font_description_from_string ("sans 7"));
    gtk_widget_set_size_request (entry, 60, 20);
    gtk_box_pack_start (GTK_BOX(h_box), entry, TRUE, FALSE, 10);
    gtk_widget_show (entry);
  gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dialog)->vbox), h_box, FALSE, FALSE, 5);
  gtk_widget_show (h_box);

  /* Dialog run */
  gint result = gtk_dialog_run (GTK_DIALOG (dialog));

  switch (result)
  {
    case GTK_RESPONSE_DELETE_EVENT:
      gtk_widget_destroy (dialog);
      break;
    case GTK_RESPONSE_OK:
      /* Read captcha */
      data = (gchar *) gtk_entry_get_text (GTK_ENTRY(entry));
      sprintf (string, "%s", data);
      gtk_widget_destroy (dialog);
      break;
  } 
	return;
}


/*
  Save stuff -> libCURL
*/
static size_t
my_fwrite (void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out=(struct FtpFile *) stream;
  char *ptr;
	
  if(out && !out->stream)
  {
    /* open file for writing */
    out->stream = fopen (out->filename, "w");
    if(!out->stream)
      /* Failure, can't open file to write */
      return -1;
  }
  return fwrite (buffer, size, nmemb, out->stream);
}


int
convert (char *stringa, char *sendstringa)
{	
  int i;
  int j = 0;
	
  for (i = 0; i < strlen(stringa) - 2; i++)
  {
    if ((int)stringa[i] == 37)
    {
      sendstringa[j] = stringa[i];
      j++;
      sendstringa[j] = '2';
      j++;
      sendstringa[j] = '5';
    }
    else if ((int)stringa[i] == 176)
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '7';
      j++;
      sendstringa[j] = 'E';
    }
    else if ((int)stringa[i] == 39) // '
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '2';
      j++;
      sendstringa[j] = '7';
    }
    else if ((int)stringa[i] == 40) // (
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '2';
      j++;
      sendstringa[j] = '8';
    }
    else if ((int)stringa[i] == 41) // )
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '2';
      j++;
      sendstringa[j] = '9';
    }
    else if ((int)stringa[i] == 42) // *
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '2';
      j++;
      sendstringa[j] = 'A';
    }
    else if ((int)stringa[i] == 43) // +
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '2';
      j++;
      sendstringa[j] = 'B';
    }
    else if ((int)stringa[i] == 91) // [
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '5';
      j++;
      sendstringa[j] = 'B';
    }
    else if ((int)stringa[i] == 93) // ]
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '5';
      j++;
      sendstringa[j] = 'D';
    }
    else if ((int)stringa[i] == 94) // ^
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '5';
      j++;
      sendstringa[j] = 'E';
    }
/*
    else if ((int)stringa[i] == 95) // _
    {
      sendstringa[j] = '%';
      j++;
      sendstringa[j] = '5';
      j++;
      sendstringa[j] = 'F';
    }
*/
    else
      sendstringa[j] = stringa[i];

    j++;
  }

  sendstringa[j] = '%';
  j++;
  sendstringa[j] = '0';
  j++;
  sendstringa[j] = 'D';
  j++;
  sendstringa[j] = '\0';	

  return 0;
}


/*
  Check if the account is correct
*/
int
vodafone_gadget_verify_session (vodafone_gadget_core_s *core)
{
  CURL *curl;
  CURLcode res;
  curl_global_init (CURL_GLOBAL_ALL);
  curl = curl_easy_init ();
  FILE *file;
  char *filename = "result.txt";
  char buffer[1024];
  char autenticazione[150];
  char *ptr = NULL;
  int i = 0;
	char proxy[30];
  core->auth = 0;

  if(curl)
  {
    /* Cookies */
    curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");
    /* Encoding required */
    curl_easy_setopt (curl, CURLOPT_ENCODING, "");
    /* No certificate SSL */
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0);
    /* Follow any location */
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
    /* Two minute timeout transfer process */
    curl_easy_setopt (curl, CURLOPT_TIMEOUT, 120);
    /* Two minute timeout connection process */
    curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT, 120);
		
    /* Check if proxy is required */
    if (core->proxy_check == 1)
    {
    	/* Create proxy in the right format */
    	snprintf (proxy, 30, "http://%s:%s", core->proxy, core->proxy_port);
      g_print ("* Using proxy: %s\n", proxy);
      curl_easy_setopt (curl, CURLOPT_PROXY, proxy);
    }

    g_print ("* Authentifying session...\n");
			
    /* Authentify session */
    sprintf (autenticazione, "username=%s&password=%s", core->account, core->password);
    file = fopen (filename, "w");
    curl_easy_setopt (curl, CURLOPT_URL, "http://www.vodafone.it/190/trilogy/jsp/login.do");
    curl_easy_setopt (curl, CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, &autenticazione);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of error */
    if(CURLE_OK != res)
    {
      g_print ("* Error authentifying session - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    g_print ("* Checking session...\n");
			    
    /* Verify if i'am logged in */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "Ciao: <b><!-- TY_DISP -->")) != NULL)
      {
        core->name = (char *) double_split (ptr, '>', '<', 2);
        core->auth = 1;
        break;
      }
      else if ((ptr = strstr (buffer, "Puoi comunque accedere alla tua")) != NULL)
      {
        /* Vodafone website -> work in progress -> service broken */
        return -10;
      }
    }
    fclose (file);

    /* Delete result.txt */
    remove (filename);

    /* Return if i'm NOT logged in */
    if (core->auth == 0)
    {
      g_print ("* ERROR: authentification failed!\n");
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return -9;
    }
    else
    {
      g_print ("* Logged in!\n\n");
      return 0;
		}
	    
  } /* END session->auth == 0 */
  else
    /* Generic error */
    return -1;
	
  /* Error init curl */
  return -8;
}


/*
  Send MMS
*/
int
vodafone_gadget_send_mms (vodafone_gadget_core_s *core)
{	
  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  struct FtpFile ftpfile = {
    "result.txt",
    NULL
  };


  FILE *file;
  char *filename = "result.txt";
  char buffer[1024];
  char autenticazione[150];
  char messaggio[1024];
  char *ptr = NULL;
  int i = 0;
  char request[400];
  char sendstring[1024];
  char proxy[30];
  core->auth = 0;

  if(curl)
  {
    /* Cookies */
    curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");
    /* Encoding required */
    curl_easy_setopt (curl, CURLOPT_ENCODING, "");
    /* No SSL */
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0);
    /* Follow any location */
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
    /* Two minute timeout transfer process */
    curl_easy_setopt (curl, CURLOPT_TIMEOUT, 120);
    /* Two minute timeout connection process */
    curl_easy_setopt (curl, CURLOPT_CONNECTTIMEOUT, 120);
		
    /* Set proxy */
    if (core->proxy_check == 1)
    {
    	/* Create proxy in the right format */
    	snprintf (proxy, 30, "http://%s:%s", core->proxy, core->proxy_port);
      g_print ("* Using proxy: %s\n", proxy);
      curl_easy_setopt (curl, CURLOPT_PROXY, proxy);
    }
		    
    g_print ("* Authentifying session...\n");
			
    /* Authentifying session */
    sprintf (autenticazione, "username=%s&password=%s", core->account, core->password);
    file = fopen (filename, "w");
    curl_easy_setopt (curl, CURLOPT_URL, "http://www.vodafone.it/190/trilogy/jsp/login.do");
    curl_easy_setopt (curl, CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, &autenticazione);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print (" Error authentifying session - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    /* Look for my name... */

    /* Verify if i'am logged in */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "Ciao: <b><!-- TY_DISP -->")) != NULL)
      {
        core->auth = 1;
        break;
      }
      else if ((ptr = strstr (buffer, "Puoi comunque accedere alla tua")) != NULL)
      {
        /* Vodafone website -> work in progress -> service broken */
        return -10;
      }
    }   
    fclose (file);
		    
    /* Delete result.txt */
    remove (filename);

    /* Return if i'm not logged */
    if (core->auth == 0)
    {
      g_print ("* ERROR: authentification failed!\n");
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return -9;
    }
    else
			g_print ("* Logged in!\n");

    g_print ("* Visiting init form...\n");
		
    /* Visit init form */
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_URL, "http://www.areaprivati.vodafone.it/190/trilogy/jsp/dispatcher.do?ty_key=mmlsendmms&tk=9609,c");
    curl_easy_setopt (curl,CURLOPT_POST, 0);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print ("* ERROR: init form - code: %d\n", res);
      core->auth = 0;
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    /* Delete result.txt */
    remove (filename);
		 
    g_print ("* Getting AuthData...\n");

    /* Visit MMS form */
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_URL, "http://mmsviaweb.net.vodafone.it/WebComposer/web/mms.jsp");
    curl_easy_setopt (curl,CURLOPT_POST, 0);
    curl_easy_setopt (curl, CURLOPT_HEADERFUNCTION , my_fwrite);
    curl_easy_setopt (curl, CURLOPT_WRITEHEADER, &ftpfile);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print ("* ERROR: MMS form - code: %d\n", res);
      core->auth = 0;
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }
    if (ftpfile.stream)
      fclose (ftpfile.stream);

    i = 0;
		    
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "AuthData=%")) != NULL)
      {
        sprintf (request, "%s", ptr);
        i++;
        break;
      }
    }
    fclose (file);
		    
    /* Delete result.txt */
    remove (filename);

    /* Return if i didn't get authdata */
    if (i == 0)
    {
      g_print ("* ERROR: AuthData missing!\n");
      core->auth = 0;
      curl_easy_cleanup (curl);
      curl_global_cleanup();
      return -2;
    }

    /* Convert AuthData in application/x-www-form-urlencoded */
    convert (request, sendstring);

    /* Delete previous settings */
    curl_easy_reset (curl);

    /* Set settings */
    curl_easy_setopt (curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt (curl, CURLOPT_ENCODING, "");
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1);
    /* Set proxy */
    if (core->proxy_check == 1)
    {
    	/* Create proxy in the right format */
    	snprintf (proxy, 30, "http://%s:%s", core->proxy, core->proxy_port);
      g_print ("* Using proxy: %s\n", proxy);
      curl_easy_setopt (curl, CURLOPT_PROXY, proxy);
    }
		
    g_print ("* Sending AuthData...\n");

    /* Send AuthData */
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_URL, "http://mmsviaweb.net.vodafone.it/WebComposer/web/mms.jsp");
    curl_easy_setopt (curl,CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, &sendstring);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDSIZE , strlen(sendstring));
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print (" Error send AuthData - code: %d\n", res);
      core->auth = 0;
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    /* Delete result.txt */
    remove (filename);

    g_print ("* Popup...\n");

    /* Send MMS step 1 */
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_URL, "http://mmsviaweb.net.vodafoneomnitel.it/WebComposer/web/popup.jsp?Caso=7");
    curl_easy_setopt (curl,CURLOPT_POST, 0);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print (" Error sens MMS step 1 - code: %d\n", res);
      core->auth = 0;
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }
		
    /* Look for errors */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "Ti ricordiamo che puoi inviare SMS via Web solo a numeri di cellulare Vodafone")) != NULL)
      {
        core->auth = 0;
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -6;
      }
      else if ((ptr = strstr (buffer, "Il numero di telefono del destinatario del messaggio non")) != NULL)
      {
        core->auth = 0;
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -5;
      }
    }
    fclose (file);
		    
    /* Delete result.txt */
    remove (filename);

    /* Send MMS step 2 */
    sprintf (messaggio, "recipient=+39%s&subjecttosend=%s&SmilName=&TextName=%s&ImageName=&AudioName=&nextPage=/web/servletresult.html", core->number, core->mms_subject, core->text_message);
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt (curl,CURLOPT_POSTFIELDS, &messaggio);
    curl_easy_setopt (curl,CURLOPT_URL, "http://mmsviaweb.net.vodafoneomnitel.it/WebComposer/web/elaborapop.jsp");
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print (" Error send MMS step 2 - code: %d\n", res);
      core->auth = 0;
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }
  
    i = 0;
		    
    /* Look for send conf */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "Il tuo messaggio &egrave; stato inviato")) != NULL)
					i++;

      else if ((ptr = strstr (buffer, "si e' verificato un errore durante la procedura")) != NULL)
      {
        curl_global_cleanup ();
        curl_easy_cleanup (curl);
        return -7;
      }
    }
    fclose (file);
		    
    /* Delete result.txt */
    remove (filename);

    if (i == 0)
    {
      g_print ("* Message NOT send!\n");
      return -1;
    }
    else
    g_print ("* MMS send!\n");

		core->auth = 0;	
    curl_easy_cleanup (curl);
  }
  curl_global_cleanup ();

  return 0;
}
	

/*
  Send SMS
*/
int
vodafone_gadget_send_sms (vodafone_gadget_core_s *core)
{
  CURL *curl;
  CURLcode res;		
  curl_global_init (CURL_GLOBAL_ALL);
  curl = curl_easy_init ();

  FILE *file;
  char *filename = "result.txt";
  char buffer[1024];
  char autenticazione[150];
  char messaggio[500];
  char messaggio_verify[500];
  char verify_code[10];
  char proxy[30];
  char *ptr = NULL;
  int i = 0;
  core->auth = 0;

	if(curl)
  {
    /* Cookies */
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    /* Encoding required */
    curl_easy_setopt(curl, CURLOPT_ENCODING, "");
    /* No SSL */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    /* Follow any location */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    /* Two minute timeout transfer process */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120);
    /* Two minute timeout connection process */
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 120);

    /* Set proxy */
    if (core->proxy_check == 1)
    {
    	/* Create proxy in the right format */
    	snprintf (proxy, 30, "http://%s:%s", core->proxy, core->proxy_port);
      g_print ("* Using proxy: %s\n", proxy);
      curl_easy_setopt (curl, CURLOPT_PROXY, proxy);
    }

    g_print ("* Authentifying session...\n");
			
    /* Authentify session */
    sprintf (autenticazione, "username=%s&password=%s", core->account, core->password);
    file = fopen (filename, "w");
    curl_easy_setopt (curl, CURLOPT_URL, "http://www.vodafone.it/190/trilogy/jsp/login.do");
    curl_easy_setopt (curl, CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, &autenticazione);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

		/* Return in event of errors */
		if (CURLE_OK != res)
    {
      g_print ("ERROR: authentifying failed - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }

    /* Verify if i'm logged correctly */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "Ciao: <b><!-- TY_DISP -->")) != NULL)
      {
        core->auth = 1;
        break;
      }
      else if ((ptr = strstr (buffer, "Puoi comunque accedere alla tua")) != NULL)
      {
        /* Vodafone website -> work in progress -> service broken */
        return -10;
      }
    }   
    fclose (file);
		    
    /* Delete result.txt */
    remove (filename);

    /* Return if i'm not logged */
    if (core->auth == 0)
    {
      g_print ("Error: authentification failed\n");
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return -9;
    }
		    
    g_print ("* Visiting advertisements...\n");
		    
    /* Visit advertisements */
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_URL, "http://www.vodafone.it/190/trilogy/jsp/dispatcher.do?ty_key=fdt_invia_sms&tk=9616,2");
    curl_easy_setopt (curl,CURLOPT_POST, 0);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if (CURLE_OK != res)
    {
      g_print ("Error - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }
		    
    /* Delete result.txt */
    remove (filename);

    g_print ("* Visiting SMS form...\n");
		    
    /* Visit SMS form */
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_URL, "http://www.areaprivati.vodafone.it/190/trilogy/jsp/dispatcher.do?ty_key=fsms_hp&ipage=next");
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if(CURLE_OK != res)
    {
      g_print ("Error - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }
		    
    /* Delete result.txt */
    remove (filename);

    g_print ("* Sending SMS without CAPTCHA...\n");
		    
    /* Send SMS */
    sprintf (messaggio, "pageTypeId=9604&programId=10384&chanelId=-18126&receiverNumber=%s&message=%s", core->number, core->text_message);
    file = fopen (filename, "w");
    curl_easy_setopt (curl,CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, &messaggio);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt (curl,CURLOPT_URL, "http://www.areaprivati.vodafone.it/190/fsms/prepare.do");
    res = curl_easy_perform (curl);
    fclose (file);
		       
    /* Return in event of errors */
    if(CURLE_OK != res)
    {
      g_print ("Error - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup ();
      return res;
    }
		       
    /* Look for errors */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "Il campo messaggio deve contenere almeno un carattere")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -3;
      }
      if ((ptr = strstr (buffer, "box_sup_limitesms.gif")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -4;
      }
      if ((ptr = strstr (buffer, "Hai superato il limite giornaliero di SMS via Web")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -4;
      }
      else if ((ptr = strstr (buffer, "Il numero di telefono del destinatario del messaggio non&nbsp;&egrave; valido")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -5;
      }
      else if ((ptr = strstr (buffer, "Ti ricordiamo che puoi inviare SMS via Web solo a numeri di cellulare Vodafone")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        return -6;			
      }
    }   
    fclose (file);
		       
    /* Delete result.txt */
    remove (filename);

    g_print ("* Saving CAPTCHA...\n");

    file = fopen (IMAGE_VERIFY, "wb");
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);		       
    curl_easy_setopt (curl,CURLOPT_POST, 0);      
    curl_easy_setopt (curl,CURLOPT_URL, "http://www.areaprivati.vodafone.it/190/fsms/generateimg.do");
    res = curl_easy_perform (curl);
    fclose (file);

    g_print ("* Opening CAPTCHA dialog...\n");
		       
    gdk_threads_enter ();
      /* Open dialog captcha */
      vodafone_gadget_send_show_dialog_captcha (verify_code);
    gdk_flush ();
    gdk_threads_leave ();
		       		      
    g_print ("* Sending message with CAPTCHA...\n");
		       
    sprintf (messaggio_verify, "verifyCode=%s&pageTypeId=9604&programId=10384&chanelId=-18126&receiverNumber=%s&message=%s", verify_code, core->number, core->text_message);
    file = fopen (filename, "w");			    		    
    curl_easy_setopt (curl,CURLOPT_POST, 1);
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, &messaggio_verify);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt (curl,CURLOPT_URL, "http://www.areaprivati.vodafone.it/190/fsms/send.do");
    res = curl_easy_perform (curl);
    fclose (file);

    /* Return in event of errors */
    if(CURLE_OK != res)
    {
      g_print (" Error - code: %d\n", res);
      curl_easy_cleanup (curl);
      curl_global_cleanup();
      return res;
    }
		       
    /* Look for send conf */
    file = fopen (filename, "r");
    while (fgets (buffer, sizeof (buffer) - 1, file) != NULL)
    {
      if ((ptr = strstr (buffer, "La tua richiesta&nbsp;&egrave; stata elaborata correttamente")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        remove (filename);
        return 0;
      }
      if ((ptr = strstr (buffer, "Inserisci il codice che visualizzi a lato nel box e seleziona invia")) != NULL)
      {
        curl_easy_cleanup (curl);
        curl_global_cleanup ();
        remove (filename);
        return -11;
      }
    }
    fclose (file);
		       
    /* Delete result.txt */
    remove (filename);
		       
    curl_easy_cleanup (curl);
  }
  curl_global_cleanup ();
  return -1;
}
