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
 *	delta_freq
 *
 * DESCRIPTION
 *	This function increments/decrements the current
 *	frequency by FREQ_INC (see "extern.h").
 *
 * ARGUMENTS
 *	CMD_INC_FREQ or CMD_DEC_FREQ (see "extern.h").
 *
 * RETURN VALUES
 *	 0 if successful.
 *	-1 on failure.
 *
 * $Id: delta_freq.c,v 1.10 2009/04/09 08:26:34 nivit Exp $
 */

#include <err.h>
#include <stdio.h>

#include "extern.h"
#include "libufm.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID DELTA_FREQ_C_SET

int
delta_freq(const int cmd)
{
	int freq, ret, inc;

	/* fd > 0, checked in run.c  */
	freq = get_freq(fd);

	if (freq < 0)
		return (-1);

	switch (cmd) {
	case CMD_INC_FREQ:
		inc = +FREQ_INC;
		break;
	case CMD_DEC_FREQ:
		inc = -FREQ_INC;
		break;
	default:
		inc = 0;
		break;
	}

	ret = check_freq(freq * 10 * kHz, japan);

	if (ret < 0) {
		if (cmd == CMD_INC_FREQ)
			freq = (japan? JAPAN_FREQ_LIM_L : FREQ_LIM_L) /
				(10 * kHz);
		else
			freq = (japan? JAPAN_FREQ_LIM_H : FREQ_LIM_H) /
				(10 * kHz);

		inc = 0;
	}

	freq += inc;

	ret = set_freq(fd, freq, japan);

	if (ret == -1 && verbose)
		warnx("%s", NLS("Invalid frequency"));

	if (ret != -1) {
		VERBOSE(1, ("FM "));
		VERBOSE(0, ("%5.2f ", freq / 100.0));
		VERBOSE(1, ("MHz"));
		VERBOSE(0, ("\n"));
	}

	return (ret);
}
