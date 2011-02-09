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

#ifndef _LIBUI_GTK_H_
#define _LIBUI_GTK_H_

#define LIBUI_GTK_VERSION "0.0.1"
#define LIBUI_GTK_NAME "libui_gtk"

#include <glib.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <gdk/gdk.h>

#if !GTK_CHECK_VERSION(2,9,0)
  #include <X11/Xlib.h>
  #include <X11/extensions/shape.h>
  #include <gdk/gdkx.h>
#endif
/*
#include "libui_gtk_label.h"
#include "libui_gtk_button.h"
#include "libui_gtk_image.h"
#include "libui_gtk_misc.h"
#include "libui_gtk_gadget_move.h"
#include "libui_gtk_gadget_window.h"

/* Create image button */
GtkWidget* libgadget_button_new_with_image (gboolean change_button_size, int width_button, int height_button, gboolean change_image_size, int width_image, int height_image, GtkPositionType align_image, char *image_name);

/* Create label button */
GtkWidget* libgadget_button_new_with_label (char *label, gboolean change_button_size, int width_button, int height_button, char *descriptor);

/* Create dialog error */
void libui_gtk_dialog_error (gchar *error);

/* Create entry using a descriptor */
GtkWidget* libui_gtk_entry_new_with_descriptor (gboolean set_max_size, int max_size, char *text, char *descriptor, gboolean change_size, int width_entry, int height_entry, gboolean change_text_color, GdkColor *color);

/* Create entry */
GtkWidget* libui_gtk_entry_new (gboolean set_max_size, int max_size, char *text, gboolean change_size, int width_entry, int height_entry, gboolean change_color, GdkColor *color);

/* Move the gadget around the screen */
gboolean libui_gtk_gadget_move (GtkWidget* pWidget, GdkEventButton* pButton, GdkWindowEdge edge);

/* Create main window */
GtkWidget*
libui_gtk_gadget_window_new (int width,
                             int height,
                             gboolean resizable,
                             gboolean paintable,
                             gboolean decorated,
                             gboolean stick,
                             gboolean keep_below,
                             gboolean set_modal,
                             GdkWindowTypeHint hint);

/* Create GtkStatusIcon and tooltip */
GtkWidget* libui_gtk_gadget_window_icon_new (GtkWidget *window, char *image, char *text);

/* Create a GdkPixbuf from a file */
GdkPixbuf *libui_gtk_image_create_pixbuf_from_file (const gchar * filename);

/* Create a GdkPixbuf from a file. Size required */
GdkPixbuf *libui_gtk_image_create_pixbuf_from_file_with_size (const gchar * filename, int width, int height);

/* Create label using a descriptor */
GtkWidget* libui_gtk_label_new_with_descriptor (char *text, char *descriptor, gboolean change_size, int width_label, int height_label, gboolean change_color, GdkColor *color);

/* Create label */
GtkWidget* libui_label_new (char *text, gboolean change_size, int width_label, int height_label, gboolean change_color, GdkColor *color);

#endif /* LIBUI_GTK_H_ */
