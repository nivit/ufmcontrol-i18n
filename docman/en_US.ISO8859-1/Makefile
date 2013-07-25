#
# Requirements: ports/textproc/docproj
#
# $Id: Makefile,v 1.2 2003/09/22 10:01:23 nivit Exp $

DOC?= install

FORMATS?= html

INSTALL_COMPRESSED?=gz
INSTALL_ONLY_COMPRESSED?=

TIDYERRS= ${.CURDIR}/obj/tidy-errors
TIDYOPTS= -e -i -n -u -f ${TIDYERRS}

CLEANFILES+= ${TIDYERRS}

SRCS= install.sgml

DOC_PREFIX?= /usr/doc

.include "${DOC_PREFIX}/share/mk/doc.project.mk"
