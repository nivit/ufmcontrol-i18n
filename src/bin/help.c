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
 *	help
 *
 * DESCRIPTION
 *	This function lists the available commands.
 *
 * ARGUMENTS
 *	None.
 *
 * RETURN VALUES
 *	None.
 *
 * $Id: help.c,v 1.9 2009/04/09 08:26:34 nivit Exp $
 */

#include <ctype.h>
#include <stdio.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID HELP_C_SET

void
help(void)
{
	int		i;
	int		n;
	const char	*s;
	struct cmdtab	*c;

	(void)printf("\n\t%s:\n\n", NLS("Available commands"));

	for (c=cmdtab; c->command != CMD_NULL; ++c) {
		if (! c->args)
			continue;

		(void)printf("\t");

		for (i = c->min, s = c->name; *s; s++, i--) {
			if (i > 0)
				n = toupper((unsigned char)*s);
			else
				n = *s;
			(void)putchar(n);
		}

		if (*c->args)
			(void)printf (" %s", c->args);

		(void)printf ("\n");
	}

	(void)printf("\n\t%s\n\t%s %s\n\n",
	    NLS("The word \"set\" is not required."),
	    NLS("It is possible to specify only the"),
	    NLS("frequency."));
}
