#!/bin/bash
# compile luau
cd luau
mkdir -p .build && cd .build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --target Luau.Repl.CLI --config RelWithDebInfo
cmake --build . --target Luau.Analyze.CLI --config RelWithDebInfo
cd ../..

mkdir -p .build
cd .build
emcmake cmake .. 
cmake --build .

cd ..
cp .build/wasm_luau.wasm out/
cp .build/wasm_luau.js out/
# firefox web/index.html
