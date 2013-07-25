#
# $Id: Makefile.inc,v 1.1 2003/10/31 16:17:33 nivit Exp $
#

.if defined(PREFIX)
SHAREDIR=	${PREFIX}/share
SHAREOWN!=	id -u -n
SHAREGRP!=	id -g -n
.endif


# File <bsd.nls.mk> does not exist in FreeBSD-4.*
.if !exists(/usr/share/mk/bsd.nls.mk)
.include "../bsd.nls.mk"
.endif
