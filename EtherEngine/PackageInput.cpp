#include "PackageInput.h"

ETHER_API int EAPI_Input_UpdateEvent(lua_State* pLuaVM)
{
	lua_pushboolean(pLuaVM, SDL_PollEvent(pGlobalEvent));

	return 1;
}

ETHER_API int EAPI_Input_SetTextInput(lua_State* pLuaVM)
{
	lua_toboolean(pLuaVM, 1) ? SDL_StartTextInput() : SDL_StopTextInput();

	return 0;
}

ETHER_API int EAPI_Input_GetType(lua_State* pLuaVM)
{
	if (pGlobalEvent->type != SDL_WINDOWEVENT)
		lua_pushinteger(pLuaVM, pGlobalEvent->type);
	else
		lua_pushinteger(pLuaVM, pGlobalEvent->window.event);

	return 1;
}

ETHER_API int EAPI_Input_GetKeyCode(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, pGlobalEvent->key.keysym.sym);

	return 1;
}

ETHER_API int EAPI_Input_GetCursorPosition(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, pGlobalEvent->motion.x);
	lua_pushinteger(pLuaVM, pGlobalEvent->motion.y);

	return 2;
}

ETHER_API int EAPI_Input_GetWheelScroll(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, pGlobalEvent->wheel.x);
	lua_pushinteger(pLuaVM, pGlobalEvent->wheel.y);

	return 2;
}

ETHER_API int EAPI_Input_GetText(lua_State* pLuaVM)
{
	lua_pushstring(pLuaVM, pGlobalEvent->text.text);

	return 1;
}