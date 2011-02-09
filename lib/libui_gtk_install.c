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

#include <sys/stat.h>
#include <sys/stat.h>

#include "libui_gtk.h"


/*
 Checks the presence of the given folder
 */
gboolean
libgadget_install_check_directory (char *folder)
{
	struct stat sbuf;
	gchar *path;

	if (lstat (folder, &sbuf) == -1)
		return FALSE;

	if (S_ISDIR (sbuf.st_mode))
		return TRUE;
}


/*
 Creates a folder
*/
gboolean
libgadget_install_create_folder (char *folder_path, gboolean rewrite)
{
	if (rewrite == TRUE)
		remove (folder_path);

	if (mkdir (folder_path, 0755) == -1)
		return FALSE;
	else
		return TRUE;
}


/*
 Checks versions
 Returns 1 if the version is the same
 Returns 2 if the original version is older
 Returns 3 if the original version is "newer"
 Returns -1 in event of errors or if the file doesn't exist
*/
int
libgadget_install_check_version (char *version_file, char *version)
{
	char *original_version;
	FILE *file;
	char *ptr;
	char *ptr1;
	char string[100];

	original_version = (char *) malloc (10);

	file = fopen (version_file, "r");
	
	if (file != NULL) {
		/* The file exists */
		while (fgets (string, sizeof (string) - 1, file) != NULL) {
			if (strncmp (string, "VERSION:", 8) == 0)
				ptr = (char *) libgadget_misc_double_split (string, ' ', '\n', 1);
				sprintf (original_version, "%s", ptr);
		}
		fclose(file);

		g_print ("  existing version %s\n", original_version);
		g_print ("  new version %s\n", version);

		if (strncmp (original_version, version, strlen (version)) == 0)
			/* Is the same version */
			return 1;

		ptr = (char *) libgadget_misc_double_split (version, ' ', '.', 0);
		ptr1 = (char *) libgadget_misc_double_split (original_version, ' ', '.', 0);

		if (atoi(ptr) > atoi(ptr1))
			/* The original version is newer */
			return 3;
		else if (atoi(ptr) < atoi(ptr1))
			/* The original version is older */
			return 2;
		else {
			ptr = (char *) libgadget_misc_split (version, '.', 1);
			ptr1 = (char *) libgadget_misc_split (original_version, '.', 1);
			
			if (atoi(ptr) > atoi(ptr1))
				/* The original version is newer */
				return 3;
			else if (atoi(ptr) < atoi(ptr1))
				/* The original version is older */
				return 2;
			else {
				
				ptr = (char *) libgadget_misc_double_split (version, '.', '\n', 2);
				ptr1 = (char *) libgadget_misc_double_split (original_version, '.', '\n', 2);
				
				if (atoi(ptr) > atoi(ptr1))
					/* The original version is newer */
					return 3;
				else if (atoi(ptr) < atoi(ptr1))
					/* The original version is older */
					return 2;
				else {
					/* Something went deeply wrong :D! */
					return -1;
				}
			}
		}
	}
	else
		return -1;
}


/*
 Creates version file
*/
int
libgadget_install_create_version_file (char *version_file, char *version, gboolean overwrite)
{
	FILE *file;

	if (overwrite == TRUE)
		/* Overwrite version_file */
		file = fopen (version_file, "w");
	else {
		file = fopen (version_file, "r");
		if (file != NULL) {
			/* The file exists -> don't overwrite -> return */
			fclose(file);
			return 2;
		}
	}
		
	if (file == NULL) {
		return -1;
	}
	else {
		fprintf (file, "VERSION: %s\n", version);
		fclose (file);
		return 1;
	}
}


/*
 Returns program's version (Must be a string - es: 'VERSION: 2.4.1')
*/
char
*libgadget_install_get_version (char *version_file)
{
	char *return_string;
	FILE *file;
	char *ptr;
	char string[512];

	return_string = (char *) malloc (10);

	file = fopen (version_file, "r");
	
	if (file != NULL) {
		while (fgets (string, sizeof (string) - 1, file) != NULL){
			if (strncmp (string, "VERSION:", 8) == 0)
				ptr = (char *) libgadget_misc_double_split (string, ' ', '\n', 1);
				sprintf (return_string, "%s", ptr);
		}
		fclose(file);
		return strdup (return_string);
	}
	else 
		return NULL;
}


/*
 Copies file from one place to another one
 Returns 1 if everything goes right
 Returns 2 if the file already exists and we don't want to replace it
 Returns -1 in event of errors
*/
int
libgadget_install_copy_file (char *original_file, char *copy_file, gboolean remove_original, gboolean replace)
{
	FILE *file_original, *file_copy;
	size_t len = 0 ;
	char *buffer;
	long lSize;
	int result;

	/* Check if the file exixts */
	file_copy = fopen (copy_file, "rb");
	if (file_copy != NULL) {
		fclose (file_copy);
		if (replace == FALSE)
			/* The file exists -> don't overwrite -> return */
			return 2;
		else
			/* The file exists -> overwrite it */
			remove (copy_file);
	}

	file_original = fopen (original_file, "rb");
	file_copy = fopen (copy_file, "wb");
	
	if ((file_original != NULL) || (file_copy != NULL)) {
		/* Obtain file size */
		fseek (file_original , 0 , SEEK_END);
		lSize = ftell (file_original);
		rewind (file_original);

		/* Allocate memory to contain the whole file */
		buffer = (char*) malloc (sizeof (char) * lSize);

		result = fread (buffer, 1, lSize, file_original);
		fwrite (buffer, lSize, 1, file_copy);

		fclose(file_original);
		fclose(file_copy);
		free (buffer);

		if (remove_original == TRUE)
			remove (original_file);
		
		return 1;
	}
	else
		return -1;
}
