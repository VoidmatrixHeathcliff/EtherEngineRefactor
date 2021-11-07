#include "PackageGraphic.h"


ETHER_API int setCursorShow(lua_State * L)
{
	SDL_ShowCursor(lua_toboolean(L, 1) ? SDL_ENABLE : SDL_DISABLE);

	return 0;
}

ETHER_API int loadImageFromFile(lua_State * L)
{
	SDL_Surface* pSurface = IMG_Load(luaL_checkstring(L, 1));
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "load image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int loadImageFromData(lua_State* L)
{
	size_t size = 0;
	SDL_Surface* pSurface = IMG_Load_RW(SDL_RWFromMem((void*)luaL_checklstring(L, 1, &size), size), 1);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pSurface, 1, "load image failed");
#endif
	SDL_Surface** uppSurface = (SDL_Surface**)lua_newuserdata(L, sizeof(SDL_Surface*));
	*uppSurface = pSurface;
	luaL_getmetatable(L, METANAME_IMAGE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int image_SetColorKey(lua_State * L)
{
	SDL_Surface* surface = GetImageDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckImageDataAt1stPos(surface);
#endif
	SDL_Color color;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 3, color);
#else
	EE_CheckColor(L, 3, color);
#endif
	SDL_SetColorKey(surface, lua_toboolean(L, 2), SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a));

	return 0;
}


ETHER_API int __gc_Image(lua_State * L)
{
	SDL_Surface* surface = GetImageDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckImageDataAt1stPos(surface);
#endif
	SDL_FreeSurface(surface);
	surface = nullptr;

	return 0;
}


ETHER_API int createTexture(lua_State * L)
{
	SDL_Surface* surface = GetImageDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckImageDataAt1stPos(surface);
	if (!pGlobalRenderer)
		luaL_error(L, "Texture creation must be done after the window created");
#endif
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pGlobalRenderer, surface);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, pTexture, 1, "create texture failed");
#endif
	SDL_Texture** uppTexture = (SDL_Texture**)lua_newuserdata(L, sizeof(SDL_Texture*));
	*uppTexture = pTexture;
	luaL_getmetatable(L, METANAME_TEXTURE);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int __gc_Texture(lua_State * L)
{
	SDL_Texture* texture = GetTextureDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckTextureDataAt1stPos(texture);
#endif
	SDL_DestroyTexture(texture);
	texture = nullptr;

	return 0;
}


ETHER_API int texture_SetAlpha(lua_State * L)
{
	SDL_Texture* texture = GetTextureDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckTextureDataAt1stPos(texture);
#endif
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int image_GetSize(lua_State * L)
{
	SDL_Surface* surface = GetImageDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckImageDataAt1stPos(surface);
#endif
	lua_pushnumber(L, surface->w);
	lua_pushnumber(L, surface->h);

	return 2;
}


ETHER_API int copyTexture(lua_State * L)
{
	SDL_Texture* texture = GetTextureDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckTextureDataAt1stPos(texture);
#endif
	SDL_Rect rect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 2, rect);
#else
	EE_CheckRect(L, 2, rect);
#endif
	SDL_RenderCopy(pGlobalRenderer, texture, nullptr, &rect);

	return 0;
}


ETHER_API int copyRotateTexture(lua_State * L)
{
	SDL_Texture* texture = GetTextureDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckTextureDataAt1stPos(texture);
#endif
	SDL_Point flipCenter;
	SDL_Rect showRect;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 3, flipCenter);
	CheckRectParam(L, 5, showRect);
#else
	EE_CheckPoint(L, 3, flipCenter);
	EE_CheckRect(L, 5, showRect);
#endif
	SDL_RendererFlip flip = SDL_FLIP_NONE;
#ifdef _ETHER_DEBUG_
	CheckTableParam(L, 4);
#endif
	lua_pushnil(L);
	while (lua_next(L, 4))
	{
		lua_pushvalue(L, -2);
		if (!lua_isnumber(L, -2))
		{
			luaL_error(L, "bad argument #4 to 'CopyRotateTexture' (table elements must be MACRO number, got %s)", luaL_typename(L, -2));
		}
		else
		{
			switch ((int)lua_tonumber(L, -2))
			{
			case FLIP_HORIZONTAL:
				flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
				break;
			case FLIP_VERTICAL:
				flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
				break;
			case FLIP_NONE:
				flip = (SDL_RendererFlip)(flip | SDL_FLIP_NONE);
				break;
			default:
				luaL_error(L, "bad argument #4 to 'CopyRotateTexture' (table elements must be MACRO number, got %s)", luaL_typename(L, -2));
				break;
			}
		}
		lua_pop(L, 2);
	}

	SDL_RenderCopyEx(pGlobalRenderer, texture, nullptr, &showRect, luaL_checknumber(L, 2), &flipCenter, flip);

	return 0;
}


