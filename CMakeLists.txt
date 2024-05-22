cmake_minimum_required(VERSION 3.8)

project(wasm_luau)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory("luau")

add_executable(wasm_luau main.cpp)
target_link_libraries(wasm_luau PUBLIC Luau.Compiler)
target_link_libraries(wasm_luau PUBLIC Luau.VM)
target_link_libraries(wasm_luau PUBLIC embind)
add_dependencies(wasm_luau Luau.Compiler Luau.VM)
