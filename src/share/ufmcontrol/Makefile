#
# $Id: Makefile,v 1.1 2003/10/31 16:17:32 nivit Exp $
#

FILES=		xufmrc
FILESDIR=	${PREFIX}/share/ufmcontrol-i18n/

.if !defined(PREFIX) && !defined(PORTNAME)
PREFIX=		/usr/local
.endif

SHAREDIR=	${PREFIX}/share
SHAREGRP!=	id -g -n
SHAREOWN!=	id -u -n

.include <bsd.prog.mk>
