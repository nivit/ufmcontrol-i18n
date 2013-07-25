#
# $Id: Makefile,v 1.1 2003/10/31 16:17:30 nivit Exp $
#

.if !defined(PREFIX)
PREFIX=			/usr/local
.endif

MTREE_CMD?=		mtree
MTREE_FILE?=		BSD.local.dist

.if !defined(NOPORTDOCS)
MTREE_EXCLUDE?=		exclude-list
.else
MTREE_EXCLUDE?=		exclude-docs-list
.endif

CLEANFILES=		${MTREE_FILE}

MTREE_SPEC:=		-L -c -d -i -n -k mode,uname,gname,link \
			-p ${.CURDIR}/../.. -X ${MTREE_EXCLUDE}
MTREE_OPTIONS:=		-U -d -e  -p ${PREFIX} -f ${MTREE_FILE}

install:
	${MTREE_CMD} ${MTREE_SPEC} > ${MTREE_FILE}
	${MTREE_CMD} ${MTREE_OPTIONS}

.include <bsd.prog.mk>
