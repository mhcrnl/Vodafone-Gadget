/*
 * Copyright © 2010
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
  Creates image button
*/
GtkWidget*
libgadget_button_new_with_image (gboolean change_button_size,
                                 int width_button,
                                 int height_button,
                                 gboolean change_image_size,
                                 int width_image,
                                 int height_image,
                                 GtkPositionType align_image,
                                 char *image_name)
{
	GtkWidget *widget;
	GtkWidget *image;
	
	widget = gtk_button_new ();

	if (change_button_size == TRUE)
		/* Set button's sizes */
		gtk_widget_set_size_request (widget, width_button, height_button);
	
	if (change_image_size == FALSE)
		/* Keep original image sizes */
		image = gtk_image_new_from_pixbuf (libui_gtk_image_create_pixbuf_from_file (image_name));
	else 
		/* Modify original image sizes */
		image = gtk_image_new_from_pixbuf (libui_gtk_image_create_pixbuf_from_file_with_size (image_name, width_image, height_image));

	/* Set image */
	gtk_button_set_image (GTK_BUTTON(widget), image);

	/* Set image position */
	gtk_button_set_image_position (GTK_BUTTON(widget), align_image);

	gtk_button_set_relief (GTK_BUTTON(widget), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click (GTK_BUTTON(widget), FALSE);
	
	return (GtkWidget *) widget;
}


/*
  Creates label button
*/
GtkWidget* libgadget_button_new_with_label (char *label, gboolean change_button_size, int width_button, int height_button, char *descriptor)
{
	GtkWidget *widget;
	
	widget = gtk_button_new_with_label (label);

	if (change_button_size == TRUE)
		/* Set button's sizes */
		gtk_widget_set_size_request (widget, width_button, height_button);

	if (descriptor != NULL)
		/* Set font */
		gtk_widget_modify_font (widget, pango_font_description_from_string (descriptor));
	
	return widget;
}
