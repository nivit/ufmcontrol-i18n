/*
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	get_freq
 *
 * DESCRIPTION
 *	This function returns the current frequency.
 *
 * ARGUMENTS
 *	The file descriptor of the radio device.
 *
 * RETURN VALUES
 *	 A positive integer if successful.
 *	-1 on failure.
 *
 * $Id: get_freq.c,v 1.9 2009/04/09 08:26:37 nivit Exp $
 */

#include <sys/ioctl.h>

#include "libufm.h"

int
get_freq (const int radio_handle)
{
	int error, freq;

	error = ioctl(radio_handle, FM_GET_FREQ, &freq);

	if (error != -1)
		return freq / (10 * kHz);
	else {
                DPRINT("impossible to get the frequency");
		return error;
	}
}
