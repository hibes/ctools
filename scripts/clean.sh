#!/bin/bash

# Get directory path of *this* script file and exit if is not set, NULL, or an empty string
SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd -P )"
SCRIPTS_DIR="${SCRIPTS_DIR:?}"

. ${SCRIPTS_DIR}/common.lib.sh

RTRN=${1}

if [[ "${RTRN}" == "" ]]; then
  RTRN=0
fi

main() {
  if ! rm -rf ${CMAKE_DIR}; then
    return ${RTRN}
  fi
}

main $@
