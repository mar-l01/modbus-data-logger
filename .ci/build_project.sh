#!/bin/bash

SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
BUILD_DIR="$SCRIPT_DIR/../scripts"

# execute build script
./$BUILD_DIR/build_native.sh
