#!/bin/bash
#
git submodule update --recursive --remote
# Compile luau
cd luau
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --target Luau.Repl.CLI --config RelWithDebInfo
cmake --build . --target Luau.Analyze.CLI --config RelWithDebInfo
