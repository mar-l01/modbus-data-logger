#!/bin/bash

# install requirements
apt-get update && \

# git integration
apt install -y git && \
# required for compiling, building and debugging
apt-get install -y build-essential gdb cmake ninja-build && \
# modbus library requirements
apt-get install -y automake autoconf libtool && \

# install 'pymodbus' library using PyPi for Modbus server and client for HW test
apt-get install -y python-pip && \
yes | pip install pymodbus && \

# clone 'libmodbus' library
apt-get install -y libmodbus-dev && \

# clone 'better-enums' repo (header only library)
cd /usr/include && \
git clone https://github.com/aantron/better-enums && \

# install 'nlohman/json' library
apt-get install -y nlohmann-json3-dev && \

# clean up
apt-get autoremove -y && \
apt-get clean -y
