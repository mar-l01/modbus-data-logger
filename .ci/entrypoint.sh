#!/bin/bash

# stop execution upon errors
set -e

# obtain scriptname
SCRIPT=$1

# build absolute path
SCRIPT_PATH="${GITHUB_WORKSPACE}/${SCRIPT}"

# do not allow an empty scriptname
if [ -z "${SCRIPT_PATH}" ]; then
    echo "A script to execute needs to be provided.."
    exit 1
fi

# check if script exists
if [ ! -e "${SCRIPT_PATH}" ]; then
    echo "The provided script '${SCRIPT_PATH}' does not exist.."
    exit 1
fi

# if not already done, make sure script can be executed
chmod 755 "${SCRIPT_PATH}"

# run the script
${SCRIPT_PATH}

# in case of an error we wouldn't have reached this line
exit 0
