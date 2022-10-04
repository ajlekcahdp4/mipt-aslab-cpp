# 01-hwmx
Calculation of the matrix determinant 
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
The main test driver is _determinant_. It's recommended to build with Boost installed to get command line options.

```sh
# Build and install
cd build/
make -j12 install
cd ../test/determinant

# Help message
bin/determinant --help
# Available options:
#  -h [ --help ]         Print this help message
#  -m [ --measure ]      Print perfomance metrics
#  --hide                Hide output
#  --broad arg (=octree) Algorithm for broad phase (bruteforce, octree, uniform-grid)

# Run sample test
bin/determinant --measure --type double < resources/huge1.dat
# octree took 217.537ms to run
```