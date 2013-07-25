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
 * $Id: parse.c,v 1.8 2009/04/09 08:26:34 nivit Exp $
 */

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID PARSE_C_SET

char *
parse(char *buf, int *cmd)
{
	char		*p;
	unsigned int	len;
	struct cmdtab	*c;

	for (p=buf; isspace (*p); p++)
		continue;

	if (isdigit (*p) != 0) {
		*cmd = CMD_SET_FREQ;
		return (p);
	}

	for (buf = p; *p != '\0' && ! isspace (*p); p++)
		continue;

	len = p - buf;
	if (! len)
		return (0);

	if (*p != '\0') {	/* It must be a spacing character! */
		char *q;

		*p++ = 0;
		for (q=p; *q && *q != '\n' && *q != '\r'; q++)
			continue;
		*q = 0;
	}

	*cmd = -1;
	for (c=cmdtab; c->command != CMD_NULL; ++c) {
		/* Is it an exact match? */
		if (! strcasecmp (buf, c->name)) {
  			*cmd = c->command;
  			break;
  		}

		/* Try short hand forms then... */
		if (len >= c->min && ! strncasecmp (buf, c->name, len)) {

			if (*cmd != -1 && *cmd != c->command) {
				warnx("%s", NLS("ambiguous command"));
				return (0);
			}
			*cmd = c->command;
  		}
	}

	if (*cmd == -1) {
		warnx("%s", NLS("invalid command, enter ``help'' for commands"));
		return (0);
	}

	while (isspace(*p) != 0 )
		p++;

	return (p);
}

