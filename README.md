# Flexus Instructions

## How to Build Environment

* Install a compatible version of `gcc`:

```sh
spack env create qflex qflex.yaml
spack install
```

* Set the recently installed version of `gcc` as default:

```sh
spack load gcc@8.5.0
```

* build `boost` from scratch (run ./build_boost.sh)
```sh
export BOOST="boost_1_70_0"
export BOOST_BUILD=$(pwd)/boost_1_70_0-build
wget https://boostorg.jfrog.io/artifactory/main/release/1.70.0/source/boost_1_70_0.tar.gz -O /tmp/${BOOST}.tar.gz
tar -xf /tmp/${BOOST}.tar.gz
cd ./${BOOST}/
./bootstrap.sh --prefix=$BOOST_BUILD
./b2 -j8
sudo ./b2 install
```

## How to Use CMake to Compile Flexus

* example options
```sh
-DTARGET_PLATFORM=arm
-DSELECTED_DEBUG=vverb
-DSIMULATOR=KnottyKraken
-DCMAKE_C_COMPILER=gcc
-DCMAKE_CXX_COMPILER=g++
-DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include
-DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib
```

* Add options by `-D${OPTION_NAME}=${OPTION}` after `cmake`.
* build `XFMKraken.so`, `KeenKraken.so` and `KnottyKraken.so` from scratch (run ./build_krakens.sh)
```sh
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include -DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib -DSIMULATOR=XFMKraken .
LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qflex/.spack-env/view/lib:./boost_1_70_0-build/lib make -j

cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include -DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib -DSIMULATOR=KeenKraken .
LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qflex/.spack-env/view/lib:./boost_1_70_0-build/lib make -j

cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include -DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib -DSIMULATOR=Harness .
LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qflex/.spack-env/view/lib:./boost_1_70_0-build/lib make -j

# Running with keen-kraken
LD_LIBRARY_PATH=$SPACK_ROOT/var/spack/environments/qflex/.spack-env/view/lib:$BOOST_BUILD/lib:$LD_LIBRARY_PATH ./../scripts/captain/captain ../scripts/captain/keen_config/system.ini -o output/
```

* Use `make clean` to only remove `*.a` and `*.so` files.
* Use `make clean_cmake` to remove all of the files and folders that cmake produces.
* If you are using non-default install paths for `boost` and/or `gcc`, modify the location of dynamic libraries before executing `make`

```sh
$ LD_LIBRARY_PATH=<path to dynamic libraries> make -j
```

## How to Use CMake to Compile Flexus Testing Harness

* Set the `SIMULATOR` variable to `Harness` while running `cmake`:

```sh
cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DBOOST_INCLUDEDIR=$(pwd)/boost_1_70_0-build/include -DBOOST_LIBRARYDIR=$(pwd)/boost_1_70_0-build/lib -DSIMULATOR=Harness .
LD_LIBRARY_PATH=./boost_1_70_0-build/lib make -j
```

* Use `make clean` to only remove `*.a` and `*.so` files.
* Use `make clean_cmake` to remove all of the files and folders that cmake produces.
* Run the tests by executing `testing_harness`:

```sh
$ ./testing_harness
```
* If you are using non-default install paths for `boost` and/or `gcc`, modify the location of dynamic libraries before executing `make` and `testing_harness`

```sh
$ LD_LIBRARY_PATH=<path to dynamic libraries> make -j
$ LD_LIBRARY_PATH=<path to dynamic libraries> ./testing_harness
```

## Flexus Code Formatting

The preferred version of Ubuntu is 18.04. When you contribute to this project, please clang-format the code at first. Please follow these instructions:

```sh
$ sudo apt-get install clang-format-6.0
```

Use the `.clang-format` file from repository and locate it in the folder where you want to format the code.

Within the folder, use this command to format .cpp/.hpp/.c/.h files:

```sh
$ find . -type f -and \( -name "*.hpp" -or -name "*.cpp" -or -name "*.h" -or -name "*.c" \) | xargs clang-format -i -style=file
```

Use the `clang_format_test.sh` file to check whether the formatting is successful or not. If it is successful, there will be green works showing `All source code in commit are properly formatted`. Otherwise, there will be red warning showing `Found formatting errors!` and which file where the problem is in.
