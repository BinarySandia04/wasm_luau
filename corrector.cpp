#include <cstring>
#include <exception>
#include <iostream>
#include <string>

#include "luau/Compiler/include/luacode.h"
#include "luau/VM/include/lua.h"
#include "luau/VM/include/lualib.h"
#include <assert.h>

static void writestring(const char *s, size_t l) { fwrite(s, 1, l, stdout); }

static int l_print(lua_State *L) {
  int n = lua_gettop(L);

  for (int i = 1; i <= n; i++) {
    size_t l;
    const char *s = luaL_tolstring(L, i, &l);
    if (i > 1)
      writestring("\t", 1);
    writestring(s, l);
    lua_pop(L, 1); // pop result
  }
  writestring("\n", 1);
  return 0;
}

static const struct luaL_Reg printlib[] = {
    {"print", l_print}, {NULL, NULL} /* end of array */
};

extern int luaopen_deflib(lua_State *L) {
  lua_getglobal(L, "_G");
  luaL_register(L, NULL, printlib);
  lua_pop(L, 1);
  return 0;
}

int main() {
  std::string content = "print(readd())";
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  luaopen_deflib(L);

  char *bytecode;
  size_t bytecodeSize = 0;
  bytecode =
      luau_compile(content.c_str(), content.length(), NULL, &bytecodeSize);
  int a = luau_load(L, "main", bytecode, bytecodeSize, 0);
  free(bytecode);

  std::cout << "Res: " << a << std::endl;
  a += lua_pcall(L, 0, 0, 0);
 
  std::cout << "Res: " << a << std::endl;
  if(lua_isnone(L, -1)){
    std::cout << "None!!!" << std::endl;
  } else {
  size_t l;
  const char *s = luaL_tolstring(L, -1, &l);
  std::cout << s << std::endl;
  }

 lua_close(L);


  return 0;
}
