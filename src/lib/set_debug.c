/*
 * UFM Radio Control utility
 * Copyright (c) 2002-2013 Nicola Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 */

/*
 * NAME
 *	set_debug
 *
 * DESCRIPTION
 *	The function enables/disables the diagnostic messages
 *	of the library.
 *
 * ARGUMENTS
 *	One of the constants DEBUG_ON, DEBUG_OFF.
 *
 * RETURN VALUES
 *	None.
 *
 * $Id: set_debug.c,v 1.9 2009/04/09 08:26:37 nivit Exp $
 */

#include "libufm.h"

int radio_debug = DEBUG_OFF; /* default value */

void
set_debug(const int status)
{

	radio_debug = status;
}

