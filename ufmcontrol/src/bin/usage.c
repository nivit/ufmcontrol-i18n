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
 *	usage
 *
 * DESCRIPTION
 *	This function prints the options available on the
 *	command line.
 *
 * ARGUMENTS
 *	None.
 *
 * RETURN VALUES
 *	None.
 *
 * $Id: usage.c,v 1.8 2009/04/09 08:26:35 nivit Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID USAGE_C_SET

void
usage(void)
{

	(void)fprintf(stderr, "%s %s %s %s\n",
	    NLS("usage: ufmcontrol"),
	    NLS("[-hjLsvV] [-f device]"),
	    NLS("[-l locale] [-m mixer]"),
	    NLS("[command ...]"));

	exit(EX_USAGE);
}

