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


#include <stdio.h>


/*
  Decode website's errors
*/
char
*decode_error (int i)
{
	if (i == 6)
		return "Couldn't resolve host";
	else if (i == 7)
		return "Failed to connect ()";
	else if (i == 23)
		return "Writing recv data to local file";
	else if (i == 28)
		return "Timeout operazione - Riprova";
	/* Internal errors */
	else if (i == -1)
		return "Errore unknown - Riprova";
	else if (i == -2)
		return "AuthData non trovato - Riprova";
	else if (i == -3)
		return "Testo vuoto";
	else if (i == -4)
		return "Superata soglia sms disponibili";
	else if (i == -5)
		return "Destinatario non valido";
	else if (i == -6)
		return "Destinatario non Vodafone";
	else if (i == -7)
		return "Errore invio - Riprova";
	else if (i == -8)
		return "Init curl non riuscita";
	else if (i == -9)
		return "Autenticazione fallita";
	else if (i == -10)
		return "Servizio non disponibile causa\nlavori manutenzione";
	else if (i == -11)
		return "Captcha sbagliato - Riprova";
}
