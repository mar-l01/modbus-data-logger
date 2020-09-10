#!/bin/bash

# lib folder of target architecture (required for libmodbus.so and libpthread.so)
TARGET_ARCHITECTURE=x86_64-linux-gnu

# by default 'Debug' build mode is set
BUILD_TYPE=Debug

printHelp()
{
   echo ""
   echo "Usage: $0 [-a] [-d|-r]"
   echo -a "\t -a If '-a' is given, application is built for given target architecture (by default: x86_64-linux-gnu)"
   echo -d "\t -d If '-d' is given, application is built in 'Debug' mode"
   echo -r "\t -r If '-r' is given, application is built in 'Release' mode"

   exit 1
}

while getopts ":a:dr" opt
do
   case "$opt" in
      a ) TARGET_ARCHITECTURE=${OPTARG} ;;
      d ) BUILD_TYPE=Debug ;;
      r ) BUILD_TYPE=Release ;;
      ? ) printHelp ;;
   esac
done

USR_LIB_ARCH=/usr/lib/${TARGET_ARCHITECTURE}
echo "-- Compiling application for target: ${TARGET_ARCHITECTURE}"

# set up some variables required
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`
BUILD_DIR="${SCRIPT_DIR}/../builds/build_shared/debug"

if [ "${BUILD_TYPE}" = Release ]
then
   echo "-- Compiling shared library in 'Release' mode"
   BUILD_DIR="${SCRIPT_DIR}/../builds/build_shared/release"
else
   echo "-- Compiling shared library in 'Debug' mode"
fi

# create build directory if it is not present
if [ ! -d "${BUILD_DIR}" ]; then
  mkdir -p ${BUILD_DIR}
  echo "Created build directory: ${BUILD_DIR}"
fi

# navigate to build folder
cd ${BUILD_DIR}

# set up ninja building for cmake (with or without unittests)
cmake -G Ninja \
   -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
   -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
   -DCMAKE_ENABLE_TESTING=OFF \
   -DCMAKE_USR_LIB_ARCH=${USR_LIB_ARCH} \
   -DCMAKE_BUILD_OBJECT=SharedLibrary \
   ../../..

# build project using ninja
cd ${BUILD_DIR}
ninja
