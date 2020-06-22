#!/bin/bash

# set up variables
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`

# execute release build
${SCRIPT_DIR}/build_native.sh -r
