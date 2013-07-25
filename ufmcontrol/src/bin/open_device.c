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
 *	open_device
 *
 * DESCRIPTION
 *	This function opens a radio device
 *
 * ARGUMENTS
 *	None.
 *
 * RETURN VALUES
 *	 0 if successful
 *	-1 on failure
 *
 * $Id: open_device.c,v 1.9 2009/04/09 08:26:34 nivit Exp $
 */

#include <err.h>

#include "extern.h"
#include "libufm.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID OPEN_DEVICE_C_SET

int
open_device(void)
{

	if (fd < 0) fd = open_radio(ufm_name);

        if (fd < 0) {
                warnx("%s \"%s\"", NLS("impossible to open device"), ufm_name);
                warn(NULL);
		return (-1);
        }

	return (0);
}

