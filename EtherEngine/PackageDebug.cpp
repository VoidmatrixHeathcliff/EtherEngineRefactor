#include "PackageDebug.h"

ETHER_API int EAPI_Debug_GetCoreError(lua_State* pLuaVM)
{
	lua_pushstring(pLuaVM, SDL_GetError());

	return 1;
}