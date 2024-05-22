#include <iostream>


#include <assert.h>
#include "luau/VM/include/lua.h"
#include "luau/VM/include/lualib.h"
#include "luau/Compiler/include/luacode.h"

using namespace std;


int main() 
{  

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    string content; 
    char* bytecode;
    size_t bytecodeSize = 0;

    content = "print('Adios mundo!')";
                                     
    bytecode = luau_compile(content.c_str(), content.length(), NULL, &bytecodeSize);
    assert(luau_load(L, "foo2", bytecode, bytecodeSize, 0) == 0);
    free(bytecode); 
    
    lua_pcall(L, 0, 0, 0);

    lua_close(L);
    
    return 0;
    
}