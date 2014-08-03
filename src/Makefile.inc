#
# $Id: Makefile.inc,v 1.2 2003/10/31 16:17:28 nivit Exp $
#

.if !defined(PREFIX)
PREFIX=		/usr/local
.endif

LIBDIR=		${PREFIX}/lib
BINDIR=		${PREFIX}/bin
MANDIR=		${PREFIX}/man/man
INCLUDEDIR=	${PREFIX}/include

BINOWN!=	id -u -n
BINGRP!=	id -g -n
SHAREOWN=	${BINOWN}
SHAREGRP=	${BINGRP}