ETHER_API int copyReshapeTexture(lua_State * L)
{
	SDL_Texture* texture = GetTextureDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckTextureDataAt1stPos(texture);
#endif
	SDL_Rect reshapeRect, showRect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 2, reshapeRect);
	CheckRectParam(L, 3, showRect);
#else
	EE_CheckRect(L, 2, reshapeRect);
	EE_CheckRect(L, 3, showRect);
#endif

	SDL_RenderCopy(pGlobalRenderer, texture, &reshapeRect, &showRect);

	return 0;
}


ETHER_API int copyRotateReshapeTexture(lua_State * L)
{
	SDL_Texture* texture = GetTextureDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckTextureDataAt1stPos(texture);
#endif
	SDL_Point flipCenter;
	SDL_Rect reshapeRect, showRect;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 3, flipCenter);
	CheckRectParam(L, 5, reshapeRect);
	CheckRectParam(L, 6, showRect);
#else
	EE_CheckPoint(L, 3, flipCenter);
	EE_CheckRect(L, 5, reshapeRect);
	EE_CheckRect(L, 6, showRect);
#endif

	SDL_RendererFlip flip = SDL_FLIP_NONE;
#ifdef _ETHER_DEBUG_
	CheckTableParam(L, 4);
#endif
	lua_pushnil(L);
	while (lua_next(L, 4))
	{
		lua_pushvalue(L, -2);
		if (!lua_isnumber(L, -2))
		{
			luaL_error(L, "bad argument #4 to 'CopyRotateReshapeTexture' (table elements must be MACRO number, got %s)", luaL_typename(L, -2));
		}
		else
		{
			switch ((int)lua_tonumber(L, -2))
			{
			case FLIP_HORIZONTAL:
				flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
				break;
			case FLIP_VERTICAL:
				flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);
				break;
			case FLIP_NONE:
				flip = (SDL_RendererFlip)(flip | SDL_FLIP_NONE);
				break;
			default:
				luaL_error(L, "bad argument #4 to 'CopyRotateReshapeTexture' (table elements must be MACRO number, got %s)", luaL_typename(L, -2));
				break;
			}
		}
		lua_pop(L, 2);
	}

	SDL_RenderCopyEx(pGlobalRenderer, texture, &reshapeRect, &showRect, luaL_checknumber(L, 2), &flipCenter, flip);

	return 0;
}


ETHER_API int setDrawColor(lua_State * L)
{
	SDL_Color color;
#ifdef _ETHER_DEBUG_
	CheckColorParam(L, 1, color);
#else
	EE_CheckColor(L, 1, color);
#endif

	SDL_SetRenderDrawBlendMode(pGlobalRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pGlobalRenderer, color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int getDrawColor(lua_State * L)
{
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));

	lua_newtable(L);
	lua_pushstring(L, "r");
	lua_pushnumber(L, color.r);
	lua_settable(L, -3);
	lua_pushstring(L, "g");
	lua_pushnumber(L, color.g);
	lua_settable(L, -3);
	lua_pushstring(L, "b");
	lua_pushnumber(L, color.b);
	lua_settable(L, -3);
	lua_pushstring(L, "a");
	lua_pushnumber(L, color.a);
	lua_settable(L, -3);

	return 1;
}


ETHER_API int drawPoint(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif

	SDL_RenderDrawPoint(pGlobalRenderer, point.x, point.y);

	return 0;
}


ETHER_API int drawLine(lua_State * L)
{
	SDL_Point startPoint, endPoint;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, startPoint);
	CheckPointParam(L, 2, endPoint);
#else
	EE_CheckPoint(L, 1, startPoint);
	EE_CheckPoint(L, 2, endPoint);
#endif

	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	aalineRGBA(pGlobalRenderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y, color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawThickLine(lua_State * L)
{
	SDL_Point startPoint, endPoint;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, startPoint);
	CheckPointParam(L, 2, endPoint);
#else
	EE_CheckPoint(L, 1, startPoint);
	EE_CheckPoint(L, 2, endPoint);
#endif

	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	thickLineRGBA(pGlobalRenderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y, luaL_checknumber(L, 3), color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawRectangle(lua_State * L)
{
	SDL_Rect rect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 1, rect);
#else
	EE_CheckRect(L, 1, rect);
#endif
	SDL_RenderDrawRect(pGlobalRenderer, &rect);

	return 0;
}


ETHER_API int drawFillRectangle(lua_State * L)
{
	SDL_Rect rect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 1, rect);
#else
	EE_CheckRect(L, 1, rect);
#endif
	SDL_RenderFillRect(pGlobalRenderer, &rect);

	return 0;
}


