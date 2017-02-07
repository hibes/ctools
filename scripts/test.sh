#!/bin/bash

# Get directory path of *this* script file and exit if is not set, NULL, or an empty string
SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd -P )"
SCRIPTS_DIR="${SCRIPTS_DIR:?}"

. ${SCRIPTS_DIR}/common.lib.sh

RTRN=${1}

if [[ "${RTRN}" == "" ]]; then
  RTRN=0
fi

# I don't particularly want this feature for now, but I might later.
EXIT_ON_TEST_FAILURE=0

main() {
  if ! ${SCRIPTS_DIR}/build.sh 1; then
    return ${RTRN}
  fi

  for test_file in $(find cmake -name "test*" -type f); do
    if ! ${test_file} && [ ${EXIT_ON_TEST_FAILURE} -ne 0 ]; then
      return ${RTRN}
    fi
  done
}

main $@
