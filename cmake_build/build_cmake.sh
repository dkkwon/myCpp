#! /bin/bash

set -e

cmake --version

export DK_ENV1=1000
export DK_ENV2="DK_TEST_ENV"
env | grep DK_ENV

rm -rf .build
mkdir -p .build
cd .build || exit

cmake ..
make

./test-cmake arg1 arg2
