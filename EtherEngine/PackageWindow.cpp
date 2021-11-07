#include "PackageWindow.h"

static const SDL_MessageBoxColorScheme MsgBoxColorScheme = 
{
	{
		{ 255, 0, 0 },
		{ 0, 255, 0 },
		{ 255, 255, 0 },
		{ 0, 0, 255 },
		{ 255, 0, 255 }
	}
};

ETHER_API int EAPI_Window_GetHandle(lua_State* pLuaVM)
{
	EE_PushUserdata<SDL_Window>(pLuaVM, pGlobalWindow, METANAME_HANDLEWINDOW);

	return 1;
}

ETHER_API int EAPI_Window_MessageBox(lua_State* pLuaVM)
{
	SDL_MessageBoxFlags _flag = SDL_MESSAGEBOX_INFORMATION;
	switch ((int)luaL_checkinteger(pLuaVM, 1))
	{
	case MSGBOX_ERROR:	_flag = SDL_MESSAGEBOX_ERROR; break;
	case MSGBOX_WARNING: _flag = SDL_MESSAGEBOX_WARNING; break;
	case MSGBOX_INFO: _flag = SDL_MESSAGEBOX_INFORMATION; break;
	default:
		luaL_argerror(pLuaVM, 1, ERRMSG_INVALIDENUM);
		break;
	}
	SDL_ShowSimpleMessageBox(
		_flag, 
		luaL_checkstring(pLuaVM, 2),
		luaL_checkstring(pLuaVM, 3),
		pGlobalWindow
	);	

	return 0;
}

ETHER_API int EAPI_Window_ConfirmBox(lua_State* pLuaVM)
{
	SDL_MessageBoxFlags _flag = SDL_MESSAGEBOX_INFORMATION;
	switch ((int)luaL_checkinteger(pLuaVM, 1))
	{
	case MSGBOX_ERROR:	_flag = SDL_MESSAGEBOX_ERROR; break;
	case MSGBOX_WARNING: _flag = SDL_MESSAGEBOX_WARNING; break;
	case MSGBOX_INFO: _flag = SDL_MESSAGEBOX_INFORMATION; break;
	default:
		luaL_argerror(pLuaVM, 1, ERRMSG_INVALIDENUM);
		break;
	}

	SDL_MessageBoxButtonData _btnData[2] = 
	{
		{
			SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, 
			lua_isstring(pLuaVM, 4) ? lua_tostring(pLuaVM, 4) : "OK" 
		},
		{
			SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, 
			lua_isstring(pLuaVM, 5) ? lua_tostring(pLuaVM, 5) : "Cancel"
		},
	};
	SDL_MessageBoxData _msgboxData = 
	{
		_flag, pGlobalWindow,
		luaL_checkstring(pLuaVM, 2),
		luaL_checkstring(pLuaVM, 3),
		2, _btnData, &MsgBoxColorScheme
	};

	int _btnID = 0;
	SDL_ShowMessageBox(&_msgboxData, &_btnID);

	lua_pushboolean(pLuaVM, _btnID == 1);

	return 1;
}

ETHER_API int EAPI_Window_Create(lua_State* pLuaVM)
{
	if (pGlobalWindow) return 0;

	SDL_Rect _rect;
	EE_CheckRect(pLuaVM, 2, _rect);

	int _flags = SDL_WINDOW_SHOWN;
	EE_TraverseTable(
		pLuaVM, 3,
		[&]() -> bool
		{
			switch ((int)lua_tointeger(pLuaVM, -1))
			{
			case WINDOW_FULLSCREEN: _flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; break;
			case WINDOW_RESIZABLE:	_flags |= SDL_WINDOW_RESIZABLE; break;
			case WINDOW_MAXIMIZED:	_flags |= SDL_WINDOW_MAXIMIZED; break;
			case WINDOW_MINIMIZED:	_flags |= SDL_WINDOW_MINIMIZED; break;
			case WINDOW_WINDOWED:	break;
			case WINDOW_FIXED:		break;
			case WINDOW_BORDERED:	break;
			default:				luaL_argerror(pLuaVM, 3, ERRMSG_INVALIDENUM); break;
			}
			return true;
		}
	);

	pGlobalWindow = SDL_CreateWindow(
		luaL_checkstring(pLuaVM, 1), 
		_rect.x, _rect.y,
		_rect.w, _rect.h, 
		_flags
	);
	pGlobalRenderer = SDL_CreateRenderer(
		pGlobalWindow,
		-1, SDL_RENDERER_ACCELERATED
	);

	return 0;
}


