#!/bin/bash

set -e
set -x

cmake --preset conan-release
cmake --build --preset conan-release
cd build/Release
./HelloCPP
