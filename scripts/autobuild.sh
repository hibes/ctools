#!/bin/bash

# Get directory path of *this* script file and exit if is not set, NULL, or an empty string
SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd -P )"
SCRIPTS_DIR="${SCRIPTS_DIR:?}"

. ${SCRIPTS_DIR}/common.lib.sh

main() {
  install_filesystem_watcher

  if is_linux; then
    err "TODO: Implement me"
  elif is_macos; then
    while true; do
      FILES=$(get_files)
      fswatch -1 ${FILES} | while read -d "" event; do \
        err "Detected a change in file ${event}.\n\n"
        npm run build
      done
    done
  fi
}

get_files() {
  find ${SCRIPTS_DIR}/.. \
    -name "CMakeLists.txt" -or \
    -name "*.c" -or \
    -name "*.cpp" -or \
    -name "*.h" -or \
    -name "*.hpp"
}

main $@
