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


#include "libui_gtk.h"


/*
  Creates label
*/
GtkWidget* libui_gtk_label_new (char *text, gboolean change_size, int width_label, int height_label, gboolean change_color, GdkColor *color)
{
  GtkWidget *widget;
  widget = gtk_label_new (text);

  /* Set label's sizes */
  gtk_widget_set_size_request (widget, width_label, height_label);

  if (color != NULL)
    /* Set text color */
    gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, color);

  return widget;
}


/*
  Creates label using a descriptor
*/
GtkWidget* libui_gtk_label_new_with_descriptor (char *text, char *descriptor, int width_label, int height_label, GdkColor *color)
{
  GtkWidget *widget;
  widget = gtk_label_new (text);

  /* Modify font */
  gtk_widget_modify_font (widget, pango_font_description_from_string (descriptor));
	
  /* Set label's sizes */
  gtk_widget_set_size_request (widget, width_label, height_label);

  if (color != NULL)
    /* Set text color */
    gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, color);
	
  return widget;
}
