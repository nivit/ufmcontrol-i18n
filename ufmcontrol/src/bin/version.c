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
 *	version
 *
 * DESCRIPTION
 *	This function prints the version of the program.
 *
 * ARGUMENTS
 *	None.
 *
 * RETURN VALUES
 *	None.
 *
 * $Id: version.c,v 1.7 2009/04/09 08:26:35 nivit Exp $
 */

#include <stdio.h>

#include "extern.h"
#include "version.h"

void
version(void)
{

	VERBOSE(1, ("UFMControl "));
	(void)printf("%s\n", VERSION);
}

