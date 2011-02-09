/*
 * Copyright © 2010 Andrea Costa <nukemup@hotmail.com>
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

/* apt-get install libnotify1 libnotify-dev */


#include <libnotify/notify.h>
#include "vodafone_gadget.h"


void
vodafone_gadget_notify (gboolean status, char *body)
{

  NotifyNotification *n;
  n = notify_notification_new	("Linux Vodafone Gadget", body, NULL, NULL);


  if (status == TRUE)
    notify_notification_set_icon_from_pixbuf (n, libui_gtk_image_create_pixbuf_from_file (IMAGE_OK));
  else
    notify_notification_set_icon_from_pixbuf (n, libui_gtk_image_create_pixbuf_from_file (IMAGE_ERROR));



  /* It shows up after 3 seconds */
  notify_notification_set_timeout (n, 500);

  notify_notification_set_urgency (n, NOTIFY_URGENCY_CRITICAL);

  /* Show it */
  if (!notify_notification_show (n, NULL))
    g_error("Failed to send notification.\n");

}
