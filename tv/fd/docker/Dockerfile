# FROM ubuntu:14.04
# FROM nvidia/cuda:devel
FROM nvidia/cuda:11.4.1-devel-ubuntu20.04

# Python support
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive \
    apt-get install -y --no-install-recommends \
        autoconf \
        automake \
        autopoint \
        build-essential \
        ccache \
        checkinstall \
        # cuda-toolkit-10-2 \
        graphviz \
        iputils-ping \
        libcurl4-openssl-dev \
        libgl1-mesa-dev \
        libgles2-mesa-dev \
        libglew-dev \
        # libglew2.0 \
        libglib2.0-dev \
        libpcap0.8 \
        libpcap0.8-dev \
        libssl-dev \
        libtool \
        libusb-dev \
        ninja-build \
        python3-dev \
        python3-pip \
        python3-ply \
        python3-setuptools \
        python3-six \
        python3-tk \
        unzip \
        valgrind \
        xorg-dev && \
    rm -rf /var/lib/apt/lists/*

# apt-get install --assume-yes --no-install-recommends \
#         cuda-toolkit-10-2 \