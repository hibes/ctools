#!/bin/bash

CMAKE_DIR=cmake

# Always print commands before executing.
set -x
# Always stop on error
set -e

err() {
  echo "$@" >&2
}
