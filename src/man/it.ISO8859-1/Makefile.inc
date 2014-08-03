#
# $Id: Makefile.inc,v 1.1 2003/10/31 16:17:31 nivit Exp $
#

.if defined(PREFIX)
SHAREDIR=	${PREFIX}
SHAREOWN!=	id -u -n
SHAREGRP!=	id -g -n
.endif
