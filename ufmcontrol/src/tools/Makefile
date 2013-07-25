#
# $Id: Makefile,v 1.1 2003/10/31 16:17:59 nivit Exp $
#

SCAN_DIR=	bin

.if ! defined (SCAN_DIR)
SCAN_DIR=	${.CURDIR:T}
.endif

.if ! defined (MASTER_CAT)
MASTER_CAT=	${.CURDIR}/../share/nls/en_US.ISO8859-1/en_US.ISO8859-1.msg
.endif

.if ! defined (TRANSLATORS)
TRANSLATORS=	${.CURDIR}/../share/doc/ufmcontrol-i18n/TRANSLATORS
.endif

.if ! defined (INITDIR)
INITDIR= ${.CURDIR}/..
.endif

.if ! defined (NLS_DIRS)
NLS_DIRS!=	find ${.CURDIR}/../share/nls -type d -maxdepth 1
NLS_DIRS:=	${NLS_DIRS:C/(.*CVS)|(.*nls\$)|(.*nls\/\$)|(.*-15\$)|(.*sr_YU.*-5\$)|(.*en_US.*)//}
.endif

.include <bsd.subdir.mk>

all:
.for dir in ${SCAN_DIR}
	-cd ${INITDIR}/${dir} && grep -n 'NLS(\".*\")' *.c > ${.CURDIR}/obj/${dir}_strings
	@awk -f ../master_catalog_header.awk ${dir}_strings > ${MASTER_CAT}
	@echo -n "Generating master catalog file ${MASTER_CAT:T}..."
	@awk -f ../make_master_catalog.awk ${dir}_strings >> ${MASTER_CAT}
	@echo " done."
.endfor

.for catalog in ${NLS_DIRS}
	@mv ${catalog}/${catalog:T}.msg ${catalog}/${catalog:T}.msg.old
	@echo -n "Updating slave catalog file ${catalog:T}..."
	@awk -f ../update_slave_catalog.awk -v master=${MASTER_CAT} -v slave=${catalog}/${catalog:T}.msg.old ${MASTER_CAT} > ${catalog}/${catalog:T}.msg
	@awk -f ../find_translator.awk -v translator=${catalog:T} ${TRANSLATORS} | cat ../slave_cvs_keywords - ${catalog}/${catalog:T}.msg > ${catalog}/${catalog:T}.msg.cvs
	@rm ${catalog}/${catalog:T}.msg
	@mv ${catalog}/${catalog:T}.msg.cvs ${catalog}/${catalog:T}.msg
	@rm ${catalog}/${catalog:T}.msg.*
	@rm ${MASTER_CAT}.*
	@echo " done."
.endfor

# Add CVS keywords to master catalog
	@cat ../master_cvs_keywords ${MASTER_CAT} > ${MASTER_CAT}.cvs
	@rm ${MASTER_CAT}
	@mv ${MASTER_CAT}.cvs ${MASTER_CAT}


.for dir in ${SCAN_DIR}
CLEANFILES+= ${.CURDIR}/obj/${dir}_strings
.endfor

clean:
	rm -f ${CLEANFILES}

