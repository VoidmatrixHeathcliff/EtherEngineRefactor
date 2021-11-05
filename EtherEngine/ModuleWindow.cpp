#include "ModuleWindow.h"


ETHER_API showMessageBox(lua_State* L)
{
	SDL_MessageBoxFlags flag;
	switch ((int)luaL_checknumber(L, 1))
	{
	case MSGBOX_ERROR:
		flag = SDL_MESSAGEBOX_ERROR;
		break;
	case MSGBOX_WARNING:
		flag = SDL_MESSAGEBOX_WARNING;
		break;
	case MSGBOX_INFO:
		flag = SDL_MESSAGEBOX_INFORMATION;
		break;
	default:
		luaL_error(L, "bad argument #1 to 'ShowMessageBox' (MACRO number expected, got %s)", luaL_typename(L, 1));
		break;
	}
	SDL_ShowSimpleMessageBox(flag, luaL_checkstring(L, 2), luaL_checkstring(L, 3), pGlobalWindow);

	return 0;
}


ETHER_API showConfirmMessageBox(lua_State* L)
{
	SDL_MessageBoxFlags flag;
	switch ((int)luaL_checknumber(L, 1))
	{
	case MSGBOX_ERROR:
		flag = SDL_MESSAGEBOX_ERROR;
		break;
	case MSGBOX_WARNING:
		flag = SDL_MESSAGEBOX_WARNING;
		break;
	case MSGBOX_INFO:
		flag = SDL_MESSAGEBOX_INFORMATION;
		break;
	default:
		luaL_error(L, "bad argument #1 to 'ShowMessageBox' (MACRO number expected, got %s)", luaL_typename(L, 1));
		break;
	}

	const SDL_MessageBoxButtonData buttons[2] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, lua_tostring(L, 4) ? lua_tostring(L, 4) : "OK" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, lua_tostring(L, 5) ? lua_tostring(L, 5) : "Cancel" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
		{
			{ 255, 0, 0 },
			{ 0, 255, 0 },
			{ 255, 255, 0 },
			{ 0, 0, 255 },
			{ 255, 0, 255 }
		}
	};
	const SDL_MessageBoxData messageboxdata = {
		flag, pGlobalWindow, luaL_checkstring(L, 2), luaL_checkstring(L, 3), 2, buttons, &colorScheme
	};

	int iButtonID = 0;
	SDL_ShowMessageBox(&messageboxdata, &iButtonID);

	lua_pushboolean(L, iButtonID == 1 ? true : false);

	return 1;
}


ETHER_API showFolderSelector(lua_State* L)
{
#ifdef __WINDOWS__

	WCHAR wszClassName[MAX_PATH] = { 0 };
	try
	{
		MultiByteToWideChar(
			CP_ACP,
			0,
			EncodingConversion::UTF8ToGBK(luaL_checkstring(L, 1)).c_str(),
			strlen(EncodingConversion::UTF8ToGBK(luaL_checkstring(L, 1)).c_str()) + 1,
			wszClassName,
			sizeof(wszClassName) / sizeof(wszClassName[0])
		);
	}
	catch (const std::exception&)
	{
		MultiByteToWideChar(
			CP_ACP,
			0,
			luaL_checkstring(L, 1),
			strlen(luaL_checkstring(L, 1)) + 1,
			wszClassName,
			sizeof(wszClassName) / sizeof(wszClassName[0])
		);
	}

	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 
		GetForegroundWindow(), 
		nullptr, 
		szBuffer, 
		wszClassName, 
		BIF_EDITBOX | BIF_UAHINT
	};
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);

	if (idl)
	{
		SHGetPathFromIDList(idl, szBuffer);
		int iLen = WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, nullptr, 0, nullptr, nullptr);
		char* chRtn = new char[iLen * sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, szBuffer, -1, chRtn, iLen, nullptr, nullptr);
		lua_pushstring(L, chRtn);
		delete[] chRtn;
	}
	else
		lua_pushnil(L);

#else

#endif

	return 1;
}


ETHER_API createWindow(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (pGlobalWindow)
		luaL_error(L, "bad operation to 'CreateWindow' (only one window can be created)");
#endif
	SDL_Rect rect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 2, rect);
#else
	GetRectParam(L, 2, rect);
