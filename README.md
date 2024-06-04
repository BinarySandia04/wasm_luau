# wasm_luau

A wasm module to execute [Luau](https://github.com/luau-lang/luau) code inside a browser

## Usage

Import the files `wasm_luau.js` and `wasm_luau.wasm` inside your web project.
There is an example `index.html` about how to use the .wasm module

## Building

### Dependencies

- [emscripten](https://github.com/kripken/emscripten)
- Clang C/C++ compiler
- Cmake 3.8+
- Python 3.6.5+

### Compiling

Run
```shell
./build.sh`
```

This will compile luau and the web assembly module and will place it
inside `out/`

### Docker

The WebAssembly Module is compiled when creating a Docker image of
this repository, also, the Docker container can be used for
using doctests

Run
```shell
./docker-build.sh 
```

This will create a docker image named `wasm_luau` that compiles the assembly module and then
it places it inside `out/` on the host machine
