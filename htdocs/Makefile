#
# This Makefile is usable only locally (if you have 'forrest',
# (see http://forrest.apache.org/), textproc/docproj port, and
# the source of the FreeBSD doc tree.
#
# $Id: Makefile,v 1.6 2005/08/10 15:41:23 nivit Exp $
#

HOMEDIR=	`pwd`
BUILDDIR=	${HOMEDIR}/build
SITEDIR=	${BUILDDIR}/site/
HTDOCS=		${HOMEDIR}/htdocs
DATE!=		date +%d%m%Y-%H%M%S
HTDOCS-TAR-GZ?=	htdocs-${DATE}.tar.gz
UFMCONTROL_SITE=	http://ufmcontrol.sourceforge.net
UFMCONTROL_UPLOAD=	nivit@ufmcontrol.sourceforge.net:/home/groups/u/uf/ufmcontrol
UFMCONTROL_VERSION=	0.2.2

###########
# TARGETS #
###########
all: build htdocs upload

build:
	@forrest -Dforrest.jvmargs=-Djava.awt.headless=true

htdocs: build
	@mkdir ${HTDOCS}
	@cp -R ${SITEDIR} ${HTDOCS}
	@cd ${HOMEDIR}
	@mkdir ${HTDOCS}/ports
	@mkdir ${HTDOCS}/ports/audio
	@cp ${HOMEDIR}/ports/audio/ufmcontrol* ${HTDOCS}/ports/audio
	@cp ${HOMEDIR}/ports/audio/ufmcontrol-${UFMCONTROL_VERSION}.shar ${HTDOCS}/ports/audio/ufmcontrol.shar
	@cd ${HOMEDIR}
	@cd ${HTDOCS} && fetch ${UFMCONTROL_SITE}/ufmcontrol.msg

site: htdocs
	@cd ${HOMEDIR} && tar -zcvf ${HTDOCS-TAR-GZ} htdocs

upload: site
	@scp ${HTDOCS-TAR-GZ} ${UFMCONTROL_UPLOAD}/${HTDOCS-TAR-GZ}

clean:
	@rm -fR ${HTDOCS}
	@rm -fR ${BUILDDIR}

cleanall: clean
	@rm -f htdocs-*.tar.gz
