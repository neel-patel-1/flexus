#!/bin/bash
[ ! -d "$SPACK_ROOT" ] && echo "enter qflex spack environment first" && exit
spack env activate qflex

rm -rf CMakeFiles/ CMakeCache.txt
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include -DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib -DSIMULATOR=KeenKraken .

LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qfle/.spack-env/view/lib:./boost_1_70_0-build/lib make -j 2

rm -rf CMakeFiles/ CMakeCache.txt
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include -DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib -DSIMULATOR=KnottyKraken .

LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qfle/.spack-env/view/lib:./boost_1_70_0-build/lib make -j 2
