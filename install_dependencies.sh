#!/bin/sh

set -ex
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
apt install python3-dev && true
