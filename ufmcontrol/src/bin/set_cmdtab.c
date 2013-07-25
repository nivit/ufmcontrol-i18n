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
 *	set_cmdtab
 *
 * DESCRIPTION
 *	This function sets the cmdtab1 array to localized values.
 *
 * ARGUMENTS
 *	None.
 *
 * RETURN VALUES
 *	 0 on successful
 *	-1 on failure
 *
 * $Id: set_cmdtab.c,v 1.7 2009/04/09 08:26:34 nivit Exp $
 */

#include <stdlib.h>
#include <string.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID	MAIN_C_SET	/* cmdtab0 is defined in main.c */

struct cmdtab	cmdtab1[CMDTAB_ELEMENTS + 1];

static void	sort_cmdtab(void);
static int	cmdtab_cmp(const void *_tab1, const void *_tab2);

int
set_cmdtab(void)
{
	int		args = 1;
	int		command = 0;
	int		ret;
	unsigned int	min;
	struct cmdtab	*ndx = cmdtab1;
	struct cmdtab	*tabptr = cmdtab1;

	cmdtab = cmdtab0;	/* point to default values */

	cmdtab1[CMDTAB_ELEMENTS].command = CMD_NULL;

	/* fill array with new values */
  	while (cmdtab->command != CMD_NULL) {
		tabptr->command = cmdtab->command;
 		tabptr->name = NLS2(tab_start + command, cmdtab->name);
 		tabptr->args = NLS2(tab_start + args, cmdtab->args);
		command += CMDTAB_CMD_OFFSET;
		args = command + CMDTAB_ARGS_OFFSET;
		tabptr++; cmdtab++;
	}

	sort_cmdtab();

	tabptr = cmdtab1;
	/* calculate minimum number of chars to identify a command */
  	while (tabptr->command != CMD_NULL) {
  		min = 1; ndx = cmdtab1;

		while (ndx != tabptr) {
			ret = strncmp(ndx->name, tabptr->name, min);
			if (ret == 0) {
				min++;
			} else {
 				tabptr->min = min;
				ndx++;
			}
		}

		tabptr++;
	}

 	cmdtab = cmdtab1;	/* point to new localized values */

	return 0;
}

void
sort_cmdtab(void)
{
	size_t		count = CMDTAB_ELEMENTS;
	size_t		size = sizeof(struct cmdtab);
	struct cmdtab	*tabptr = cmdtab1;

	qsort(tabptr, count, size, cmdtab_cmp);
}

int
cmdtab_cmp(const void *tab1, const void *tab2)
{

	return strcoll(((const struct cmdtab*)tab1)->name,
	    ((const struct cmdtab*)tab2)->name);
}
