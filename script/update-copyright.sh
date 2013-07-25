#!/bin/sh
if [ -z "$1" -o ! -d "$1" ]; then
    echo "usage: $0 <directory>"
    exit 1
fi

CWD=$(pwd)
CURRENT_YEAR=$(date +%Y)

FIND="/usr/bin/find"
FIND_ARGS="${1} -type f -print0"
#FIND_DELETE_BAK="${1} -type f -name '*.bak' -delete"

SED=/usr/bin/sed
SED_ARGS="-E -e s,([0-9]{4})-[0-9]{4},\1-${CURRENT_YEAR},g -i .bak"

XARGS=/usr/bin/xargs
XARGS_ARGS="-0 ${SED} ${SED_ARGS}"

(${FIND} ${FIND_ARGS} | ${XARGS} ${XARGS_ARGS})
(cd ${CWD} && ${FIND} ${1} -type f -name '*.bak' -delete)
