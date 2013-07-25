/*
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	set_freq
 *
 * DESCRIPTION
 *	This function sets the frequency of the radio.
 *
 * ARGUMENTS
 *	An integer, which specifies the frequency to set,
 *	multiplied for 100.
 *	E.g., for 107.05 (MHz) pass 10705.
 *
 * RETURN VALUES
 *	 0 if successful
 *	-1 on failure
 *
 * $Id: set_freq.c,v 1.9 2009/04/09 08:26:37 nivit Exp $
 */

#include <sys/ioctl.h>

#include "libufm.h"

int
set_freq (const int radio_handle, const int freq, const int japan)
{
	int error, f, invalid;

	f = freq * 10 * kHz;

	invalid = check_freq(f, japan);

	if (!invalid) {
		error = ioctl(radio_handle, FM_SET_FREQ, &f);
		if (error != -1)
			return 0;
		else
			DPRINT("impossible to set frequency");
	}

	return -1;
}
