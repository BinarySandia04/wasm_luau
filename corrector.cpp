#include <cstring>
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>

#include "luau/Compiler/include/luacode.h"
#include "luau/VM/include/lua.h"
#include "luau/VM/include/lualib.h"
#include <assert.h>

#ifdef GLOBAL_INPUT_PATH
#define INPUT_FILENAME "/tmp/input.lua"
#else
#define INPUT_FILENAME "input.lua"
#endif

static void writestring(const char *s, size_t l) { fwrite(s, 1, l, stdout); }

static int l_print(lua_State *L) {
  int n = lua_gettop(L);

  for (int i = 1; i <= n; i++) {
    size_t l;
    const char *s = luaL_tolstring(L, i, &l);
    if (i > 1)
      writestring(" ", 1);
    writestring(s, l);
    lua_pop(L, 1); // pop result
  }
  writestring("\n", 1);
  return 0;
}

/*
static int l_read(lua_State *L){
    int n = lua_gettop(L);
    
    // Ajuntem tots els arguments a s
    std::string prompt = "";
    for(int i = 1; i <= n; i++){
        size_t l;
        const char *in = luaL_tolstring(L, i, &l);
        prompt += std::string(in);
        if(i > 1) prompt += " ";
    }

    std::string readString;

    // Mostrem la linea del prompt per pantalla
    writestring(prompt.c_str(), prompt.size());

    // Llegim la entrada i la retornem
    std::getline(std::cin, readString);
    fflush(stdin);
    
    const char *ret = readString.c_str();
    lua_pushlstring(L, ret, strlen(ret));
    return 1;
}
*/

static const struct luaL_Reg printlib[] = {
    {"print", l_print},
    // {"read", l_read},
    {NULL, NULL} /* end of array */
};

extern int luaopen_deflib(lua_State *L) {
  lua_getglobal(L, "_G");
  luaL_register(L, NULL, printlib);
  lua_pop(L, 1);
  return 0;
}

int main() {
  std::fstream input_file(INPUT_FILENAME);
  std::stringstream input_buffer;
  input_buffer << input_file.rdbuf();
  std::string content = input_buffer.str();
  input_file.close();

  bool error = false;
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_deflib(L);

  char *bytecode;
  size_t bytecodeSize = 0;
  bytecode = luau_compile(content.c_str(), content.length(), NULL, &bytecodeSize);
  if(luau_load(L, "main", bytecode, bytecodeSize, 0) != LUA_OK){
    error = true;
    free(bytecode);
  }
  free(bytecode);

  if(lua_pcall(L, 0, 0, 0) != LUA_OK) error = true;

  /*
  Per si es vol executar més codi
  */
  content = "";
  std::string line = "";
  while(std::getline(std::cin, line)){
    content += line + "\n";
  }
  bytecode = luau_compile(content.c_str(), content.length(), NULL, &bytecodeSize);
  if(luau_load(L, "main", bytecode, bytecodeSize, 0) != LUA_OK){
    error = true;
    free(bytecode);
  }
  free(bytecode);

  if(lua_pcall(L, 0, 0, 0) != LUA_OK) error = true;
  lua_close(L);

  return 0;
}
