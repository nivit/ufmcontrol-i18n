#
# $Id: Makefile,v 1.5 2007/01/15 10:54:11 nivit Exp $
#

SRCS=		delta_freq.c	\
		input.c		\
		help.c		\
		main.c		\
		open_device.c	\
		parse.c		\
		prompt.c	\
		run.c		\
		set_cmdtab.c	\
		set_devname.c	\
		set_locale.c	\
		set_volume.c	\
		usage.c		\
		version.c

PROG=		ufmcontrol
SCRIPTS=	tkradio.tcl

CFLAGS+=	-I${.CURDIR}/../include ${BDECFLAGS} -Werror

DPADD=		${LIBEDIT} ${LIBTERMCAP}

LDADD=		-ledit -ltermcap -lufm -L${.CURDIR}/../lib/obj

NO_MAN=		YES

.include <bsd.prog.mk>
