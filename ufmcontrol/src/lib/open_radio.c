/*
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	open_radio
 *
 * DESCRIPTION
 *	The function opens the device, specified as argument.
 *
 * ARGUMENTS
 *	A string that specifies the path of the radio device.
 *
 * RETURN VALUES
 *	A file descriptor (a non-negative integer) to use
 *	in all next calls to other library functions.
 *	-1 on failure.
 *
 * $Id: open_radio.c,v 1.8 2009/04/09 08:26:37 nivit Exp $
 */

#if 0
#include <sys/syslimits.h>    /* PATH_MAX => FILENAME_MAX */
#endif

#include <fcntl.h>
#include <paths.h>
#include <stdio.h>
#include <string.h>

#include "libufm.h"

int
open_radio(const char *radio_device)
{
	int fd, len;
	char devpath[FILENAME_MAX + 1];

	fd = -1;
	len = 0;

	if (radio_device != NULL)
		len = strlen(radio_device);
	else
		return -1;

	if (len > 0 && len <= FILENAME_MAX) {
		if (*radio_device != '/')
			sprintf(devpath, "%s%s", _PATH_DEV, radio_device);
		else
			sprintf(devpath, "%s", radio_device);

		fd = open(devpath, O_RDWR);

		if (fd < 0)
			DPRINT("impossible to open device");
	}

	return fd;
}
