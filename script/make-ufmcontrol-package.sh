#!/bin/sh
# $Revision$
# $Author$
# $HeadURL$

if [ -z "$1" ]; then
    echo "usage: $(basename $0) <nome_modulo>";
    exit 1;
fi

DESTDIR="${HOME}/prog/sourceforge.net/ufmcontrol-packages"
MODULO=$(basename $1)
MODULODIR="$(dirname $1)/"
SVN_SRC_MODULES="${HOME}/prog/sourceforge.net/ufmcontrol/svn/trunk"
OUTPUTDIR=$(mktemp -d)
SUBSTITUTIONS="version"
SVNROOT=$(pwd)

sed_substitution () {
    if [ -f "${OUTPUTDIR}/${MODULODIR}${MODULO}/$1" ]; then
        VERSION="$(cat "${OUTPUTDIR}/${MODULODIR}${MODULO}/$1")"

        SUBSTITUTION=$(basename -s .txt $1)
        echo "===> SUBSTITUTION(${SUBSTITUTION}) => ${VERSION} <==="
        SED_SUBS="s:@${SUBSTITUTION}@:${VERSION}:g"

        for f in $(find ${OUTPUTDIR}/${MODULODIR}${MODULO} -type f)
        do
            sed -i '' -e "${SED_SUBS}" ${f}
        done;
    fi
}

if [ -d "${OUTPUTDIR}" ]; then

    if [ "${MODULODIR}" != "./" ]; then
        mkdir -p ${OUTPUTDIR}/${MODULODIR}
    else
        MODULODIR="/"
    fi

    svn export "${SVN_SRC_MODULES}"/"${MODULODIR}${MODULO}" "${OUTPUTDIR}/${MODULODIR}${MODULO}"

    # esegue eventuale Makefile
    MAKEFILEDIR="$(pwd)/${MODULODIR}${MODULO}"
    if [ -f "${MAKEFILEDIR}/Makefile" ]; then
        (cd ${MAKEFILEDIR} && env OUTPUTDIR=${OUTPUTDIR}/${MODULODIR}${MODULO} make)
    fi

    for t in ${SUBSTITUTIONS}
    do
    if [ -f "${OUTPUTDIR}/${MODULODIR}${MODULO}/${t}.txt" ]; then
        sed_substitution ${t}.txt
    fi
    done;

    # comprime modulo
    if [ -f "${OUTPUTDIR}/${MODULODIR}${MODULO}/version.txt" ]; then
        MOD_VERSION=i18n-$(cat ${OUTPUTDIR}/${MODULODIR}${MODULO}/version.txt)
    else
        MOD_VERSION='0.0.0'
    fi
    if [ -f "$1"/mod_name.txt ]; then
        TARNAME=$(cat "$1/mod_name.txt")
    else
        TARNAME=${MODULO}
    fi
    TARARCHIVE="${TARNAME}-${MOD_VERSION}"

    cd ${OUTPUTDIR}/${MODULODIR} && \
    mv ${MODULO} ${TARARCHIVE} && \
    tar -X ${SVNROOT}/.tar_ignore -zcvf ${TARARCHIVE}.tar.gz ${TARARCHIVE}
    # copia file compresso in DESTDIR
    if [ -f ${DESTDIR}/${TARARCHIVE}.tar.gz ]; then
        DATE=$(ls -l -D "%Y%m%H%M" ${DESTDIR}/${TARARCHIVE}.tar.gz | cut -d ' ' -f 9 -)
        mv ${DESTDIR}/${TARARCHIVE}.tar.gz ${DESTDIR}/${TARARCHIVE}.tar.gz.${DATE}
        echo "==> Eseguito backup di ${DESTDIR}/${TARARCHIVE}"
    fi
    cp ${OUTPUTDIR}/${MODULODIR}${TARARCHIVE}.tar.gz ${DESTDIR}/
    echo "==> MOD (versione: ${MOD_VERSION}) copiato in ${DESTDIR}"

    # rimuove directory temporanea
    echo ${OUTPUTDIR}
    #rm -r ${OUTPUTDIR}
fi
