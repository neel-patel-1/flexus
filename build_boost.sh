#!/bin/bash
export BOOST="boost_1_70_0"
export BOOST_BUILD=$(pwd)/boost_1_70_0-build
wget https://boostorg.jfrog.io/artifactory/main/release/1.70.0/source/boost_1_70_0.tar.gz -O /tmp/${BOOST}.tar.gz
tar -xf /tmp/${BOOST}.tar.gz
cd ./${BOOST}/
./bootstrap.sh --prefix=$BOOST_BUILD
./b2 -j8
sudo ./b2 install