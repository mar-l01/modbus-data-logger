#!/bin/bash

# stop execution upon errors
set -e

# obtain scriptname
SCRIPT_NAME=$1

# do not allow an empty scriptname
if [ -z "${SCRIPT_NAME}" ]; then
    echo "A script to execute needs to be provided.."
    exit 1
fi

# adapt path to script to github-workspace directory
SCRIPT="${GITHUB_WORKSPACE}/${SCRIPT}"

# check if script exists
if [ ! -e "${SCRIPT}" ]; then
    echo "The provided script '${SCRIPT}' does not exist.."
    exit 1
fi

# if not already done, make sure script can be executed
chmod 755 "${SCRIPT}"

# run the script
${SCRIPT}

# in case of an error we wouldn't have reached this line
exit 0
