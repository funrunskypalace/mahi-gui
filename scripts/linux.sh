#!/usr/bin/env bash

set -eu
export PATH="$PATH:$(npm bin)"
export OSVER=linux
export OOSTYPE=OpenOrder
# call protoc 
export JSOUT_DIR=../source/commonjs/message
export APIDIR=../api
export SRCDIR=../source

cd ..
BASEDIR=$(pwd)
yes | cp -rf CMakeLists-template-linux.txt CMakeLists.txt
mkdir -p app_home
cd app_home
mkdir -p config
mkdir -p lib
mkdir -p bin
cd ..
mkdir -p build
cd build
mkdir -p x64linux
cd x64linux
# del CMakeCache.txt
cmake ../.. -DCMAKE_INSTALL_PREFIX=$BASEDIR/target -DCMAKE_BUILD_TYPE=Release -DWIN32MT=off -D_CRT_NO_TIME_T=on  -D_CRT_NO_TIME_T=off -DNOT_QTCREATORPRO=ON
cd ../../scripts
cd ../build/x64linux
make -j$(nproc)
make install
ODIR01=$(pwd)
cd ../../scripts
