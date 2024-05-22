#!/bin/sh

set -ex
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
/opt/carma/scripts/install_dependencies.sh python3-dev 
# TODO
