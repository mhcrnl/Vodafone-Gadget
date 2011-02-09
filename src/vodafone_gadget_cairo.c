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
 Define gadget's background
*/
gboolean
on_expose_event (GtkWidget *widget, GdkEventExpose *event, vodafone_gadget_core_s *core)
{
	cairo_t *cr;
	
	double x0      = 5.0;
	double y0      = 10.0;
	double rect_width  = core->width - 10;
	double rect_height = core->height - 15;
	double radius = 30;
	double x1, y1;
	
	cr = gdk_cairo_create (widget->window);
	
	/* Set background gadget */
	cairo_set_source_rgba (cr, 1.0f, 1.0f, 1.0f, 0.0f);
	cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
	cairo_paint (cr);

	x1 = x0 + rect_width;
	y1 = y0 + rect_height;
	
	if (!rect_width || !rect_height)
		return;

  cairo_move_to  (cr, x0, y0 + radius);
  cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
  cairo_line_to (cr, x1 - radius, y0);
  cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
  cairo_line_to (cr, x1 , y1 - radius);
  cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
  cairo_line_to (cr, x0 + radius, y1);
  cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);

	cairo_close_path (cr);

	cairo_set_source_rgba (cr, 1.0, 1.0, 1.0, 1.0);
	cairo_fill_preserve (cr);
	cairo_set_source_rgba (cr, 1.0, 0.0, 0.0, 0.0);
	cairo_set_line_width (cr, 1.0);
	cairo_stroke (cr);
	cairo_destroy(cr);
	
	return FALSE;
}

