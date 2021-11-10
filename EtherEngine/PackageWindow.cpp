#include "PackageWindow.h"

ETHER_API int EAPI_Window_GetWindowHandle(lua_State* pLuaVM)
{
	EE_PushUserdata<SDL_Window>(pLuaVM, pGlobalWindow, METANAME_HANDLEWINDOW);

	return 1;
}

ETHER_API int EAPI_Window_GetRendererHandle(lua_State* pLuaVM)
{
	EE_PushUserdata<SDL_Renderer>(pLuaVM, pGlobalRenderer, METANAME_HANDLERENDERER);

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
	default: luaL_argerror(pLuaVM, 1, ERRMSG_INVALIDENUM); break;
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
	default: luaL_argerror(pLuaVM, 1, ERRMSG_INVALIDENUM); break;
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
		(Uint32)_flag, pGlobalWindow,
		luaL_checkstring(pLuaVM, 2),
		luaL_checkstring(pLuaVM, 3),
		2, _btnData, Window_pMsgBoxColorScheme
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
	if (lua_gettop(pLuaVM) > 2)
	{
		luaL_argexpected(pLuaVM, lua_istable(pLuaVM, 3), 3, LUA_TABLIBNAME);
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
				case WINDOW_BORDERLESS:	_flags |= SDL_WINDOW_BORDERLESS; break;
				case WINDOW_WINDOWED:	break;
				case WINDOW_FIXED:		break;
				case WINDOW_BORDERED:	break;
				default:				luaL_argerror(pLuaVM, 3, ERRMSG_INVALIDENUM); break;
				}
				return true;
			}
		);
	}

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

ETHER_API int EAPI_Window_Close(lua_State* pLuaVM)
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

ETHER_API int EAPI_Window_SetTitle(lua_State* pLuaVM)
{
	SDL_SetWindowTitle(pGlobalWindow, luaL_checkstring(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Window_GetTitle(lua_State* pLuaVM)
{
	lua_pushstring(pLuaVM, SDL_GetWindowTitle(pGlobalWindow));

	return 1;
}

ETHER_API int EAPI_Window_SetStyle(lua_State* pLuaVM)
{
	switch ((int)luaL_checkinteger(pLuaVM, 1))
	{
	case WINDOW_WINDOWED:	SDL_SetWindowFullscreen(pGlobalWindow, 0); break;
	case WINDOW_FULLSCREEN: SDL_SetWindowFullscreen(pGlobalWindow, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
	case WINDOW_BORDERLESS: SDL_SetWindowBordered(pGlobalWindow, SDL_FALSE); break;
	case WINDOW_BORDERED:	SDL_SetWindowFullscreen(pGlobalWindow, SDL_TRUE); break;
	case WINDOW_RESIZABLE:	SDL_SetWindowResizable(pGlobalWindow, SDL_TRUE); break;
	case WINDOW_MAXIMIZED:	SDL_MaximizeWindow(pGlobalWindow); break;
	case WINDOW_MINIMIZED:	SDL_MinimizeWindow(pGlobalWindow); break;
	case WINDOW_FIXED:		SDL_SetWindowResizable(pGlobalWindow, SDL_FALSE); break;
	default:				luaL_argerror(pLuaVM, 1, ERRMSG_INVALIDENUM); break;
	}

	return 0;
}

ETHER_API int EAPI_Window_SetOpacity(lua_State* pLuaVM)
{
	SDL_SetWindowOpacity(pGlobalWindow, (float)luaL_checknumber(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Window_SetSize(lua_State* pLuaVM)
{
	SDL_SetWindowSize(pGlobalWindow, (int)luaL_checknumber(pLuaVM, 1), 
		(int)luaL_checknumber(pLuaVM, 2));

	return 0;
}

ETHER_API int EAPI_Window_GetSize(lua_State* pLuaVM)
{
	int _width = 0, _height = 0;
	SDL_GetWindowSize(pGlobalWindow, &_width, &_height);
	lua_pushinteger(pLuaVM, _width); lua_pushinteger(pLuaVM, _height);

	return 2;
}

ETHER_API int EAPI_Window_GetDrawableSize(lua_State* pLuaVM)
{
	int _width = 0, _height = 0;
	SDL_GetRendererOutputSize(pGlobalRenderer, &_width, &_height);
	lua_pushinteger(pLuaVM, _width); lua_pushinteger(pLuaVM, _height);

	return 2;
}

ETHER_API int EAPI_Window_SetMaxSize(lua_State* pLuaVM)
{
	SDL_SetWindowMaximumSize(pGlobalWindow, (int)luaL_checknumber(pLuaVM, 1), 
		(int)luaL_checknumber(pLuaVM, 2));

	return 0;
}

ETHER_API int EAPI_Window_GetMaxSize(lua_State* pLuaVM)
{
	int _width = 0, _height = 0;
	SDL_GetWindowMaximumSize(pGlobalWindow, &_width, &_height);
	lua_pushnumber(pLuaVM, _width); lua_pushnumber(pLuaVM, _height);

	return 2;
}

ETHER_API int EAPI_Window_SetMinSize(lua_State* pLuaVM)
{
	SDL_SetWindowMinimumSize(pGlobalWindow, (int)luaL_checknumber(pLuaVM, 1),
		(int)luaL_checknumber(pLuaVM, 2));

	return 0;
}

ETHER_API int EAPI_Window_GetMinSize(lua_State* pLuaVM)
{
	int _width = 0, _height = 0;
	SDL_GetWindowMinimumSize(pGlobalWindow, &_width, &_height);
	lua_pushnumber(pLuaVM, _width); lua_pushnumber(pLuaVM, _height);

	return 2;
}

ETHER_API int EAPI_Window_SetPosition(lua_State* pLuaVM)
{
	SDL_SetWindowPosition(pGlobalWindow, (int)luaL_checknumber(pLuaVM, 1), 
		(int)luaL_checknumber(pLuaVM, 2));

	return 0;
}


ETHER_API int EAPI_Window_GetPosition(lua_State* pLuaVM)
{
	int _x = 0, _y = 0;
	SDL_GetWindowPosition(pGlobalWindow, &_x, &_y);
	lua_pushnumber(pLuaVM, _x); lua_pushnumber(pLuaVM, _y);

	return 2;
}

ETHER_API int EAPI_Window_SetIcon(lua_State* pLuaVM)
{
	SDL_Surface* _surface = EE_ToUserdata<SDL_Surface>(pLuaVM, 1, METANAME_IMAGE);

	SDL_SetWindowIcon(pGlobalWindow, _surface);

	return 0;
}


ETHER_API int EAPI_Window_Clear(lua_State* pLuaVM)
{
	SDL_RenderClear(pGlobalRenderer);

	return 0;
}


ETHER_API int EAPI_Window_Update(lua_State* pLuaVM)
{
	SDL_RenderPresent(pGlobalRenderer);

	return 0;
}