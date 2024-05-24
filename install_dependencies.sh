#!/bin/sh

set -ex
# Get ubuntu distribution code name. All STOL APT debian packages are pushed to S3 bucket based on distribution codename.
echo "BUILD_ARCHITECTURE=${BUILD_ARCHITECTURE}"
if [[ "${BUILD_ARCHITECTURE}" == "x64" ]]; then
    /opt/carma/script/install_dependencies_script.sh python3-dev
else
    echo "NOTE: Python module support currently only exists for x64 builds. Only building C++ library ... "
fi
