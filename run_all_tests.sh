#!/bin/bash

mkdir build
cd build
cmake ..
make
./data_structures/array/tests/array_tests
./data_structures/darray/tests/darray_tests
./data_structures/rbt/tests/rbt_tests
