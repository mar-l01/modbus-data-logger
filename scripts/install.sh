#!/bin/bash

# set up variables
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname ${SCRIPT_PATH}`

# workplace directories
BUILD_DIR="${SCRIPT_DIR}/../builds/build_native/release"
RESOURCE_DIR="${SCRIPT_DIR}/../resources"
SERVICE_DIR="${SCRIPT_DIR}/../service"

# os install directories
INSTALL_DIR="/opt/mbdl"
SERVICE_INSTALL_DIR="/etc/systemd/mbdl"

# execute release build
${SCRIPT_DIR}/build_native.sh -r

# create install directory if it is not present
if [ ! -d "${INSTALL_DIR}" ]; then
  mkdir -p ${INSTALL_DIR}
  echo "Created install directory: ${INSTALL_DIR}"
fi

# move application binary to install directory
mv -f "${BUILD_DIR}/modbus-data-logger" "${INSTALL_DIR}"

# move configuration file to install directory
mv -f "${RESOURCE_DIR}/mbdl_config.json" "${INSTALL_DIR}"

# move start/stop scripts to install directory
mv -f "${SERVICE_DIR}/start_mbdl_service.sh" "${INSTALL_DIR}"
mv -f "${SERVICE_DIR}/stop_mbdl_service.sh" "${INSTALL_DIR}"

# create service directory if it is not present
if [ ! -d "${SERVICE_INSTALL_DIR}" ]; then
  mkdir -p ${SERVICE_INSTALL_DIR}
  echo "Created service directory: ${SERVICE_INSTALL_DIR}"
fi

# move .service file to service directory
mv -f "${SERVICE_DIR}/mbdl.service" "${SERVICE_INSTALL_DIR}"
