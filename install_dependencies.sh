#!/bin/bash

set -ex
# export BUILD_ARCHITECTURE=arm64
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
echo "BUILD_ARCHITECTURE=${BUILD_ARCHITECTURE}"
if [[ "${BUILD_ARCHITECTURE}" =~ arm* ]]; then
    echo "WARNING: Python module support currently only exists for native x64 builds. Skipping Python module binding ... "
else
    /opt/carma/scripts/install_dependencies_script.sh python3-dev
fi
