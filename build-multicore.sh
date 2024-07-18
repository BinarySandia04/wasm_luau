#!/bin/bash

rm -r .build .build_c
# compile luau
cd luau

mkdir -p .build && cd .build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --target Luau.Repl.CLI --config RelWithDebInfo -j
cmake --build . --target Luau.Analyze.CLI --config RelWithDebInfo -j
cd ../..

# Build web
mkdir -p .build
cd .build
emcmake cmake .. -DWASM_BUILD=ON
cmake --build . -j

cd ..
cp .build/wasm_luau.wasm out/
cp .build/wasm_luau.js out/
# firefox web/index.html

# Build corrector
mkdir -p .build_c
cd .build_c
cmake .. -DWASM_BUILD=OFF
cmake --build . -j

cd ..
cp .build_c/corrector .
