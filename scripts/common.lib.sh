#!/bin/bash

CMAKE_DIR=cmake
DEFAULT_EXIT_CODE=0
DEFAULT_ERROR_CODE=-1

# Always print commands before executing.
set -x
# Always stop on error
set -e

err() {
  echo -e "$@" >&2
}

is_linux() {
  if uname -a | grep "^Linux" > /dev/null 2>&1; then
    return ${DEFAULT_EXIT_CODE}
  else
    return ${DEFAULT_ERROR_CODE}
  fi
}

is_macos() {
  if uname -a | grep "^Darwin" > /dev/null 2>&1; then
    return ${DEFAULT_EXIT_CODE}
  else
    return ${DEFAULT_ERROR_CODE}
  fi
}

inotifywait_installed() {
  is_tool_installed inotifywait

  return $?
}

fswatch_installed() {
  is_tool_installed fswatch

  return $?
}

is_tool_installed() {
  if [ -f "$(which $1)" ]; then
    return ${DEFAULT_EXIT_CODE}
  else
    return ${DEFAULT_ERROR_CODE}
  fi
}

install_filesystem_watcher() {
  if is_linux && ! inotifywait_installed; then
    sudo apt update && sudo apt install inotify-tools
  elif is_macos && ! fswatch_installed; then
    brew install fswatch
  fi
}

init() {
  install_filesystem_watcher
}
