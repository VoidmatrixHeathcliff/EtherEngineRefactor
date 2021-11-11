#ifndef _INTERACTIVITY_H_
#define _INTERACTIVITY_H_

#include "Config.h"

#include <lua.hpp>
#include <SDL.h>

extern SDL_Event* pGlobalEvent;

ETHER_API int EAPI_Input_UpdateEvent(lua_State* pLuaVM);

ETHER_API int EAPI_Input_SetTextInput(lua_State* pLuaVM);

ETHER_API int EAPI_Input_GetType(lua_State* pLuaVM);

ETHER_API int EAPI_Input_GetKeyCode(lua_State* pLuaVM);

ETHER_API int EAPI_Input_GetCursorPosition(lua_State* pLuaVM);

ETHER_API int EAPI_Input_GetWheelScroll(lua_State* pLuaVM);

ETHER_API int EAPI_Input_GetText(lua_State* pLuaVM);

#endif // !_INTERACTIVITY_H_
