#include "PackageGraphic.h"

ETHER_API int EAPI_Graphic_SetCursorShown(lua_State* pLuaVM)
{
	SDL_ShowCursor(lua_toboolean(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Graphic_LoadImageFromFile(lua_State* pLuaVM)
{
	SDL_Surface* _pSurface = nullptr;

	if (_pSurface = IMG_Load(luaL_checkstring(pLuaVM, 1)))
		EE_PushUserdata<SDL_Surface>(pLuaVM, _pSurface, METANAME_IMAGE);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

ETHER_API int EAPI_Graphic_LoadImageFromData(lua_State* pLuaVM)
{
	size_t _szData = 0;
	const char* _pData = luaL_checklstring(pLuaVM, 1, &_szData);
	SDL_Surface* _pSurface = nullptr;
	if (_pSurface = IMG_Load_RW(SDL_RWFromMem((void*)_pData, (int)_szData), 1))
		EE_PushUserdata<SDL_Surface>(pLuaVM, _pSurface, METANAME_IMAGE);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

ETHER_API int EAPI_Graphic_Image_SetColorKey(lua_State* pLuaVM)
{
	SDL_Surface* _pSurface = EE_ToUserdata<SDL_Surface>(pLuaVM, 1, METANAME_IMAGE);

	SDL_Color _color;
	EE_CheckColor(pLuaVM, 2, _color);

	SDL_SetColorKey(_pSurface, lua_toboolean(pLuaVM, 3),
		SDL_MapRGBA(_pSurface->format, _color.r, _color.g, _color.b, _color.a));

	return 0;
}

ETHER_API int EAPI_Graphic_Image_Texture(lua_State* pLuaVM)
{
	SDL_Surface* _pSurface = EE_ToUserdata<SDL_Surface>(pLuaVM, 1, METANAME_IMAGE);

	SDL_Texture* _pTexture = nullptr;
	if (_pTexture = SDL_CreateTextureFromSurface(pGlobalRenderer, _pSurface))
		EE_PushUserdata<SDL_Texture>(pLuaVM, _pTexture, METANAME_TEXTURE);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

ETHER_API int EAPI_Graphic_Image_size(lua_State* pLuaVM)
{
	SDL_Surface* _pSurface = EE_ToUserdata<SDL_Surface>(pLuaVM, 1, METANAME_IMAGE);

	lua_pushnumber(pLuaVM, _pSurface->w);
	lua_pushnumber(pLuaVM, _pSurface->h);

	return 2;
}

ETHER_API int EAPI_Graphic_Image_GC(lua_State* pLuaVM)
{
	SDL_Surface* _pSurface = EE_ToUserdata<SDL_Surface>(pLuaVM, 1, METANAME_IMAGE);

	SDL_FreeSurface(_pSurface); _pSurface = nullptr;

	return 0;
}

ETHER_API int EAPI_Graphic_Texture_SetAlpha(lua_State* pLuaVM)
{
	SDL_Texture* _pTexture = EE_ToUserdata<SDL_Texture>(pLuaVM, 1, METANAME_TEXTURE);

	SDL_SetTextureBlendMode(_pTexture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(_pTexture, (Uint8)luaL_checknumber(pLuaVM, 2));

	return 0;
}

ETHER_API int EAPI_Graphic_Texture_size(lua_State* pLuaVM)
{
	SDL_Texture* _pTexture = EE_ToUserdata<SDL_Texture>(pLuaVM, 1, METANAME_TEXTURE);

	int _width = 0, _height = 0;
	SDL_QueryTexture(_pTexture, nullptr, nullptr, &_width, &_height);

	lua_pushinteger(pLuaVM, _width);
	lua_pushinteger(pLuaVM, _height);

	return 2;
}

ETHER_API int EAPI_Graphic_Texture_GC(lua_State* pLuaVM)
{
	SDL_Texture* _pTexture = EE_ToUserdata<SDL_Texture>(pLuaVM, 1, METANAME_TEXTURE);

	SDL_DestroyTexture(_pTexture); _pTexture = nullptr;

	return 0;
}

ETHER_API int EAPI_Graphic_RenderTexture(lua_State* pLuaVM)
{
	SDL_Texture* _pTexture = EE_ToUserdata<SDL_Texture>(pLuaVM, 1, METANAME_TEXTURE);
	SDL_Rect _rect_dst; EE_CheckRect(pLuaVM, 2, _rect_dst);

	if (lua_gettop(pLuaVM) < 3)
		SDL_RenderCopy(pGlobalRenderer, _pTexture, nullptr, &_rect_dst);
	else
	{
		SDL_Rect _rect_src; EE_CheckRect(pLuaVM, 3, _rect_src);
		SDL_RenderCopy(pGlobalRenderer, _pTexture, &_rect_src, &_rect_dst);
	}

	return 0;
}

ETHER_API int EAPI_Graphic_RenderTextureEx(lua_State* pLuaVM)
{
	SDL_Texture* _pTexture = EE_ToUserdata<SDL_Texture>(pLuaVM, 1, METANAME_TEXTURE);
	SDL_Rect _rect_dst; EE_CheckRect(pLuaVM, 2, _rect_dst);

	if (lua_gettop(pLuaVM) < 3)
		SDL_RenderCopy(pGlobalRenderer, _pTexture, nullptr, &_rect_dst);
	else if (lua_gettop(pLuaVM) < 4)
	{
		SDL_Rect _rect_src; EE_CheckRect(pLuaVM, 3, _rect_src);
		SDL_RenderCopy(pGlobalRenderer, _pTexture, &_rect_src, &_rect_dst);
	}
	else if (lua_gettop(pLuaVM) < 6)
	{
		SDL_Rect _rect_src; EE_CheckRect(pLuaVM, 3, _rect_src);
		SDL_Point _point_center; EE_CheckPoint(pLuaVM, 4, _point_center);
		SDL_RenderCopyEx(pGlobalRenderer, _pTexture, &_rect_src, &_rect_dst,
			luaL_checknumber(pLuaVM, 5), &_point_center, SDL_FLIP_NONE);
	}
	else
	{
		SDL_Rect _rect_src; EE_CheckRect(pLuaVM, 3, _rect_src);
		SDL_Point _point_center; EE_CheckPoint(pLuaVM, 4, _point_center);
		luaL_argexpected(pLuaVM, lua_istable(pLuaVM, 6), 6, LUA_TABLIBNAME);
		SDL_RendererFlip _flags = SDL_FLIP_NONE; EE_TraverseTable(
			pLuaVM, 6,
			[&]() -> bool
			{
				switch ((int)lua_tointeger(pLuaVM, -1))
				{
				case FLIP_HORIZONTAL:	_flags = (SDL_RendererFlip)(_flags | SDL_FLIP_HORIZONTAL); break;
				case FLIP_VERTICAL:		_flags = (SDL_RendererFlip)(_flags | SDL_FLIP_VERTICAL); break;
				case FLIP_NONE:			_flags = (SDL_RendererFlip)(_flags | SDL_FLIP_NONE); break;
				default:				luaL_argerror(pLuaVM, 6, ERRMSG_INVALIDENUM); break;
				}
				return true;
			}
		);
		SDL_RenderCopyEx(pGlobalRenderer, _pTexture, &_rect_src, &_rect_dst,
			luaL_checknumber(pLuaVM, 5), &_point_center, _flags);
	}

	return 0;
}

ETHER_API int EAPI_Graphic_SetDrawColor(lua_State* pLuaVM)
{
	SDL_Color _color; EE_CheckColor(pLuaVM, 1, _color);
	
	if (_color.a != 255) SDL_SetRenderDrawBlendMode(pGlobalRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pGlobalRenderer, _color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_GetDrawColor(lua_State* pLuaVM)
{
	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));

	lua_createtable(pLuaVM, 0, 4);
	lua_pushstring(pLuaVM, "r"); lua_pushnumber(pLuaVM, _color.r); lua_rawset(pLuaVM, -3);
	lua_pushstring(pLuaVM, "g"); lua_pushnumber(pLuaVM, _color.r); lua_rawset(pLuaVM, -3);
	lua_pushstring(pLuaVM, "b"); lua_pushnumber(pLuaVM, _color.r); lua_rawset(pLuaVM, -3);
	lua_pushstring(pLuaVM, "a"); lua_pushnumber(pLuaVM, _color.r); lua_rawset(pLuaVM, -3);

	return 1;
}

ETHER_API int EAPI_Graphic_DrawPoint(lua_State* pLuaVM)
{
	SDL_Point _point; EE_CheckPoint(pLuaVM, 1, _point);

	SDL_RenderDrawPoint(pGlobalRenderer, _point.x, _point.y);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawLine(lua_State* pLuaVM)
{
	SDL_Point _point_1, _point_2;
	EE_CheckPoint(pLuaVM, 1, _point_1);
	EE_CheckPoint(pLuaVM, 2, _point_2);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (lua_gettop(pLuaVM) < 3)
		aalineRGBA(pGlobalRenderer, _point_1.x, _point_1.y, _point_2.x, _point_2.y,
			_color.r, _color.g, _color.b, _color.a);
	else
		thickLineRGBA(pGlobalRenderer, _point_1.x, _point_1.y, _point_2.x, _point_2.y,
			(Uint8)luaL_checknumber(pLuaVM, 3), _color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawRectangle(lua_State* pLuaVM)
{
	SDL_Rect _rect; EE_CheckRect(pLuaVM, 1, _rect);

	if (!lua_toboolean(pLuaVM, 2))
		SDL_RenderDrawRect(pGlobalRenderer, &_rect);
	else
		SDL_RenderFillRect(pGlobalRenderer, &_rect);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawRoundRectangle(lua_State* pLuaVM)
{
	SDL_Rect _rect; EE_CheckRect(pLuaVM, 1, _rect);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (!lua_toboolean(pLuaVM, 3))
		roundedRectangleRGBA(pGlobalRenderer, _rect.x, _rect.y, _rect.x + _rect.w, _rect.y + _rect.h, 
			(Sint16)luaL_checknumber(pLuaVM, 2), _color.r, _color.g, _color.b, _color.a);
	else
		roundedBoxRGBA(pGlobalRenderer, _rect.x, _rect.y, _rect.x + _rect.w, _rect.y + _rect.h,
			(Sint16)luaL_checknumber(pLuaVM, 2), _color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawCircle(lua_State* pLuaVM)
{
	SDL_Point _point; EE_CheckPoint(pLuaVM, 1, _point);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (!lua_toboolean(pLuaVM, 3))
		aacircleRGBA(pGlobalRenderer, _point.x, _point.y, (Sint16)luaL_checknumber(pLuaVM, 2),
			_color.r, _color.g, _color.b, _color.a);
	else
		filledCircleRGBA(pGlobalRenderer, _point.x, _point.y, (Sint16)luaL_checknumber(pLuaVM, 2),
			_color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawEllipse(lua_State* pLuaVM)
{
	SDL_Point _point; EE_CheckPoint(pLuaVM, 1, _point);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (!lua_toboolean(pLuaVM, 4))
		aaellipseRGBA(pGlobalRenderer, _point.x, _point.y, (Sint16)luaL_checknumber(pLuaVM, 2),
			(Sint16)luaL_checknumber(pLuaVM, 3), _color.r, _color.g, _color.b, _color.a);
	else
		filledEllipseRGBA(pGlobalRenderer, _point.x, _point.y, (Sint16)luaL_checknumber(pLuaVM, 2),
			(Sint16)luaL_checknumber(pLuaVM, 3), _color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawPie(lua_State* pLuaVM)
{
	SDL_Point _point; EE_CheckPoint(pLuaVM, 1, _point);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (!lua_toboolean(pLuaVM, 5))
		pieRGBA(pGlobalRenderer, _point.x, _point.y, (Sint16)luaL_checknumber(pLuaVM, 2), 
			(Sint16)luaL_checknumber(pLuaVM, 3), (Sint16)luaL_checknumber(pLuaVM, 4), _color.r, _color.g, _color.b, _color.a);
	else
		filledPieRGBA(pGlobalRenderer, _point.x, _point.y, (Sint16)luaL_checknumber(pLuaVM, 2),
			(Sint16)luaL_checknumber(pLuaVM, 3), (Sint16)luaL_checknumber(pLuaVM, 4), _color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawTriangle(lua_State* pLuaVM)
{
	SDL_Point _point_1, _point_2, _point_3;
	EE_CheckPoint(pLuaVM, 1, _point_1); EE_CheckPoint(pLuaVM, 2, _point_2); EE_CheckPoint(pLuaVM, 3, _point_3);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (!lua_toboolean(pLuaVM, 4))
		aatrigonRGBA(pGlobalRenderer, _point_1.x, _point_1.y, _point_2.x, _point_2.y, _point_3.x, _point_3.y, 
			_color.r, _color.g, _color.b, _color.a);
	else
		filledTrigonRGBA(pGlobalRenderer, _point_1.x, _point_1.y, _point_2.x, _point_2.y, _point_3.x, _point_3.y,
			_color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawPolygon(lua_State* pLuaVM)
{
	std::vector<Sint16> _vecx, _vecy;
	luaL_argexpected(pLuaVM, lua_istable(pLuaVM, 1), 1, LUA_TABLIBNAME);
	EE_TraverseTable(
		pLuaVM, 1,
		[&]() -> bool
		{
			SDL_Point _point;

			luaL_argcheck(pLuaVM, lua_istable(pLuaVM, -1), 1, "elements expected table");

			lua_pushstring(pLuaVM, "x"); lua_rawget(pLuaVM, -1);
			luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1), 1, std::string("point at #")
				.append(std::to_string(lua_tointeger(pLuaVM, -2))).append(" ").append(ERRMSG_INVALIDMEMBER).append(":x").c_str());
			_point.x = (int)lua_tointeger(pLuaVM, -1);
			lua_pop(pLuaVM, 1);

			lua_pushstring(pLuaVM, "y"); lua_rawget(pLuaVM, -1);
			luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1), 1, std::string("point at #")
				.append(std::to_string(lua_tointeger(pLuaVM, -2))).append(" ").append(ERRMSG_INVALIDMEMBER).append(":y").c_str());
			_point.y = (int)lua_tointeger(pLuaVM, -1);
			lua_pop(pLuaVM, 1);
			
			_vecx.push_back(_point.x); _vecy.push_back(_point.y);

			return true;
		}
	);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	if (!lua_toboolean(pLuaVM, 2))
		aapolygonRGBA(pGlobalRenderer, &_vecx[0], &_vecy[0], (int)_vecx.size(),
			_color.r, _color.g, _color.b, _color.a);
	else
		filledPolygonRGBA(pGlobalRenderer, &_vecx[0], &_vecy[0], (int)_vecx.size(),
			_color.r, _color.g, _color.b, _color.a);

	return 0;
}

ETHER_API int EAPI_Graphic_DrawBezier(lua_State* pLuaVM)
{
	std::vector<Sint16> _vecx, _vecy;
	luaL_argexpected(pLuaVM, lua_istable(pLuaVM, 1), 1, LUA_TABLIBNAME);
	EE_TraverseTable(
		pLuaVM, 1,
		[&]() -> bool
		{
			SDL_Point _point;

			luaL_argcheck(pLuaVM, lua_istable(pLuaVM, -1), 1, "elements expected table");

			lua_pushstring(pLuaVM, "x"); lua_rawget(pLuaVM, -1);
			luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1), 1, std::string("point at #")
				.append(std::to_string(lua_tointeger(pLuaVM, -2))).append(" ").append(ERRMSG_INVALIDMEMBER).append(":x").c_str());
			_point.x = (int)lua_tointeger(pLuaVM, -1);
			lua_pop(pLuaVM, 1);

			lua_pushstring(pLuaVM, "y"); lua_rawget(pLuaVM, -1);
			luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1), 1, std::string("point at #")
				.append(std::to_string(lua_tointeger(pLuaVM, -2))).append(" ").append(ERRMSG_INVALIDMEMBER).append(":y").c_str());
			_point.y = (int)lua_tointeger(pLuaVM, -1);
			lua_pop(pLuaVM, 1);

			_vecx.push_back(_point.x); _vecy.push_back(_point.y);

			return true;
		}
	);

	SDL_Color _color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(_color.r), &(_color.g), &(_color.b), &(_color.a));
	bezierRGBA(pGlobalRenderer, &_vecx[0], &_vecy[0], (int)_vecx.size(), (int)lua_tonumber(pLuaVM, 2),
		_color.r, _color.g, _color.b, _color.a);

	return 0;
}







ETHER_API int loadFontFromFile(lua_State* pLuaVM)
{
	TTF_Font* pFont = TTF_OpenFont(luaL_checkstring(L, 1), luaL_checknumber(L, 2));
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pFont, 1, "load font failed");
#endif
	TTF_Font** uppFont = (TTF_Font**)lua_newuserdata(L, sizeof(TTF_Font*));
	*uppFont = pFont;
	luaL_getmetatable(L, METANAME_FONT);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int loadFontFromData(lua_State* pLuaVM)
{
	size_t size = 0;
	TTF_Font* pFont = TTF_OpenFontRW(SDL_RWFromMem((void*)luaL_checklstring(L, 1, &size), size), 1, luaL_checknumber(L, 2));
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pFont, 1, "load font failed");
#endif
	TTF_Font** uppFont = (TTF_Font**)lua_newuserdata(L, sizeof(TTF_Font*));
	*uppFont = pFont;
	luaL_getmetatable(L, METANAME_FONT);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int __gc_Font(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	TTF_CloseFont(font);
	font = nullptr;

	return 0;
}


ETHER_API int font_GetStyle(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_newtable(L);
	int style = TTF_GetFontStyle(font);
	if (style == TTF_STYLE_NORMAL)
	{
		lua_pushnumber(L, 1);
		lua_pushnumber(L, FONT_STYLE_NORMAL);
		lua_settable(L, -3);
	}
	else
	{
		int index = 1;
		if (style & TTF_STYLE_BOLD)
		{
			lua_pushnumber(L, index);
			lua_pushnumber(L, FONT_STYLE_BOLD);
			lua_settable(L, -3);
			index++;
		}
		if (style & TTF_STYLE_ITALIC)
		{
			lua_pushnumber(L, index);
			lua_pushnumber(L, FONT_STYLE_ITALIC);
			lua_settable(L, -3);
			index++;
		}
		if (style & TTF_STYLE_UNDERLINE)
		{
			lua_pushnumber(L, index);
			lua_pushnumber(L, FONT_STYLE_UNDERLINE);
			lua_settable(L, -3);
			index++;
		}
		if (style & TTF_STYLE_STRIKETHROUGH)
		{
			lua_pushnumber(L, index);
			lua_pushnumber(L, FONT_STYLE_STRIKETHROUGH);
			lua_settable(L, -3);
			index++;
		}
	}

	return 1;
}


ETHER_API int font_SetStyle(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
	CheckTableParam(L, 2);
#endif
	int style = 0;
	lua_pushnil(L);
	while (lua_next(L, 2))
	{
		lua_pushvalue(L, -2);
		if (!lua_isnumber(L, -2))
			luaL_error(L, "bad argument #2 to 'SetFontStyle' (table elements must be MACRO number, got %s)", luaL_typename(L, -2));
		else
		{
			switch ((int)lua_tonumber(L, -2))
			{
			case FONT_STYLE_BOLD:
				style |= TTF_STYLE_BOLD;
				break;
			case FONT_STYLE_ITALIC:
				style |= TTF_STYLE_ITALIC;
				break;
			case FONT_STYLE_UNDERLINE:
				style |= TTF_STYLE_UNDERLINE;
				break;
			case FONT_STYLE_STRIKETHROUGH:
				style |= TTF_STYLE_STRIKETHROUGH;
				break;
			case FONT_STYLE_NORMAL:
				style |= TTF_STYLE_NORMAL;
				break;
			default:
				luaL_error(L, "bad argument #2 to 'SetFontStyle' (table elements must be MACRO number, got %s)", luaL_typename(L, -2));
				break;
			}
		}
		lua_pop(L, 2);
	}

	TTF_SetFontStyle(font, style);

	return 0;
}


ETHER_API int font_GetOutlineWidth(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_pushnumber(L, TTF_GetFontOutline(font));

	return 1;
}


ETHER_API int font_SetOutlineWidth(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	TTF_SetFontOutline(font, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int font_GetKerning(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_pushnumber(L, TTF_GetFontKerning(font));

	return 1;
}


ETHER_API int font_SetKerning(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	TTF_SetFontKerning(font, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int font_GetHeight(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_pushnumber(L, TTF_FontHeight(font));

	return 1;
}


ETHER_API int getTextSize(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	int width, height;
	TTF_SizeText(font, luaL_checkstring(L, 2), &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API int getUTF8TextSize(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	int width, height;
	TTF_SizeUTF8(font, luaL_checkstring(L, 2), &width, &height);
	lua_pushnumber(L, width);
	lua_pushnumber(L, height);

	return 2;
}


ETHER_API int createTextImageSolid(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	SDL_Color color;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, color);
#else
	EE_CheckColor(L, 3, color);
#endif
	SDL_Surface* pSurface = TTF_RenderText_Solid(font, luaL_checkstring(L, 2), color);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "create text image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int createUTF8TextImageSolid(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	SDL_Color color;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, color);
#else
	EE_CheckColor(L, 3, color);
#endif
	SDL_Surface* pSurface = TTF_RenderUTF8_Solid(font, luaL_checkstring(L, 2), color);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "create text image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int createTextImageShaded(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	SDL_Color fgColor, bgColor;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, fgColor);
	CheckColorParam(L, 4, bgColor);
#else
	EE_CheckColor(L, 3, fgColor);
	EE_CheckColor(L, 4, bgColor);
#endif
	SDL_Surface* pSurface = TTF_RenderText_Shaded(font, luaL_checkstring(L, 2), fgColor, bgColor);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "create text image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int createUTF8TextImageShaded(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	SDL_Color fgColor, bgColor;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, fgColor);
	CheckColorParam(L, 4, bgColor);
#else
	EE_CheckColor(L, 3, fgColor);
	EE_CheckColor(L, 4, bgColor);
#endif
	SDL_Surface* pSurface = TTF_RenderUTF8_Shaded(font, luaL_checkstring(L, 2), fgColor, bgColor);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "create text image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int createTextImageBlended(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	SDL_Color color;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, color);
#else
	EE_CheckColor(L, 3, color);
#endif
	SDL_Surface* pSurface = TTF_RenderText_Blended(font, luaL_checkstring(L, 2), color);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "create text image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int createUTF8TextImageBlended(lua_State* pLuaVM)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	SDL_Color color;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, color);
#else
	EE_CheckColor(L, 3, color);
#endif
	SDL_Surface* pSurface = TTF_RenderUTF8_Blended(font, luaL_checkstring(L, 2), color);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "create text image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}