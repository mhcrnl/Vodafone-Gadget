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
  Create and run dialog setting
*/
void
vodafone_gadget_about (GtkMenuItem *menuitem, gpointer user_data)
{
  gchar *authors[] = {"Andrea Costa <nukemup@hotmail.com>", NULL};
	gchar* comments = {"Linux Vodafone SMS MMS gadget"};
	gchar* copyright = {"GPL General Public License"};
	gchar* website = "http://github.com/andypc";

  gtk_show_about_dialog (NULL,
                         "logo", libui_gtk_image_create_pixbuf_from_file (IMAGE_LOGO),
                         "program-name", "Vodafone Gadget",
                         "authors", authors,
                         "version", VERSION,
                         "comments", comments,
                         "copyright", copyright,
                         "website", website,
                         NULL);

}

