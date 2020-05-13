#!/bin/bash

# unittests are not enabled by default
UNITTESTS_ENABLED=OFF

printHelp()
{
   echo ""
   echo "Usage: $0 [-t]"
   echo -e "\t -t If '-t' is given, unittests are build"

   exit 1
}

while getopts "t" opt
do
   case "$opt" in
      t ) UNITTESTS_ENABLED=ON ;;
      ? ) printHelp ;;
   esac
done

if [ "${UNITTESTS_ENABLED}" = ON ]
then
   echo "Compiling application with unittests";
else
  echo "Compiling application without unittests";
fi

# set up some variables required
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`
BUILD_DIR="${SCRIPT_DIR}/../builds/build_native"

# create build directory if it is not present
if [ ! -d "${BUILD_DIR}" ]; then
  mkdir -p ${BUILD_DIR}
  echo "Created build directory: ${BUILD_DIR}"
fi

# navigate to build folder
cd ${BUILD_DIR}

# set up ninja building for cmake (with or without unittests)
cmake -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_ENABLE_TESTING=${UNITTESTS_ENABLED} ../..

# build project using ninja
cd ${BUILD_DIR}
ninja
