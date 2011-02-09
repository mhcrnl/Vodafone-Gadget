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

#ifndef __VODAFONE_GADGET_H__
#define __VODAFONE_GADGET_H__

#include "../lib/libui_gtk.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "ephy-spinner.h"
#include "vodafone_gadget_errors.h"

#define DATA_FILE "data.txt"
#define IMAGE_CLOSE "images/close.png"
#define IMAGE_MOVE "images/move.png"
#define IMAGE_SETTING "images/setting.png"
#define IMAGE_VERIFY "images/verify_code.jpg"
#define IMAGE_LOGO "images/logo.png"
#define IMAGE_ERROR "images/error.png"
#define IMAGE_OK "images/ok.png"
#define IMAGE_LOADING "images/loading.gif"
#define STYLE "style/style.rc"
#define XML_FILE "vodafone-gadget.xml"
#define VERSION "2.1.0"

typedef struct {
	/* Gadget */
	GtkWidget *button_close;
	GtkWidget *button_move;
	GtkWidget *button_setting;
	GtkWidget *window;
	GtkWidget *icon;
	GtkWidget *icon_menu;
	GtkWidget *quit_item;
	GtkWidget *about_item;
	GtkWidget *fixed;
	GtkWidget *spinner;
	/* Connection settings */
	char account[20];
	char password[10];
	char proxy[21];
	char proxy_port[5];
	int remember_me;
	int proxy_check;
	char *name;
	int auth;
	char *number;
	char mms_subject[30];
	char text_message[501];
	/* Init area */
	GtkWidget *init_vbox;
	GtkWidget *button_login;
	GtkWidget *label_error;
	GtkWidget *entry_account;
	GtkWidget *entry_password;
	/* Logged area */
	GtkWidget *logged_vbox;
	GtkWidget *button_send;
	GtkWidget *scrolled_win;
	GtkTextBuffer *buffer;
	GtkWidget *textview;
	GtkWidget *label_char_left;
	GtkWidget *label_notify;
	GtkWidget *combobox_send;
	GtkWidget *entry_subject;
	GtkWidget *entry_mobile_number;
	int width;
	int height;
	int x_window;
	int y_window;
	gboolean presence;
  char *version;
} vodafone_gadget_core_s;

#endif /* __VODAFONE_GADGET_H__ */
