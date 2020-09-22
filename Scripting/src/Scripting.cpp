#include "Scripting\Scripting.h"

#include <string>
#include <iostream>
#include <cstdlib>

#include <filesystem>


#include "lua.hpp"

bool CheckLua(lua_State *L, int luaResult)
{
	if (luaResult != LUA_OK)
	{
		std::cerr << lua_tostring(L, -1) << std::endl;
		return false;
	}
	return true;
}

void TestMono()
{

	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (CheckLua(L, luaL_dofile(L, "..\\Assets\\Scripts\\Test.lua")))
	{
		lua_getglobal(L, "a");
		if (lua_isnumber(L, -1))
		{
			float a_in_cpp = (float)lua_tonumber(L, -1);
			std::cout << "a_in_cpp = " << a_in_cpp << std::endl;
		}
	}

	system("pause");
	lua_close(L);
	std::cout << "Scripting works!" << std::endl;
}
