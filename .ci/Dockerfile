FROM debian:buster

RUN apt-get update && \
    # required for compiling, building and debugging
    apt-get install -y build-essential gdb cmake ninja-build && \
    # code formatting
    apt install -y clang-format && \
    apt-get install -y clang-tidy && \
    # clean up
    apt-get autoremove -y && \
    apt-get clean -y
