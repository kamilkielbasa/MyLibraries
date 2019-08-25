#!/bin/bash

if [ ! -d "build" ]; then
  mkdir build
fi

pushd build
cmake ..
make
./containers/array/tests/array_tests 2>/dev/null
./containers/darray/tests/darray_tests 2>/dev/null
./containers/rbt/tests/rbt_tests 2>/dev/null
./containers/list/tests/list_tests 2>/dev/null
popd
rm -r build
