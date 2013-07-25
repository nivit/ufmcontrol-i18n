/*-
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	check_freq
 *
 * DESCRIPTION
 *	The function controls that the value of the frequency is
 *	in a valid band.
 *
 * ARGUMENTS
 *	The first argument is an integer, which is the frequency
 *	to check.
 *	The second one a not null integer if the requested band
 *	is 76 MHz ~ 91.00 MHz (Japan), 0 otherwise (88.00 MHz ~
 *	108.00 MHz).
 *
 * RETURN VALUES
 *	 0 if the frequency is valid.
 *	-1 otherwise.
 *
 * $Id: check_freq.c,v 1.9 2009/04/09 08:26:37 nivit Exp $
 */

#include <stdio.h>

#include "libufm.h"

int
check_freq(const int freq, const int japan)
{

	if (japan == 0 && freq >= FREQ_LIM_L && freq <= FREQ_LIM_H)
		return 0;

	if (japan > 0 && freq >= JAPAN_FREQ_LIM_L && freq <= JAPAN_FREQ_LIM_H)
		return 0;

	DPRINT("invalid frequency");
	return -1;
}
