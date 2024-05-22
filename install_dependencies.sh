#!/bin/sh

set -ex
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
ls /etc/apt/sources.list.d/
apt-get update
apt-get install -y python3-dev 
# TODO
