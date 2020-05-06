#!/bin/sh

DIRECTORY="$PWD/builds/build_native"

# create build directory if it is not present
if [ ! -d "$DIRECTORY" ]; then
  mkdir -p $DIRECTORY
  echo "Created build directory: $DIRECTORY"
fi

# navigate to build folder
cd $DIRECTORY

# set up ninja building for cmake
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..

# build project using ninja
cd $DIRECTORY
ninja
