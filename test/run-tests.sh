#!/bin/bash

# Return failure as soon as a command fails to execute

set -e

cd "$(dirname "$0")"

# Tests included with the repository

pushd asm
./test.sh
popd

pushd link
./test.sh
popd

# Test some significant external projects that use RGBDS
# When adding new ones, don't forget to add them to the .gitignore!

export PATH="$PWD/..:$PATH"

if [ ! -d pokecrystal ]; then
	git clone https://github.com/pret/pokecrystal.git --shallow-since=2018-06-04 --single-branch
fi
pushd pokecrystal
git fetch
git checkout fa7261a8eea2676746c130a517479e2a82a7d352
make clean
make -j
make compare
popd

if [ ! -d pokered ]; then
	git clone --recursive https://github.com/pret/pokered.git --shallow-since=2018-03-23 --single-branch
fi
pushd pokered
git fetch
git checkout ab696295b3d029663062238e8033882bb8cd9cff
make clean
make -j
make compare
popd

if [ ! -d ucity ]; then
	git clone https://github.com/AntonioND/ucity.git --shallow-since=2017-07-13 --single-branch
fi
pushd ucity
git fetch
git checkout b0635f12553c2fae947fd91aa54d4caa602d8266
make clean
make -j
popd
