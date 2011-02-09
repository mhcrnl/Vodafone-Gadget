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

#include <libxml/parser.h>
#include <libxml/xinclude.h>
#include <libxml/tree.h>
#include <libxml/xmlIO.h>
#include <string.h>


/*
  Get general info data
*/
static void
vodafone_gadget_xml_parse_info (xmlDocPtr doc, xmlNodePtr cur, vodafone_gadget_core_s *core)
{
  xmlChar *key;
  cur = cur->xmlChildrenNode;
  while (cur != NULL)
  {
    /* Save app's name - Max 20 char */
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "name")))
    {
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      core->name = (char *) malloc (20);
      snprintf (core->name, 20, "%s", (char *) key);
      xmlFree (key);
    }
    /* Save app's version - Max 10 char */
    else if ((!xmlStrcmp (cur->name, (const xmlChar *) "version")))
    {
			core->version = (char *) malloc (10);
      key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
      snprintf (core->version, 10, "%s", (char *) key);
      xmlFree (key);
    }

    cur = cur->next;
  }
  return;
}


/*
  Get graphics data
*/
static void
vodafone_gadget_xml_parse_graphics (xmlDocPtr doc, xmlNodePtr cur, vodafone_gadget_core_s *core)
{
  xmlChar *key;
  cur = cur->xmlChildrenNode;
  while (cur != NULL)
  {
    /* Save app's width */
		if ((!xmlStrcmp (cur->name, (const xmlChar *) "width")))
    {
			key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
			core->width = atoi (key);
			xmlFree (key);
 		}
    /* Save app's height */
		else if ((!xmlStrcmp (cur->name, (const xmlChar *) "height")))
    {
			key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
			core->height = atoi (key);
			xmlFree (key);
 		}
		cur = cur->next;
	}
  return;
}


/*
 Get all XML configuration data
*/
int
vodafone_gadget_xml_parse (vodafone_gadget_core_s *core,
                           char *sidegadget_xml_file,
                           gboolean get_info,
                           gboolean get_author,
                           gboolean get_graphics,
                           gboolean get_setting)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL)
    return -1;

  cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "vodafone-gadget"))
  {
    xmlFreeDoc (doc);
    return -1;
  }

  core->width = -1;
  core->height = -1;
  core->x_window = -1;
  core->y_window = -1;

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {
    if ((!xmlStrcmp (cur->name, (const xmlChar *) "info")))
    {
      if (get_info == TRUE)
  			vodafone_gadget_xml_parse_info (doc, cur, core);
    }
		else if ((!xmlStrcmp (cur->name, (const xmlChar *) "graphics")))
    {
      if (get_graphics == TRUE)
      {
        core->x_window = atoi ((char *) xmlGetProp (cur, "x_win"));
        core->y_window = atoi ((char *) xmlGetProp (cur, "y_win"));
        vodafone_gadget_xml_parse_graphics (doc, cur, core);

      }
		}
		else if ((!xmlStrcmp (cur->name, (const xmlChar *) "setting")))
    {
      if (get_setting == TRUE)
      {
        /* Proxy's status */
        core->proxy_check = atoi ((char *) xmlGetProp (cur, "proxy_on"));
        /* Proxy - Max 21 char */
        snprintf (core->proxy, 21, "%s", (char *) xmlGetProp (cur, "proxy"));
        /* Proxy port - Max 5 char */
        snprintf (core->proxy_port, 5, "%s", (char *) xmlGetProp (cur, "proxy_port"));
        /* Remember me */
        core->remember_me = atoi ((char *) xmlGetProp (cur, "remember_me"));
        /* Username - Max 20 char */
        snprintf (core->account, 20, "%s", (char *) xmlGetProp (cur, "username"));
      }
		}

		cur = cur->next;
	}

  /* Print app's info */
  g_print ("*  version     %s\n", core->version);

	return 0;
}


/*
 Update XML configuration file
*/
int
vodafone_gadget_xml_update_position (char *sidegadget_xml_file, int x_win, int y_win)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL )
  {
    libui_gtk_dialog_error ("XML Configuration - Error parsing xml file.");
    return -1;
  }

	cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    libui_gtk_dialog_error ("XML Configuration - File empty.");
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "vodafone-gadget"))
  {
    libui_gtk_dialog_error ("XML Configuration\nRoot node \"vodafone-gadget\" is missing.");
    xmlFreeDoc (doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {

    if ((!xmlStrcmp(cur->name, (const xmlChar *) "graphics")))
    {
      xmlSetProp (cur, "x_win", g_strdup_printf ("%d", x_win));
      xmlSetProp (cur, "y_win", g_strdup_printf ("%d", y_win));
    }
    cur = cur->next;
  }

  if (doc != NULL)
  {
    xmlSaveFormatFile (sidegadget_xml_file, doc, 0);
    xmlFreeDoc (doc);
  }
	return 0;
}

/*
 Update XML configuration file
*/
int
vodafone_gadget_xml_update_remember_me (char *sidegadget_xml_file, char *username)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL )
  {
    libui_gtk_dialog_error ("XML Configuration - Error parsing xml file.");
    return -1;
  }

	cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    libui_gtk_dialog_error ("XML Configuration - File empty.");
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "vodafone-gadget"))
  {
    libui_gtk_dialog_error ("XML Configuration\nRoot node \"vodafone-gadget\" is missing.");
    xmlFreeDoc (doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {

    if ((!xmlStrcmp(cur->name, (const xmlChar *) "setting")))
    {
      xmlSetProp (cur, "remember_me", "1");
      xmlSetProp (cur, "username", g_strdup_printf ("%s", username));
    }
    cur = cur->next;
  }

  if (doc != NULL)
  {
    xmlSaveFormatFile (sidegadget_xml_file, doc, 0);
    xmlFreeDoc (doc);
  }
	return 0;
}


/*
 Update XML configuration file
*/
int
vodafone_gadget_xml_update_setting (char *sidegadget_xml_file, int proxy_check, char *proxy, char *proxy_port, int remember_me, char *username)
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile (sidegadget_xml_file);
	
  if (doc == NULL )
    return -1;

	cur = xmlDocGetRootElement (doc);

  if (cur == NULL)
  {
    xmlFreeDoc (doc);
    return -1;
  }
	
  if (xmlStrcmp (cur->name, (const xmlChar *) "vodafone-gadget"))
  {
    xmlFreeDoc (doc);
    return -1;
  }

  cur = cur->xmlChildrenNode;

  while (cur != NULL)
  {

    if ((!xmlStrcmp(cur->name, (const xmlChar *) "setting")))
    {
      xmlSetProp (cur, "proxy_on", g_strdup_printf ("%d", proxy_check));
      xmlSetProp (cur, "proxy", proxy);
      xmlSetProp (cur, "proxy_port", proxy_port);
      xmlSetProp (cur, "remember_me", g_strdup_printf ("%d", remember_me));
      if (remember_me == 1)
        xmlSetProp (cur, "username", g_strdup_printf ("%s", username));
      else
        xmlSetProp (cur, "username", "");
    }
    cur = cur->next;
  }

  if (doc != NULL)
  {
    xmlSaveFormatFile (sidegadget_xml_file, doc, 0);
    xmlFreeDoc (doc);
  }
	return 0;
}


