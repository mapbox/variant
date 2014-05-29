#!/usr/bin/env bash

set -e -u
set -o pipefail

# ppa for latest boost
sudo add-apt-repository -y ppa:boost-latest/ppa
# ppa for g++ 4.7 and 4.8
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update -y

# install boost headers and g++ upgrades
sudo apt-get -y install boost1.55 gcc-4.8 g++-4.8 gcc-4.7 g++-4.7

# run tests against g++ 4.7
export CXX="g++-4.7"; export CC="gcc-4.7"
make test
make bench
make clean

# run tests against g++ 4.7
export CXX="g++-4.8"; export CC="gcc-4.8"
make test
make bench
make clean

# compare object sizes against boost::variant
make sizes `brew --prefix`/include/boost/variant.hpp
make clean

# test building with gyp
make gyp