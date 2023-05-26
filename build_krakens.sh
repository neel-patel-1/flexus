#!/bin/bash
BOOST=/home/n869p538/qflex/flexus/boost_1_70_0-build
[ ! -d "$SPACK_ROOT" ] && echo "enter qflex spack environment first" && exit
spack env activate qflex

rm -rf CMakeFiles/ CMakeCache.txt
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=${BOOST}/include -DBOOST_LIBRARYDIR=${BOOST}/lib -DSIMULATOR=KeenKraken .

LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qflex/.spack-env/view/lib:${BOOST}/lib make -j

rm -rf CMakeFiles/ CMakeCache.txt
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=${BOOST}/include -DBOOST_LIBRARYDIR=${BOOST}/lib -DSIMULATOR=KnottyKraken .

LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qflex/.spack-env/view/lib:${BOOST}/lib make -j 2
