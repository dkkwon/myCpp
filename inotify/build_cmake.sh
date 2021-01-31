#! /bin/bash

set -e

cmake --version

rm -rf build
mkdir -p build
cd build || exit

cmake ..
make

# ./test ../watchedfolder/a.txt
./test
