#
# $Id: Makefile,v 1.2 2003/10/31 16:17:28 nivit Exp $
#

SUBDIR=		etc	\
		lib	\
		bin	\
		include \
		man

# the directory tools must precede share
.if defined(MAKECATALOGS) || make(clean)
SUBDIR+=	tools
.endif

SUBDIR+=	share

.include <bsd.subdir.mk>
