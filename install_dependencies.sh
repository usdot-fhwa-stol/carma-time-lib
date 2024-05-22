#!/bin/bash

set -ex
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
source /etc/lsb-release
echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports ${DISTRIB_CODENAME} main restricted universe multiverse" >> /etc/apt/sources.list
echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports ${DISTRIB_CODENAME}-updates main restricted universe multiverse" >> /etc/apt/sources.list
echo "deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports ${DISTRIB_CODENAME}-security main restricted universe multiverse" >> /etc/apt/sources.list
apt update
apt install -y python3-dev 
# TODO
