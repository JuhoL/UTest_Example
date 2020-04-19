#!/bin/bash
cmake .
cmake --build .
ctest --output-on-failure
