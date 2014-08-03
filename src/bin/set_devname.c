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
 *	set_devname
 *
 * DESCRIPTION
 *	This function sets the device name to open.
 *
 * ARGUMENTS
 *	The variable to set and a path 
 *	(a buffer of FILENAME_MAX + 1 bytes).
 *
 * RETURN VALUES
 *	 0 on successful
 *	-1 on failure
 *
 * $Id: set_devname.c,v 1.9 2009/04/09 08:26:34 nivit Exp $
 */

#if 0
#include <sys/syslimits.h>    /* PATH_MAX >= FILENAME_MAX */
#endif

#include <err.h>
#include <stdio.h>
#include <string.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID SET_DEVNAME_C_SET

int
set_devname(char * dev, const char *path)
{
	size_t len;

	if (path != NULL)
		len = strlen(path);
	else
		return (-1);

	if (dev == NULL)
		return (-1);

	if (len <= FILENAME_MAX) {
		(void)strncpy(dev, path, len);
		dev[len] = '\0';
		return (0);
	} else {
		warnx("%s %d %s",
		    NLS("The device name exceeds"), FILENAME_MAX,
		    NLS("bytes"));
		return (-1);
	}
}
