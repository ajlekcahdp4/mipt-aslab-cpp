# 04-kd-tree
KD-tree for N-dimensional points

## 1. How to build
### Linux
```sh
git submodule init
git submodule update
cmake -S ./ -B build/ -DCMAKE_BUILD_TYPE=Release
cd build/
make -j12 install
```

## 2. How to run tests
```sh
# Navigate to build directory and run unit and end-to-end tests
cd build/
ctest
```

## 3. Test driver program
The main test driver is _closest_.

```sh
# Build and install
cd build/
make -j12 install
cd ../test/closest

bin/closest < resources/small0.dat
```