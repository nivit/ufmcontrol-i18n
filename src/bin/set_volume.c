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
 *	set_volume
 *
 * DESCRIPTION
 *	This function sets the volume of the mixer.
 *
 * ARGUMENTS
 *	The name of the mixer, the volume of the left channel,
 *	and the volume of the right one.
 *	If mixer is "", then prints the current values.
 *
 * RETURN VALUES
 *	 0 on successful
 *	-1 on failure
 *
 * $Id: set_volume.c,v 1.7 2009/04/09 08:26:35 nivit Exp $
 */

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID	SET_VOLUME_C_SET

#ifdef SPACE
#undef SPACE
#endif
#define SPACE " "

#ifndef ARG_MAX
#define ARG_MAX		4096
#endif

#define MXR_CMD		"/usr/sbin/mixer"
#define OPT1		SPACE "-s"
#define OPT2		SPACE "-f" SPACE

int
set_volume(const char* mixer, unsigned int left, unsigned int right)
{
	size_t		len;
	unsigned int	l, r;
	FILE		*po;
	char		command[FILENAME_MAX + ARG_MAX + 1];
	char		line[LINE_MAX + 1];
	char		new_values[ARG_MAX + 1];

	l = left % (100 + 1);
	r = right % (100 + 1);

	STRNCPY(command, MXR_CMD);

	if (verbose < 2)
		STRNCAT(command, OPT1);

	STRNCAT(command, OPT2);
	STRNCAT(command, ufm_mixer);
	STRNCAT(command, SPACE);
	STRNCAT(command, ufm_inport);

	len = strlen(mixer);

	if (len != 0) {
		(void)snprintf(new_values, sizeof(new_values),
		    " %u:%u", l, r);
		STRNCAT(command, new_values);
	}

	po = popen(command, "r");

	if (po == NULL) {
		warnx("%s: %s",
		    NLS("impossible to execute command"), command);
		return (-1);
	}
	else {
		if (fgets(line, LINE_MAX, po) == NULL)
			;	/* error! */
		else if (verbose)
			(void)printf("%s", line);

		/* mixer does not print a newline... */
		if (verbose < 2 && (mixer[0] == '\0'))
			(void)printf("\n");

		pclose(po);
	}

	return (0);
}
