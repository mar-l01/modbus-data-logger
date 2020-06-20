#!/bin/bash

# set up some variables required
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`
BUILD_DIR="${SCRIPT_DIR}/../builds/build_native/release"

# navigate to build folder
cd ${BUILD_DIR}

# execute application
./modbus_data_logger
