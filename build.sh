#!/bin/bash
mkdir -p .build
cd .build
emcmake cmake .. 
cmake --build . -j

cd ..
cp .build/wasm_luau.wasm web/
cp .build/wasm_luau.js web/
# firefox web/index.html
