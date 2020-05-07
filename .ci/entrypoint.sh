#!/bin/bash

# stop execution upon errors
set -e

# obtain scriptname
SCRIPT=$1

echo $GITHUB_WORKSPACE
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
echo $SCRIPT_PATH
echo $SCRIPT_DIR

BUILD_SCRIPT_DIR="$SCRIPT_DIR/../.ci"

EXE_SCRIPT="$BUILD_SCRIPT_DIR/SCRIPT"


# do not allow an empty scriptname
if [ -z "${EXE_SCRIPT}" ]; then
    echo "A script to execute needs to be provided.."
    exit 1
fi

# check if script exists
if [ ! -e "${EXE_SCRIPT}" ]; then
    echo "The provided script '${EXE_SCRIPT}' does not exist.."
    exit 1
fi

# if not already done, make sure script can be executed
chmod 755 "${EXE_SCRIPT}"

# run the script
./${EXE_SCRIPT}

# in case of an error we wouldn't have reached this line
exit 0
