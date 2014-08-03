#
# $Id: Makefile,v 1.1 2003/10/31 16:17:33 nivit Exp $
#

SUBDIR!=	find ${.CURDIR} -type d -maxdepth 1
SUBDIR:=	${SUBDIR:C/(\.\/)|(.*nls)|(.*CVS)//:T}

.include <bsd.subdir.mk>
