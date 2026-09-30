#!/usr/bin/env bash
# L1: build and boot OP-TEE on QEMU ARMv8 (TF-A -> OP-TEE -> U-Boot -> Linux)
set -e
mkdir -p ~/optee && cd ~/optee
repo init -u https://github.com/OP-TEE/manifest.git -m qemu_v8.xml
repo sync -j4 --no-clone-bundle
cd build
make -j2 toolchains
make -j8
make run-only