#endif
	rect.x = rect.x == WINDOW_POSITION_DEFAULT ? SDL_WINDOWPOS_UNDEFINED : rect.x;
	rect.y = rect.y == WINDOW_POSITION_DEFAULT ? SDL_WINDOWPOS_UNDEFINED : rect.y;

	int flags = SDL_WINDOW_SHOWN;
	luaL_argcheck(L, lua_istable(L, 3), 3, "table expected");
	lua_pushnil(L);
	while (lua_next(L, 3))
	{
		lua_pushvalue(L, -2);
		switch ((int)lua_tonumber(L, -2))
		{
		case WINDOW_FULLSCREEN:
			flags |= SDL_WINDOW_FULLSCREEN;
			break;
		case WINDOW_FULLSCREEN_DESKTOP:
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			break;
		case WINDOW_BORDERLESS:
			flags |= SDL_WINDOW_BORDERLESS;
			break;
		case WINDOW_RESIZABLE:
			flags |= SDL_WINDOW_RESIZABLE;
			break;
		case WINDOW_MAXIMIZED:
			flags |= SDL_WINDOW_MAXIMIZED;
			break;
		case WINDOW_MINIMIZED:
			flags |= SDL_WINDOW_MINIMIZED;
			break;
		default:
			luaL_error(L, "bad argument #3 to 'CreateWindow' (the elements of table must be MACRO number, got %s)", luaL_typename(L, -2));
			break;
		}
		lua_pop(L, 2);
	}

	pGlobalWindow = SDL_CreateWindow(luaL_checkstring(L, 1), rect.x, rect.y, rect.w, rect.h, flags);
	pGlobalRenderer = SDL_CreateRenderer(pGlobalWindow, -1, SDL_RENDERER_ACCELERATED);

#ifdef __WINDOWS__
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
#else
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
#endif

	return 0;
}


ETHER_API closeWindow(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "close operation must be done after the window created");
#endif

	SDL_DestroyWindow(pGlobalWindow);
	pGlobalWindow = nullptr;

	return 0;
}


ETHER_API setWindowTitle(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "title operation must be done after the window created");
#endif

	SDL_SetWindowTitle(pGlobalWindow, luaL_checkstring(L, 1));

	return 0;
}


ETHER_API getWindowTitle(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "title operation must be done after the window created");
#endif

	lua_pushstring(L, SDL_GetWindowTitle(pGlobalWindow));
	return 1;
}


ETHER_API setWindowMode(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "mode operation must be done after the window created");
#endif

	switch ((int)luaL_checknumber(L, 1))
	{
	case WINDOW_MODE_WINDOWED:
		SDL_SetWindowFullscreen(pGlobalWindow, 0);
		break;
	case WINDOW_MODE_FULLSCREEN:
		SDL_SetWindowFullscreen(pGlobalWindow, SDL_WINDOW_FULLSCREEN);
		break;
	case WINDOW_MODE_FULLSCREEN_DESKTOP:
		SDL_SetWindowFullscreen(pGlobalWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case WINDOW_MODE_BORDERLESS:
		SDL_SetWindowBordered(pGlobalWindow, SDL_FALSE);
		break;
	case WINDOW_MODE_BORDERED:
		SDL_SetWindowFullscreen(pGlobalWindow, SDL_TRUE);
		break;
	case WINDOW_MODE_RESIZABLE:
		SDL_SetWindowResizable(pGlobalWindow, SDL_TRUE);
		break;
	case WINDOW_MODE_SIZEFIXED:
		SDL_SetWindowResizable(pGlobalWindow, SDL_FALSE);
		break;
	default:
		luaL_error(L, "bad argument #1 to 'SetWindowMode' (number-MACRO expected, got %s)", luaL_typename(L, -1));
		break;
	}

	return 0;
}


ETHER_API setWindowOpacity(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "opacity operation must be done after the window created");
#endif

	SDL_SetWindowOpacity(pGlobalWindow, luaL_checknumber(L, 1));

	return 0;
}


ETHER_API setWindowSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	SDL_SetWindowSize(pGlobalWindow, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API getWindowSize(lua_State* L)
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


ETHER_API getWindowDrawableSize(lua_State* L)
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


ETHER_API setWindowMaxSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	SDL_SetWindowMaximumSize(pGlobalWindow, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API getWindowMaxSize(lua_State* L)
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


ETHER_API setWindowMinSize(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "size operation must be done after the window created");
#endif

	SDL_SetWindowMinimumSize(pGlobalWindow, luaL_checknumber(L, 1), luaL_checknumber(L, 2));

	return 0;
}


ETHER_API getWindowMinSize(lua_State* L)
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


ETHER_API setWindowPosition(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "position operation must be done after the window created");
#endif

	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	GetPointParam(L, 1, point);
#endif
	SDL_SetWindowPosition(pGlobalWindow, point.x, point.y);

	return 0;
}


ETHER_API getWindowPosition(lua_State* L)
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

ETHER_API setWindowIcon(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "icon operation must be done after the window created");
#endif

	SDL_Surface* surface = (SDL_Surface*)(*(void**)luaL_checkudata(L, 1, METANAME_IMAGE));
	SDL_SetWindowIcon(pGlobalWindow, surface);

	return 0;
}


ETHER_API clearWindow(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "clear operation must be done after the window created");
#endif

	SDL_RenderClear(pGlobalRenderer);

	return 0;
}


ETHER_API updateWindow(lua_State* L)
{
#ifdef _ETHER_DEBUG_
	if (!pGlobalWindow)
		luaL_error(L, "update operation must be done after the window created");
#endif

	SDL_RenderPresent(pGlobalRenderer);

	return 0;
}