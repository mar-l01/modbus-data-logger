FROM debian:buster

RUN apt-get update && \
    apt-get install -y apt-utils && \
    # git integration
    apt install -y git && \
    # nano editor
    apt install -y nano && \
    # required for compiling, building and debugging
    apt-get install -y build-essential gdb cmake ninja-build && \
    # code formatting
    apt install -y clang-format && \
    apt-get install -y clang-tidy && \
    # clean up
    apt-get autoremove -y && \
    apt-get clean -y && \
    # add user 'docker'
    useradd -m docker

USER docker
