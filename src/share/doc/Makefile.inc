#
# $Id: Makefile.inc,v 1.1 2003/10/31 16:17:32 nivit Exp $
#

.if !defined(PREFIX) && !defined(PORTNAME)
PREFIX=		/usr/local
.endif

LIBDIR=		${PREFIX}/lib
BINDIR=		${PREFIX}/bin
MANDIR=		${PREFIX}/man/man
INCLUDEDIR=	${PREFIX}/include
SHAREDIR=	${PREFIX}/share

BINOWN!=	id -u -n
BINGRP!=	id -g -n
SHAREOWN=	${BINOWN}
SHAREGRP=	${BINGRP}
