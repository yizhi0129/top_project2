#!/bin/bash

rm -f ../src/cmake_install.cmake ../src/CMakeCache.txt ../src/Makefile ../src/my_executable
find ../src -maxdepth 1 -type f -name '*.dylib' -delete
find ../src -maxdepth 1 -type f -name '*.so' -delete
find ../src -maxdepth 1 -type f -name 'perf.da*' -delete
rm -rf ../src/CMakeFiles