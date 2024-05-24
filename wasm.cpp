#include <cstring>
#include <iostream>
#include <string>
#include <exception>

#include "luau/Compiler/include/luacode.h"
#include "luau/VM/include/lua.h"
#include "luau/VM/include/lualib.h"
#include <assert.h>

#include <emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

EM_JS(void, js_luau_print, (const char *text, int lentext), {
    luauPrint(UTF8ToString(text, lentext));
});

EM_JS(void, js_luau_error, (const char *text, int lentext), {
    luauError(UTF8ToString(text, lentext));
});

EM_JS(char*, js_luau_read, (const char *text, int lentext), {
    str = luauRead(UTF8ToString(text, lentext));
    pointer = Module._malloc(lengthBytesUTF8(str) + 1);
    stringToUTF8(str, pointer, lengthBytesUTF8(str) + 1);
    return pointer;
});

void writestring(const char *c, size_t l){
    js_luau_print(c, l);
}

static int l_print(lua_State *L) {
  int n = lua_gettop(L);

  for (int i = 1; i <= n; i++) {
    size_t l;
    const char *s = luaL_tolstring(L, i, &l); 
    if (i > 1) writestring("\t", 1);
    writestring(s, l);
    lua_pop(L, 1); // pop result
  }
  writestring("\n", 1);
  return 0;
}

static int l_read(lua_State *L){
    int n = lua_gettop(L);
    
    std::string s = "";
    for(int i = 1; i <= n; i++){
        size_t l;
        const char *in = luaL_tolstring(L, i, &l);
        s += std::string(in);
        if(i > 1) s += " ";
    }

    const char *prompt = s.c_str();
    const char *ret = js_luau_read(prompt, strlen(prompt));
    lua_pushlstring(L, ret, strlen(ret));
    return 1;
}

static const struct luaL_Reg printlib [] = {
  {"print", l_print},
  {"read", l_read},
  {NULL, NULL} /* end of array */
};

extern int luaopen_deflib(lua_State *L)
{
  lua_getglobal(L, "_G");
  luaL_register(L, NULL, printlib);
  lua_pop(L, 1);
  return 0;
}

int main() {
    // std::cout << "Loaded succesfully Lua lib" << std::endl;
    
    return 0;
}

void throw_lua_error(lua_State *L){
  size_t l;
  const char *s = luaL_tolstring(L, -1, &l);
  js_luau_error(s, l);
}

void exec(std::string content) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_deflib(L);

  char *bytecode;
  size_t bytecodeSize = 0;
  bytecode = luau_compile(content.c_str(), content.length(), NULL, &bytecodeSize);
  if(luau_load(L, "main", bytecode, bytecodeSize, 0) != LUA_OK){
    throw_lua_error(L);
    free(bytecode);
  }
  free(bytecode);

  if(lua_pcall(L, 0, 0, 0) != LUA_OK) throw_lua_error(L);
  lua_close(L);
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("exec", &exec);
}
