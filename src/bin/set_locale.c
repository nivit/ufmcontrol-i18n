/*-
 * USB FM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 *
 * This software is based on and includes code of cdcontrol(1),
 * written by Serge V. Vakulenko, Jukka A. Jukkonen, Jean-Marc
 * Zucconi and Andrey A. Chernov.
 */

/*
 * NAME
 *	set_locale
 *
 * DESCRIPTION
 *	This function sets the locale.
 *
 * ARGUMENTS
 *	The name of the locale.
 *
 * RETURN VALUES
 *	 0 on successful
 *	-1 on failure
 *
 * $Id: set_locale.c,v 1.7 2009/04/09 08:26:34 nivit Exp $
 */


#include <err.h>
#include <limits.h>
#include <locale.h>
#include <string.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID	SET_LOCALE_C_SET

char	ufm_locale[NL_LANGMAX + 1];
nl_catd	nl_cd;

int
set_locale(const char * locale)
{
	size_t	len;
	nl_catd new_cd;

	if (locale != NULL)
		len = strlen(locale);
	else
		return (-1);

	if (len <= NL_LANGMAX) {

		if (setlocale(LC_ALL, locale) != NULL) {
			STRNCPY(ufm_locale, locale);

			new_cd = catopen(UFM_MESSAGE_CATALOG, NL_CAT_LOCALE);

			if (new_cd == (nl_catd) -1) {

				if (verbose > 0)
					warnx("%s %s",
					    NLS("impossible to open message catalog for locale"), ufm_locale);

				/* restore old locale? */

				return (-1);
			} else {
				/* close previous catalog */
				if (nl_cd != (nl_catd) -1)
					catclose(nl_cd);
				nl_cd = new_cd;
			}

 			set_cmdtab();

		} else {
			if (verbose > 0)
				warnx("%s (%s)",
				    NLS("impossible to set locale"), locale);
			return (-1);
		}

			return (0);
	} else {
		warnx("error: %s %d %s",
		    NLS("The name exceeds"),
		    NL_LANGMAX, NLS("bytes"));
		return (-1);
	}
}
