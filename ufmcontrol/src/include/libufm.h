/*-
 * Copyright (c) 2002-2013 Nicola S. Vitale <nivit@users.sourceforge.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Function prototypes and defines
 *
 * $Id: libufm.h,v 1.9 2009/04/09 08:26:36 nivit Exp $
 */

#ifndef LIBUFM_H_SEEN
#define LIBUFM_H_SEEN

#include <sys/param.h>
#if __FreeBSD_version < 800000
#include <dev/usb/dsbr100io.h>
#else
#include <dev/usb/ufm_ioctl.h>
#endif
#include <err.h>
#include <stdio.h>

int	 open_radio(const char *_device_path);

int	 close_radio(const int _radio_handle);

int	 get_freq(const int _radio_handle);
int	 set_freq(const int _radio_handle, const int _freq, const int _japan);
#define kHz    1000

int	 check_freq(const int _freq, const int _japan);
#define FREQ_LIM_L           88000000
#define FREQ_LIM_H          108000000
#define JAPAN_FREQ_LIM_L     76000000
#define JAPAN_FREQ_LIM_H     91000000

int	 get_stereo(const int _radio_handle);

int	 turn_power(int _radio_handle, unsigned long _power);
#define POWER_ON     FM_START
#define POWER_OFF    FM_STOP

/*
 * Debug stuff
 */

extern int radio_debug;

void	 set_debug(const int _status);
#define DEBUG_ON 1
#define DEBUG_OFF 0

#define DPRINT(x) if (radio_debug == DEBUG_ON) (warnx("libufm: %s", x))

#endif /* !LIBUFM_H_SEEN */
