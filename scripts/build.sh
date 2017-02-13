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
  init

  mkdir -p ${CMAKE_DIR}

  pushd ${CMAKE_DIR}

  if ! cmake -v ..; then
    return ${RTRN}
  fi

  if ! VERBOSE=1 make -j4; then
    return ${RTRN}
  fi

  popd
}

main $@
