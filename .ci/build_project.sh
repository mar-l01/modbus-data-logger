#!/bin/bash

SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
BUILD_DIR="$SCRIPT_DIR/../scripts"

# change into scripts directory
cd $BUILD_DIR

# execute build script
./build_native.sh
