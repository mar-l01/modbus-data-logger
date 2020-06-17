#!/bin/bash

# unittests are not enabled by default
UNITTESTS_ENABLED=OFF

# by default 'Debug' build mode is set
BUILD_TYPE=Debug

printHelp()
{
   echo ""
   echo "Usage: $0 [-t] [-d|-r]"
   echo -e "\t -t If '-t' is given, application is built in 'Debug' mode together with its unittests"
   echo -d "\t -d If '-d' is given, application is built in 'Debug' mode"
   echo -r "\t -r If '-r' is given, application is built in 'Release' mode"

   exit 1
}

while getopts ":tdr" opt
do
   case "$opt" in
      t ) UNITTESTS_ENABLED=ON
            BUILD_TYPE=Debug ;;
      d ) BUILD_TYPE=Debug ;;
      r ) BUILD_TYPE=Release ;;
      ? ) printHelp ;;
   esac
done

if [ "${UNITTESTS_ENABLED}" = ON ]
then
   echo "-- Compiling application with unittests"
else
   echo "-- Compiling application without unittests"
fi

# set up some variables required
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`
BUILD_DIR="${SCRIPT_DIR}/../builds/build_native/debug"

if [ "${BUILD_TYPE}" = Release ]
then
   echo "-- Compiling application in 'Release' mode"
   BUILD_DIR="${SCRIPT_DIR}/../builds/build_native/release"
else
   echo "-- Compiling application in 'Debug' mode"
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
   -DCMAKE_ENABLE_TESTING=${UNITTESTS_ENABLED} \
   ../../..

# build project using ninja
cd ${BUILD_DIR}
ninja
