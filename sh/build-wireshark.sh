#!/bin/bash
#
# Wireshark 3 changes to using cmake3/ninja as its build system.
# I don't use that enough to remember it, so this takes the source
# tar.xz file and does everything. Assumes all dependencies are met.

if [[ $# -eq 0 ]] ; then
    echo "Usage: build-wireshark.sh wireshark-x.y.z.tar.zx"
    exit 1
fi

if [[ "$1" =~ wireshark-[0-9]\.[0-9]\.[0-9]\.tar\.xz ]]; then
    tar xf "$1"
    mkdir -p wsbuild
    cd wsbuild
    DIR=$(echo "$1" | sed -e 's/.tar.xz//g')
    cmake3 -G Ninja "../$DIR/"
    ninja-build
    sudo ninja-build install
    cd ..
    rm -rf "wsbuild/"
    rm -rf "$DIR/"
else
    echo "Error: $1 doesn't appear to be wireshark-x.y.z.tar.xz"
    exit 1
fi
