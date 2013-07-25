#
# $Id: Makefile,v 1.5 2007/01/15 10:55:49 nivit Exp $
#

SRCS=		check_freq.c	\
		close_radio.c	\
		get_freq.c	\
		get_stereo.c	\
		open_radio.c	\
		set_debug.c	\
		set_freq.c	\
		turn_power.c

LIB=		ufm
NO_PROFILE=	yes
SHLIB_MAJOR=	1

CFLAGS+=	-I${.CURDIR}/../include ${BDECFLAGS} -Werror



.include <bsd.lib.mk>
