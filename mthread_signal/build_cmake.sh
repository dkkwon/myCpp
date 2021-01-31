#! /bin/bash

set -e

cmake --version

rm -rf build
mkdir -p build
cd build || exit

cmake -DCMAKE_BUILD_TYPE=Debug ..
make

./test_dk
