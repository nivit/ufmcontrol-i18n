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
 *	ufmcontrol_prompt
 *
 * DESCRIPTION
 *	This function simply prints the prompt.
 *
 * ARGUMENTS
 *	None.
 *
 * RETURN VALUES
 *	A char pointer.
 *
 * $Id: prompt.c,v 1.8 2009/04/09 08:26:34 nivit Exp $
 */

#include "extern.h"

const char *
ufmcontrol_prompt(void)
{

	return ("ufmcontrol> ");
}
