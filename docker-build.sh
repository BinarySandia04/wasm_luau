#!/bin/bash
docker build -t wasm_luau .
id=$(docker create wasm_luau)
docker cp $id:/src/out/wasm_luau.js ./out/
docker cp $id:/src/out/wasm_luau.wasm ./out/
docker rm -v $id
