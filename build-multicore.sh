#!/bin/bash
# compile luau
cd luau
mkdir -p .build && cd .build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --target Luau.Repl.CLI --config RelWithDebInfo -j 8
cmake --build . --target Luau.Analyze.CLI --config RelWithDebInfo -j 8
cd ../..

# Build web
mkdir -p .build
cd .build
emcmake cmake .. -DWASM_BUILD=ON
cmake --build . -j 8

cd ..
cp .build/wasm_luau.wasm out/
cp .build/wasm_luau.js out/
# firefox web/index.html

# Build corrector
mkdir -p .build_c
cd .build_c
cmake .. -DWASM_BUILD=OFF
cmake --build . -j 8

cd ..
cp .build_c/corrector .
