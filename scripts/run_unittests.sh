#!/bin/bash

SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`
BUILD_DIR="${SCRIPT_DIR}/../builds/build_native"

# change into build directory if it exists
if [ ! -d "${BUILD_DIR}" ]; then
  echo "Error: ${BUILD_DIR} not available! Make sure at least one project-build was executed.."
  exit 1
fi
cd ${BUILD_DIR}

# run unittests
./mdl_unittests
