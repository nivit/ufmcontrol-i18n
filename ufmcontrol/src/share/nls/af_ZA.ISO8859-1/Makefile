#
# $Id: Makefile,v 1.1 2003/10/31 16:17:33 nivit Exp $
#

NLS:=		${.CURDIR:T}
NLSNAME=	ufmcontrol

# legerdemain for FreeBSD-4.*
.for file in ${NLS}
NLSLINKS_${file}=	${NLS:R}5
.endfor

.include <bsd.prog.mk>

