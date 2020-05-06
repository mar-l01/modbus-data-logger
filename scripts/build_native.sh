#!/bin/bash

SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
BUILD_DIR="$SCRIPT_DIR/../builds/build_native"

# create build directory if it is not present
if [ ! -d "$BUILD_DIR" ]; then
  mkdir -p $BUILD_DIR
  echo "Created build directory: $BUILD_DIR"
fi

# navigate to build folder
cd $BUILD_DIR

# set up ninja building for cmake
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

# build project using ninja
cd $BUILD_DIR
ninja
