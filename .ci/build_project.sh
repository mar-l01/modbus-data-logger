#!/bin/bash

# obtain path to build scripts
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`
BUILD_DIR="${SCRIPT_DIR}/../scripts"

# change into scripts directory
cd ${BUILD_DIR}

#
# define scripts to execute here
#

# execute build native version
BUILD_SCRIPT='build_native.sh'
chmod 755 "${BUILD_SCRIPT}"
./${BUILD_SCRIPT}
