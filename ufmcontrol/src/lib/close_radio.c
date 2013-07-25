/*-
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	close_radio
 *
 * DESCRIPTION
 *	The function closes the radio device, whose file
 *	descriptor is specified as argument.
 *
 * ARGUMENTS
 *	The file descriptor of the radio device.
 *
 * RETURN VALUES
 *	 0 if successful
 *	-1 on failure
 *
 * $Id: close_radio.c,v 1.9 2009/04/09 08:26:37 nivit Exp $
 */

#include <unistd.h>

#include "libufm.h"

int
close_radio(const int radio_handle)
{
	int ret;

	ret = close(radio_handle);

	if (ret != 0)
		DPRINT("impossible to close the device");

	return ret;
}
