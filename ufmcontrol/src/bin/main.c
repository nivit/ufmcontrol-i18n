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
 * $Id: main.c,v 1.9 2009/04/09 08:26:34 nivit Exp $
 */

#if 0
#include <sys/syslimits.h>    /* PATH_MAX >= FILENAME_MAX */
#endif

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "extern.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID MAIN_C_SET

int	fd = -1;		   /* file descriptor for the device       */
int 	japan = 0;		   /* default band: 88.00 MHz ~ 108.00 MHz */
int	tab_start = __LINE__ + 10; /* lines at the first comment in cmdtab */
int	verbose = 1;

char	ufm_name[FILENAME_MAX + 1] = "";
char	ufm_mixer[FILENAME_MAX + 1] = "";

/*
 * ATTENTION: DO NOT REMOVE OR MODIFY COMMENTS HERE!
 */
struct cmdtab cmdtab0[] = {
	{ CMD_HELP,	"?",	             /* NLS("?")                  */
	 	1,	""	             /* NLS("")                   */ },
	{ CMD_INC_FREQ,	"+",	             /* NLS("+")                  */
		1,	""	             /* NLS("")                   */ },
	{ CMD_DEC_FREQ,	"-",	             /* NLS("-")                  */
		1,	""	             /* NLS("")                   */ },
	{ CMD_DEVICE,	"device",	     /* NLS("device")             */
		1,	"[filename]"	     /* NLS("[filename]")         */ },
	{ CMD_GET_FREQ,	"get",	             /* NLS("get")                */
		1,	""	             /* NLS("")                   */ },
	{ CMD_HELP,	"help",	             /* NLS("help")               */
		1,	""	             /* NLS("")                   */ },
	{ CMD_JAPAN,	"japan",	     /* NLS("japan")              */
		1,	"[yes | no]"	     /* NLS("[yes | no]")         */ },
	{ CMD_LOCALE,	"locale",	     /* NLS("locale")             */
		1,	"[locale]"	     /* NLS("[locale]")           */ },
	{ CMD_MIXER,	"mixer",	     /* NLS("mixer")              */
		1,	"[device [port]]"    /* NLS("[device [port]]")    */ },
	{ CMD_PLAY,	"play",	             /* NLS("play")               */
		1,	""	             /* NLS("")                   */ },
	{ CMD_PORT,	"port",	             /* NLS("port")               */
		2,	"[mixer_device]"     /* NLS("[mixer_device]")     */ },
	{ CMD_QUIT,	"quit",	             /* NLS("quit")               */
		1,	""	             /* NLS("")                   */ },
	{ CMD_SET_FREQ,	"set",	             /* NLS("set")                */
		1,	"frequency"	     /* NLS("frequency")          */ },
	{ CMD_STEREO,	"stereo",	     /* NLS("stereo")             */
		2,	""	             /* NLS("")                   */ },
	{ CMD_STOP,	"stop",	             /* NLS("stop")               */
		3,	""	             /* NLS("")                   */ },
	{ CMD_VERBOSE,	"verbose",	     /* NLS("verbose")            */
		1,	"[yes | no]"	     /* NLS("[yes | no]")	  */ },
	{ CMD_VOLUME,	"volume",	     /* NLS("volume")             */
		2,	"[<l> [<r>] | mute]" /* NLS("[<l> [<r>] | mute]") */ },
	{ CMD_NULL,		"",		0,	"" }
};

struct cmdtab	*cmdtab = cmdtab0;

int
main(int argc, char *argv[])
{
	int cmd;
	int nls = 1;
	char *arg;

	for (;;) {
		/* If you add options here, remember to change usage() too,
		   and to update manpage */
		switch (getopt (argc, argv, "hjLsvVf:l:m:")) {
		case EOF:
			break;
		case 'j': /* FM ... 76.00 MHz ~ 96 MHz */
			japan = 1;
			continue;
		case 'L':
			nls = 0;	/* disable native language support */
			continue;
		case 's':
			verbose = 0;
			continue;
		case 'v':
			verbose = 2;
			continue;
		case 'V':
			version();
			exit(EX_OK);
		case 'f':
			(void)set_devname(ufm_name, optarg);
			continue;
		case 'l':
			(void)set_locale(optarg);
			continue;
		case 'm':
			(void)set_devname(ufm_mixer, optarg);
			continue;
		case 'h':
		default:
			usage();
		}
			break;
	}

	argc -= optind;
	argv += optind;

	if (nls == 1 && ufm_locale[0] == '\0')
		(void)set_locale(setlocale(LC_ALL, ufm_locale));

	if (argc > 0 && strcasecmp(*argv, "help") == 0) {
		(void)printf("%s %s %s\n",
		    NLS("USB Radio Control utility,"),
		    NLS("version"), VERSION);
		help();
		usage();
	}

	if (ufm_name[0] == '\0')
		(void)set_devname(ufm_name, getenv("UFM_RADIO"));

  	if (ufm_name[0] == '\0') {
		(void)set_devname(ufm_name, DEFAULT_UFM_DEVICE);

		if (verbose > 1)
			warnx("%s %s", NLS("no UFM device specified, using"),
			    ufm_name);
	}

	if (ufm_mixer[0] == '\0')
		(void)set_devname(ufm_mixer, getenv("MIXER"));

  	if (ufm_mixer[0] == '\0') {
		(void)set_devname(ufm_mixer, DEFAULT_MIXER_DEVICE);

		if (verbose > 1)
			warnx("%s %s", NLS("no mixer device specified, using"),
			    ufm_mixer);
	}

	if (argc > 0) {
		char *p, buf[80];
		int len;

		for (p = buf; argc-- > 0; ++argv) {
			len = strlen(*argv);

			if (p + len >= buf + sizeof(buf) - 1)
				usage();

			if (p > buf)
				*p++ = ' ';

			strcpy(p, *argv);
			p += len;
		}

		*p = '\0';
		arg = parse(buf, &cmd);
		return(run(cmd, arg));
	}

	if (verbose == 1)
		verbose = isatty(STDIN_FILENO);

	if (verbose) {
		(void)printf("%s %s %s\n%s\n\n",
		    NLS("USB Radio Control utility,"),
		    NLS("version"), VERSION,
		    NLS("Type `?' for command list"));
	}

	for (;;) {
		arg = input (&cmd);
		if (run(cmd, arg) < 0) {
			if (verbose)
				warn(NULL);
			close(fd);
			fd = -1;
		}
		fflush(stdout);
	}
}