ETHER_API int EAPI_Window_Close(lua_State* L)
{
	if (pGlobalRenderer)
	{
		SDL_DestroyRenderer(pGlobalRenderer);
		pGlobalRenderer = nullptr;
	}
	if (pGlobalWindow)
	{
		SDL_DestroyWindow(pGlobalWindow);
		pGlobalWindow = nullptr;
	}

	return 0;
}


ETHER_API int EAPI_Window_SetTitle(lua_State* L)
{
	SDL_SetWindowTitle(pGlobalWindow, luaL_checkstring(L, 1));

	return 0;
}


ETHER_API int getWindowTitle(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "title operation must be done after the window created");
#endif

	lua_pushstring(L, SDL_GetWindowTitle(pGlobalWindow));
	return 1;
}


ETHER_API int setWindowMode(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "mode operation must be done after the window created");
#endif

	switch ((int)luaL_checknumber(L, 1))
	{
	case WINDOW_WINDOWED:
		SDL_SetWindowFullscreen(pGlobalWindow, 0);
		break;
	case WINDOW_FULLSCREEN:
		SDL_SetWindowFullscreen(pGlobalWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case WINDOW_BORDERLESS:
		SDL_SetWindowBordered(pGlobalWindow, SDL_FALSE);
		break;
	case WINDOW_BORDERED:
		SDL_SetWindowFullscreen(pGlobalWindow, SDL_TRUE);
		break;
	case WINDOW_RESIZABLE:
		SDL_SetWindowResizable(pGlobalWindow, SDL_TRUE);
		break;
	case WINDOW_FIXED:
		SDL_SetWindowResizable(pGlobalWindow, SDL_FALSE);
		break;
	default:
		luaL_error(L, "bad argument #1 to 'SetWindowMode' (number-MACRO expected, got %s)", luaL_typename(L, -1));
		break;
	}

	return 0;
}


ETHER_API int setWindowOpacity(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "opacity operation must be done after the window created");
#endif

	SDL_SetWindowOpacity(pGlobalWindow, luaL_checknumber(L, 1));

	return 0;
}


ETHER_API int setWindowSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	SDL_SetWindowSize(pGlobalWindow, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int getWindowSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	int width = 0, height = 0;
	SDL_GetWindowSize(pGlobalWindow, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API int getWindowDrawableSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	int width = 0, height = 0;
	SDL_GetRendererOutputSize(pGlobalRenderer, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API int setWindowMaxSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	SDL_SetWindowMaximumSize(pGlobalWindow, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int getWindowMaxSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	int width = 0, height = 0;
	SDL_GetWindowMaximumSize(pGlobalWindow, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API int setWindowMinSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	SDL_SetWindowMinimumSize(pGlobalWindow, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int getWindowMinSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	int width = 0, height = 0;
	SDL_GetWindowMinimumSize(pGlobalWindow, &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API int setWindowPosition(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "position operation must be done after the window created");
#endif

	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_SetWindowPosition(pGlobalWindow, point.x, point.y);

	return 0;
}


ETHER_API int getWindowPosition(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "position operation must be done after the window created");
#endif

	SDL_Point point;
	SDL_GetWindowPosition(pGlobalWindow, &point.x, &point.y);
	lua_newtable(L);
	lua_pushstring(L, "x");
	lua_pushnumber(L, point.x);
	lua_settable(L, -3);
	lua_pushstring(L, "y");
	lua_pushnumber(L, point.y);
	lua_settable(L, -3);

	return 1;
}

ETHER_API int setWindowIcon(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "icon operation must be done after the window created");
#endif

	SDL_Surface* surface = (SDL_Surface*)(*(void**)luaL_checkudata(L, 1, METANAME_IMAGE));
	SDL_SetWindowIcon(pGlobalWindow, surface);

	return 0;
}


ETHER_API int clearWindow(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "clear operation must be done after the window created");
#endif

	SDL_RenderClear(pGlobalRenderer);

	return 0;
}


ETHER_API int updateWindow(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "update operation must be done after the window created");
#endif

	SDL_RenderPresent(pGlobalRenderer);

	return 0;
}