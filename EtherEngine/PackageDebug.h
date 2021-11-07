#ifndef _PACKAGE_DEBUG_H_
#define _PACKAGE_DEBUG_H_

#include "Config.h"

#include <lua.hpp>
#include <SDL.h>

ETHER_API int EAPI_Debug_GetCoreError(lua_State* pLuaVM);

#endif // !_PACKAGE_DEBUG_H_
