/*
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	turn_power
 *
 * DESCRIPTION
 *	The function turns on/off the radio
 *
 * ARGUMENTS
 *	One of the integers FM_START/FM_STOP
 *
 * RETURN VALUES
 *	 0 if successful
 *	-1 on failure
 *
 * $Id: turn_power.c,v 1.9 2009/04/09 08:26:37 nivit Exp $
 */

#include <sys/ioctl.h>

#include "libufm.h"

int
turn_power(int radio_handle, unsigned long status)
{
	int error, ret;

	error = ioctl(radio_handle, status, &ret);

	return (error != -1)? 0 : -1;
}
