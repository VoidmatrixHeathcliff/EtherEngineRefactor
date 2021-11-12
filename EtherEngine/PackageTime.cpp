#include "PackageTime.h"

ETHER_API int EAPI_Time_Sleep(lua_State* pLuaVM)
{
	SDL_Delay((Uint32)luaL_checknumber(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Time_GetInitTime(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, SDL_GetTicks());

	return 1;
}

ETHER_API int EAPI_Time_GetPerformanceCounter(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, SDL_GetPerformanceCounter());

	return 1;
}

ETHER_API int EAPI_Time_GetPerformanceFrequency(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, SDL_GetPerformanceFrequency());

	return 1;
}