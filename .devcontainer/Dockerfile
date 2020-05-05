FROM debian:buster

RUN apt-get update && \
    # git integration
    apt install -y git && \
    # nano editor
    apt install -y nano && \
    # code formatting
    apt install -y clang-format && \
    # required for compiling, building and debugging
    apt-get install -y build-essential gdb cmake ninja-build && \
    # clean up
    apt-get autoremove -y && \
    apt-get clean -y && \
    # add user 'docker'
    useradd -m docker

USER docker
