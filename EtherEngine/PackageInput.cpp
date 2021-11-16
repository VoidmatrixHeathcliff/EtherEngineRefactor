#include "PackageInput.h"

ETHER_API int EAPI_Input_UpdateEvent(lua_State* pLuaVM)
{
	lua_pushboolean(pLuaVM, SDL_PollEvent(pGlobalEvent));

	return 1;
}

ETHER_API int EAPI_Input_StartTextInput(lua_State* pLuaVM)
{
	SDL_StartTextInput();

	return 0;
}

ETHER_API int EAPI_Input_StopTextInput(lua_State* pLuaVM)
{
	SDL_StopTextInput();

	return 0;
}

ETHER_API int EAPI_Input_GetEventType(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, pGlobalEvent->type);

	return 1;
}

ETHER_API int EAPI_Input_GetWindowEventType(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, pGlobalEvent->window.event);		

	return 1;
}

ETHER_API int EAPI_Input_GetMouseButtonID(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, pGlobalEvent->button.button);

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

ETHER_API int EAPI_Input_GetDropFile(lua_State* pLuaVM)
{
	lua_pushstring(pLuaVM, pGlobalEvent->drop.file);

	return 1;
}