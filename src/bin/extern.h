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
 * $Id: extern.h,v 1.10 2009/04/09 08:26:34 nivit Exp $
 */

#ifndef EXTERN_H_SEEN
#define EXTERN_H_SEEN

#include "nls.h"
#include "version.h"

#ifndef DEFAULT_UFM_DEVICE
#define DEFAULT_UFM_DEVICE "/dev/ufm0"
#endif

#ifndef DEFAULT_MIXER_DEVICE
#define DEFAULT_MIXER_DEVICE "/dev/mixer0"
#endif

#define CMD_NULL	 0
#define CMD_DEVICE	 1
#define CMD_GET_FREQ	 2
#define CMD_HELP	 3
#define CMD_DEC_FREQ	 4
#define CMD_INC_FREQ	 5
#define CMD_JAPAN	 6
#define CMD_LOCALE	 7
#define CMD_MIXER	 8
#define CMD_PLAY	 9
#define CMD_PORT	10
#define CMD_QUIT	11
#define CMD_SET_FREQ	12
#define CMD_STEREO	13
#define CMD_STOP	14
#define CMD_VERBOSE	15
#define CMD_VOLUME	16

#define CMDTAB_ELEMENTS		16 + 1	/* command 'help' is double */
#define CMDTAB_CMD_OFFSET	2	/* see cmdtab[] in main.c */
#define CMDTAB_ARGS_OFFSET	1

#define FREQ_INC	 1		/*  x 10  kHz */

#define SOUND_DEVICE_NAMES_LEN	7	/* see <sys/soundcards.h> */

extern char	ufm_locale[];
extern char	ufm_name[];
extern char	ufm_mixer[];
extern char	ufm_inport[];		/* microphone or line */

extern int	fd;
extern int	japan;
extern int	verbose;
extern int	tab_start;

extern nl_catd  nl_cd;

extern struct cmdtab {
	int command;
	const char *name;
	unsigned min;
	const char *args;
} cmdtab0[], cmdtab1[];

extern struct cmdtab	*cmdtab;

void		help(void);
void		usage(void);
void		version(void);
char		*input(int*);
char		*parse(char*, int*);
const char	*ufmcontrol_prompt(void);
int		delta_freq(const int);	/* inc/decrement frequency */
int		open_device(void);
int		run(const int, const char*);
int		set_cmdtab(void);
int		set_devname(char*, const char*);
int		set_locale(const char*);
int		set_volume(const char*, unsigned int, unsigned int);

#define STRNCAT(S, T)   \
        (strncat(S, T, sizeof(S) - strlen(S) - 1))

#define STRNCPY(D, S) do {			\
        (strncpy(D, S, sizeof(D) - 1));		\
        (D[sizeof(D) - 1] = '\0');		\
} while (0)

#define VERBOSE(l, s) if (verbose > (l)) (void) printf s

#endif /* !EXTERN_H_SEEN */

