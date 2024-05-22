#!/bin/bash

set -ex
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
cat /etc/apt/sources.list
apt update
apt install -y python3-dev 
# TODO
