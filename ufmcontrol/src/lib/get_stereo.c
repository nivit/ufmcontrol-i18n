/*
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	get_stereo
 *
 * DESCRIPTION
 *	The function checks if the radio is in stereo mode.
 *
 * ARGUMENTS
 *	A file descriptor as returned from the open_radio
 *	function.
 *
 * RETURN VALUES
 *      1 if in stereo mode.
 *      0 if not in stereo mode.
 *     -1 on failure
 *
 * NOTES
 *	This function could be used to tune in the broadcasters.
 *
 * $Id: get_stereo.c,v 1.10 2009/04/09 08:26:37 nivit Exp $
 */

#include <sys/ioctl.h>

#include "libufm.h"

int
get_stereo(const int radio_handle)
{
	int error, status;

	error = ioctl(radio_handle, FM_GET_STAT, &status);

	if (error != -1)
		/* if bit0 = 1 then not stereo, else stereo */
		return ((status & 0x01)? 0 : 1);
	else
		return -1;
}