ETHER_API int drawRoundRectangle(lua_State * L)
{
	SDL_Rect rect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 1, rect);
#else
	EE_CheckRect(L, 1, rect);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	roundedRectangleRGBA(pGlobalRenderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, luaL_checknumber(L, 2), color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawFillRoundRectangle(lua_State * L)
{
	SDL_Rect rect;
#ifdef _ETHER_DEBUG_
	CheckRectParam(L, 1, rect);
#else
	EE_CheckRect(L, 1, rect);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	roundedBoxRGBA(pGlobalRenderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, luaL_checknumber(L, 2), color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawCircle(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	aacircleRGBA(pGlobalRenderer, point.x, point.y, luaL_checknumber(L, 2), color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawFillCircle(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	filledCircleRGBA(pGlobalRenderer, point.x, point.y, luaL_checknumber(L, 2), color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawEllipse(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	aaellipseRGBA(pGlobalRenderer, point.x, point.y, luaL_checknumber(L, 2), luaL_checknumber(L, 3), color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int drawFillEllipse(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	filledEllipseRGBA(pGlobalRenderer, point.x, point.y, luaL_checknumber(L, 2), luaL_checknumber(L, 3), color.r, color.g, color.b, color.a);

	return 0;
}

ETHER_API int drawPie(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	pieRGBA(pGlobalRenderer, point.x, point.y, luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), color.r, color.g, color.b, color.a);

	return 0;
}

ETHER_API int drawFillPie(lua_State * L)
{
	SDL_Point point;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point);
#else
	EE_CheckPoint(L, 1, point);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	filledPieRGBA(pGlobalRenderer, point.x, point.y, luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), color.r, color.g, color.b, color.a);

	return 0;
}

ETHER_API int drawTriangle(lua_State * L)
{
	SDL_Point point_1, point_2, point_3;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point_1);
	CheckPointParam(L, 2, point_2);
	CheckPointParam(L, 3, point_3);
#else
	EE_CheckPoint(L, 1, point_1);
	EE_CheckPoint(L, 2, point_2);
	EE_CheckPoint(L, 3, point_3);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	aatrigonRGBA(pGlobalRenderer, point_1.x, point_1.y, point_2.x, point_2.y, point_3.x, point_3.y, color.r, color.g, color.b, color.a);

	return 0;
}

ETHER_API int drawFillTriangle(lua_State * L)
{
	SDL_Point point_1, point_2, point_3;
#ifdef _ETHER_DEBUG_
	CheckPointParam(L, 1, point_1);
	CheckPointParam(L, 2, point_2);
	CheckPointParam(L, 3, point_3);
#else
	EE_CheckPoint(L, 1, point_1);
	EE_CheckPoint(L, 2, point_2);
	EE_CheckPoint(L, 3, point_3);
#endif
	SDL_Color color;
	SDL_GetRenderDrawColor(pGlobalRenderer, &(color.r), &(color.g), &(color.b), &(color.a));
	filledTrigonRGBA(pGlobalRenderer, point_1.x, point_1.y, point_2.x, point_2.y, point_3.x, point_3.y, color.r, color.g, color.b, color.a);

	return 0;
}


ETHER_API int loadFontFromFile(lua_State * L)
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


ETHER_API int loadFontFromData(lua_State* L)
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


ETHER_API int __gc_Font(lua_State * L)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	TTF_CloseFont(font);
	font = nullptr;

	return 0;
}


ETHER_API int font_GetStyle(lua_State * L)
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


ETHER_API int font_SetStyle(lua_State * L)
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


ETHER_API int font_GetOutlineWidth(lua_State * L)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_pushnumber(L, TTF_GetFontOutline(font));

	return 1;
}


ETHER_API int font_SetOutlineWidth(lua_State * L)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	TTF_SetFontOutline(font, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int font_GetKerning(lua_State * L)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_pushnumber(L, TTF_GetFontKerning(font));

	return 1;
}


ETHER_API int font_SetKerning(lua_State * L)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	TTF_SetFontKerning(font, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int font_GetHeight(lua_State * L)
{
	TTF_Font* font = GetFontDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckFontDataAt1stPos(font);
#endif
	lua_pushnumber(L, TTF_FontHeight(font));

	return 1;
}


ETHER_API int getTextSize(lua_State * L)
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


ETHER_API int getUTF8TextSize(lua_State * L)
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


ETHER_API int createTextImageSolid(lua_State * L)
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


ETHER_API int createUTF8TextImageSolid(lua_State * L)
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


ETHER_API int createTextImageShaded(lua_State * L)
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


ETHER_API int createUTF8TextImageShaded(lua_State * L)
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


ETHER_API int createTextImageBlended(lua_State * L)
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


ETHER_API int createUTF8TextImageBlended(lua_State * L)
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