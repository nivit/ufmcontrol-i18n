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
 * $Id: input.c,v 1.8 2009/04/09 08:26:34 nivit Exp $
 */
#include <sys/param.h>
#include <string.h>

#include <histedit.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID INPUT_C_SET

char *
input(int *cmd)
{
#define MAXLINE 80
	static EditLine	*el = NULL;
	#if __FreeBSD_version >= 500000
	HistEvent he;
	#endif
	static History	*hist = NULL;
	const char	*bp = NULL;
	static char	buf[MAXLINE];
	char		*p;
	int		num = 0;
	int		len;

	do {
		if (verbose) {
			if (!el) {
				el = el_init("ufmcontrol", stdin, stdout
				#if __FreeBSD_version >= 500000
					 , stderr
				#endif
				);
				hist = history_init();
				history(hist,
				#if __FreeBSD_version >= 500000
					&he,
				#endif
					H_EVENT, 100);
				el_set(el, EL_HIST, history, hist);
				el_set(el, EL_EDITOR, "emacs");
				el_set(el, EL_PROMPT, ufmcontrol_prompt);
				el_set(el, EL_SIGNAL, 1);
				el_source(el, NULL);
			}
			if ((bp = el_gets(el, &num)) == NULL || num == 0) {
				*cmd = CMD_QUIT;
				(void)fprintf(stderr, "\r\n");
				return (0);
			}

			len = (num > MAXLINE) ? MAXLINE : num;
			memcpy(buf, bp, len);
			buf[len] = 0;
			history(hist,
				#if __FreeBSD_version >= 500000
					&he,
				#endif
				H_ENTER, bp);
#undef MAXLINE

		} else {
			if (! fgets (buf, sizeof (buf), stdin)) {
				*cmd = CMD_QUIT;
				(void)fprintf(stderr, "\r\n");
				return (0);
			}
		}
		p = parse (buf, cmd);
	} while (! p);

	return (p);
}

