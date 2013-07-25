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
 * $Id: run.c,v 1.10 2009/04/09 08:26:34 nivit Exp $
 */

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "extern.h"
#include "libufm.h"

#ifdef CAT_SETID
#undef CAT_SETID
#endif
#define CAT_SETID RUN_C_SET

char	ufm_inport[SOUND_DEVICE_NAMES_LEN + 1] = "line";

int
run(const int cmd, const char *arg)
{
	int		ret, freq, args;
	unsigned int	left, right;
	char		dev[FILENAME_MAX + 1];
	char		port[SOUND_DEVICE_NAMES_LEN + 1];

	switch (cmd) {

	case CMD_DEVICE:
		if (*arg != '\0') { /* set a new device; close the old one */
			(void)set_devname(ufm_name, arg);

			if (fd > 0) {
				close(fd);
				fd = -1;
			}
		}

		VERBOSE(1, (NLS("Radio device: ")));
		VERBOSE(0, ("%s\n", ufm_name));

		break;

	case CMD_GET_FREQ:
		if (fd < 0 && (open_device() == -1))
		    return (0);

		freq = get_freq(fd);

		if (freq != -1) {
			VERBOSE(1, ("FM "));
			VERBOSE(0, ("%5.2f ", freq / 100.0));
			VERBOSE(1, ("MHz"));
			VERBOSE(0, ("\n"));
		} else
			return (-1);

		break;

	case CMD_DEC_FREQ:
	case CMD_INC_FREQ:
		if (fd < 0 && (open_device() == -1))
		    return (0);

		return (delta_freq(cmd));

	case CMD_JAPAN:
		if (*arg == '\0' && verbose)
			(void)printf("%s\n", japan? NLS("Yes") : /* a capo */
			    NLS("No"));

		if (strcasecmp(arg, NLS("no")) == 0)
			japan = 0;

		if (strcasecmp(arg, NLS("yes")) == 0)
			japan = 1;

		break;

	case CMD_LOCALE:
		if (*arg == '\0' && verbose) {
			VERBOSE(1, (NLS("locale: ")));
			VERBOSE(0, ("%s\n", ufm_locale));
		} else
			(void)set_locale(arg);

		break;

	case CMD_MIXER:
		if (*arg != '\0') {	/* set a new mixer device */
			/* 1024 == FILENAME_MAX, 7 = SOUND_DEVICE_NAMES_LEN */
			args =  sscanf(arg, "%1024s %7s", dev, port);

			switch (args) {
			case 2:
				if(isalpha(port[0]))
					STRNCPY(ufm_inport, port);
				/* else ignore */
			/* no break! */
			case 1:
				(void)set_devname(ufm_mixer, dev);
				break;
			default:
				return (-1);
				break;
			}
		}

		if (verbose) {
			VERBOSE(1, (NLS("Mixer: ")));
			VERBOSE(0, ("%s\n", ufm_mixer));
			VERBOSE(1, (NLS("device: ")));
			VERBOSE(0, ("%s\n", ufm_inport));
		}

		break;

	case CMD_PLAY:
		if (fd < 0 && (open_device() == -1))
		    return (0);

		ret = turn_power(fd, POWER_ON);

		#if 0
		/* The driver checks if the device is in stereo mode,
		   so, after a CMD_STOP, it returns always -1 */
		if (ret != -1) {
		#endif
			VERBOSE(1, ("UFM Radio - "));
			VERBOSE(0, ("PLAY\n"));
		#if 0
		}

		return (ret);
		#endif

		break;

	case CMD_PORT:
		if (*arg != '\0') {
			/* set a new mixer device */
			/* 7 == SOUND_DEVICE_NAMES_LEN */
			ret = sscanf (arg, "%7s", port);

			/* ufm_inport starts with a letter */
			if (ret == 0 || !isalpha(port[0])) {
				return (-1);
			} else
				STRNCPY(ufm_inport, port);
		}

		if (verbose) {
			VERBOSE(1, (NLS("Mixer device: ")));
			VERBOSE(0, ("%s\n", ufm_inport));
		}

		break;

	case CMD_QUIT:
		if (nl_cd != (nl_catd) -1)
			catclose(nl_cd);

		exit(EXIT_SUCCESS);

	case CMD_SET_FREQ:
		if (fd < 0 && (open_device() == -1))
		    return (0);

		if (*arg != '\0') {
			freq = atof(arg) * 100;

			ret = set_freq(fd, freq, japan);

			if (ret == -1 && verbose)
  				warnx("%s", NLS("Invalid frequency"));

			return (ret);
		}

		if (verbose) {
			warnx("%s",
			    NLS("\"set\" command requires an argument"));
		}

		break;

	case CMD_STEREO:
		if (fd < 0 && (open_device() == -1))
		    return (0);

		ret = get_stereo(fd);

		if (ret != -1 && verbose)
			(void)printf("%s%s\n",
			    ret? "" : NLS("Not "), /* a capo */
			    NLS("stereo"));

		return (ret);

	case CMD_STOP:
		if (fd < 0 && (open_device() == -1))
		    return (0);

		ret = turn_power(fd, POWER_OFF);

		if (ret != -1) {
			VERBOSE(1, ("UFM Radio - "));
			VERBOSE(0, ("STOP\n"));
		}

		return (ret);

	case CMD_VERBOSE:
		if (*arg == '\0' && verbose)
			(void)printf("%s\n",
			    (verbose > 1)? NLS("Yes") : /* a  capo! */
			    NLS("No"));

		if (strcasecmp(arg, NLS("no")) == 0)
			verbose = 1;

		if (strcasecmp(arg, NLS("yes")) == 0)
			verbose = 2;

		break;

	case CMD_VOLUME:
		if (*arg == '\0')
			return (set_volume("", 0, 0));

		if (!strncasecmp(arg, NLS("mute"), strlen(arg)))
                        return (set_volume(ufm_mixer, 0, 0));

		if (1 > (args = sscanf (arg, "%u %u", &left, &right))) {
                        warnx(NLS("invalid command arguments"));
                        return (-1);
		}

		if (args == 1)
			right = left;

		return (set_volume(ufm_mixer, left, right));

	case CMD_HELP:

	default:
		help();
		break;
	}

	return (0);
}
