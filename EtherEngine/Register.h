#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "PackageWindow.h"
#include "PackageGraphic.h"
#include "PackageMedia.h"
#include "PackageInteractivity.h"
#include "PackageTime.h"
#include "PackageOS.h"
#include "PackageAlgorithm.h"
#include "PackageNetwork.h"
#include "PackageString.h"
#include "PackageJSON.h"
#include "PackageCompress.h"
#include "PackageXML.h"
#include "PackageDebug.h"

#include <lua.hpp>

#include <vector>
#include <string>
#include <functional>

struct ParamEnum
{
	std::string name;
	int value;
};

struct MetaTableData
{
	std::string name;
	std::vector<luaL_Reg> func_list;
	lua_CFunction gc_func;
};

struct BuiltinPackageData
{
	std::string name;
	lua_CFunction on_load;
};

inline void EE_PushBuiltinPackageData(lua_State* pLuaVM, 
	const std::vector<luaL_Reg>& func_list,
	const std::vector<ParamEnum> enum_list = std::vector<ParamEnum>(),
	const std::vector<MetaTableData>&metatable_list = std::vector<MetaTableData>())
{
	for (const MetaTableData& mt : metatable_list)
	{
		luaL_newmetatable(pLuaVM, mt.name.c_str());

		if (!mt.func_list.empty())
		{
			lua_pushstring(pLuaVM, "__index");
			lua_createtable(pLuaVM, 0, mt.func_list.size());
			for (const luaL_Reg& func : mt.func_list)
			{
				lua_pushstring(pLuaVM, func.name);
				lua_pushcfunction(pLuaVM, func.func);
				lua_rawset(pLuaVM, -3);
			}
			lua_rawset(pLuaVM, -3);
		}
		
		if (mt.gc_func)
		{
			lua_pushstring(pLuaVM, "__gc");
			lua_pushcfunction(pLuaVM, mt.gc_func);
			lua_rawset(pLuaVM, -3);
		}
	}

	lua_createtable(pLuaVM, 0, func_list.size() + enum_list.size());

	for (const luaL_Reg& func : func_list)
	{
		lua_pushstring(pLuaVM, func.name);
		lua_pushcfunction(pLuaVM, func.func);
		lua_rawset(pLuaVM, -3);
	}

	for (const ParamEnum& pe : enum_list)
	{
		lua_pushstring(pLuaVM, pe.name.c_str());
		lua_pushinteger(pLuaVM, pe.value);
		lua_rawset(pLuaVM, -3);
	}
}

inline void EE_SetPackageGCFunc(lua_State* pLuaVM, lua_CFunction gc)
{
	lua_createtable(pLuaVM, 0, 1);

	lua_pushstring(pLuaVM, "__gc");
	lua_pushcfunction(pLuaVM, gc);
	lua_rawset(pLuaVM, -3);

	lua_setmetatable(pLuaVM, -2);
}

static std::vector<BuiltinPackageData> BuiltinPackageList =
{
	{
		"@Algorithm",
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "Clamp", clamp },
				{ "CheckPointInRect", checkPointInRect },
				{ "CheckPointInCircle", checkPointInCircle },
				{ "CheckRectsOverlap", checkRectsOverlap },
				{ "CheckCirclesOverlap", checkCirclesOverlap },
				{ "GetPointsDistance", getPointsDistance },
				{ "GetLinesDistance", getLinesDistance },
				{ "GetPointLineDistance", getPointLineDistance },
				{ "RGBAToHSLA", rgbaToHSLA },
				{ "HSLAToRGBA", hslaToRGBA },
				{ "EncodeBase64", encodeBase64 },
				{ "DecodeBase64", decodeBase64 },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list);

			return 1;
		}
	},
	{ 
		"@Graphic",
		[](lua_State* pLuaVM) -> int
		{
			if (!TTF_WasInit()) TTF_Init();
			IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

			std::vector<luaL_Reg> func_list = {
				{ "SetCursorShow", setCursorShow },
				{ "LoadImageFromFile", loadImageFromFile },
				{ "LoadImageFromData", loadImageFromData },
				{ "CreateTexture", createTexture },
				{ "CopyTexture", copyTexture },
				{ "CopyRotateTexture", copyRotateTexture },
				{ "CopyReshapeTexture", copyReshapeTexture },
				{ "CopyRotateReshapeTexture", copyRotateReshapeTexture },
				{ "SetDrawColor", setDrawColor },
				{ "GetDrawColor", getDrawColor },
				{ "DrawPoint", drawPoint },
				{ "DrawLine", drawLine },
				{ "DrawThickLine", drawThickLine },
				{ "DrawRectangle", drawRectangle },
				{ "DrawFillRectangle", drawFillRectangle },
				{ "DrawRoundRectangle", drawRoundRectangle },
				{ "DrawFillRoundRectangle", drawFillRoundRectangle },
				{ "DrawCircle", drawCircle },
				{ "DrawFillCircle", drawFillCircle },
				{ "DrawEllipse", drawEllipse },
				{ "DrawFillEllipse", drawFillEllipse },
				{ "DrawPie", drawPie },
				{ "DrawFillPie", drawFillPie },
				{ "DrawTriangle", drawTriangle },
				{ "DrawFillTriangle", drawFillTriangle },
				{ "LoadFontFromFile", loadFontFromFile },
				{ "LoadFontFromData", loadFontFromData },
				{ "GetTextSize", getTextSize },
				{ "GetUTF8TextSize", getUTF8TextSize },
				{ "CreateTextImageSolid", createTextImageSolid },
				{ "CreateUTF8TextImageSolid", createUTF8TextImageSolid },
				{ "CreateTextImageShaded", createTextImageShaded },
				{ "CreateUTF8TextImageShaded", createUTF8TextImageShaded },
				{ "CreateTextImageBlended", createTextImageBlended },
				{ "CreateUTF8TextImageBlended", createUTF8TextImageBlended },
			};

			std::vector<ParamEnum> enum_list = {
				{ "FLIP_HORIZONTAL", FLIP_HORIZONTAL },
				{ "FLIP_VERTICAL", FLIP_VERTICAL },
				{ "FLIP_NONE", FLIP_NONE },

				{ "FONT_STYLE_BOLD", FONT_STYLE_BOLD },
				{ "FONT_STYLE_ITALIC", FONT_STYLE_ITALIC },
				{ "FONT_STYLE_UNDERLINE", FONT_STYLE_UNDERLINE },
				{ "FONT_STYLE_STRIKETHROUGH", FONT_STYLE_STRIKETHROUGH },
				{ "FONT_STYLE_NORMAL", FONT_STYLE_NORMAL },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_IMAGE,
					{
						{ "SetColorKey", image_SetColorKey },
						{ "GetSize", image_GetSize },
					},
					__gc_Image
				},
				{
					METANAME_TEXTURE,
					{
						{ "SetAlpha", texture_SetAlpha },
					},
					__gc_Texture
				},
				{
					METANAME_FONT,
					{
						{ "GetStyle", font_GetStyle },
						{ "SetStyle", font_SetStyle },
						{ "GetOutlineWidth", font_GetOutlineWidth },
						{ "SetOutlineWidth", font_SetOutlineWidth },
						{ "GetKerning", font_GetKerning },
						{ "SetKerning", font_SetKerning },
						{ "GetHeight", font_GetHeight },
					},
					__gc_Font
				},
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list, metatable_list);

			EE_SetPackageGCFunc(
				pLuaVM,
				[](lua_State* pLuaVM) -> int
				{
					TTF_Quit(); IMG_Quit();

					return 0;
				});

			return 1;
		}
	},
	{ 
		"@Interactivity",
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "GetCursorPosition", getCursorPosition },
				{ "GetScrollValue", getScrollValue },
				{ "StartTextInput", startTextInput },
				{ "StopTextInput", stopTextInput },
				{ "GetInputText", getInputText },
				{ "UpdateEvent", updateEvent },
				{ "GetEventType", getEventType },
			};

			std::vector<ParamEnum> enum_list = {
				{ "EVENT_QUIT", EVENT_QUIT },

				{ "EVENT_MOUSEMOTION", EVENT_MOUSEMOTION },
				{ "EVENT_MOUSEBTNDOWN_LEFT", EVENT_MOUSEBTNDOWN_LEFT },
				{ "EVENT_MOUSEBTNDOWN_RIGHT", EVENT_MOUSEBTNDOWN_RIGHT },
				{ "EVENT_MOUSEBTNDOWN_MIDDLE", EVENT_MOUSEBTNDOWN_MIDDLE },
				{ "EVENT_MOUSEBTNUP_LEFT", EVENT_MOUSEBTNUP_LEFT },
				{ "EVENT_MOUSEBTNUP_RIGHT", EVENT_MOUSEBTNUP_RIGHT },
				{ "EVENT_MOUSEBTNUP_MIDDLE", EVENT_MOUSEBTNUP_MIDDLE },
				{ "EVENT_MOUSESCROLL", EVENT_MOUSESCROLL },

				{ "EVENT_WINDOW_SHOW", EVENT_WINDOW_SHOW },
				{ "EVENT_WINDOW_HIDE", EVENT_WINDOW_HIDE },
				{ "EVENT_WINDOW_MOVE", EVENT_WINDOW_MOVE },
				{ "EVENT_WINDOW_RESIZE", EVENT_WINDOW_RESIZE },
				{ "EVENT_WINDOW_MINSIZE", EVENT_WINDOW_MINSIZE },
				{ "EVENT_WINDOW_MAXSIZE", EVENT_WINDOW_MAXSIZE },
				{ "EVENT_WINDOW_ENTER", EVENT_WINDOW_ENTER },
				{ "EVENT_WINDOW_LEAVE", EVENT_WINDOW_LEAVE },
				{ "EVENT_WINDOW_FOCUS", EVENT_WINDOW_FOCUS },
				{ "EVENT_WINDOW_LOST", EVENT_WINDOW_LOST },
				{ "EVENT_WINDOW_CLOSE", EVENT_WINDOW_CLOSE },
				{ "EVENT_WINDOW_EXPOSED", EVENT_WINDOW_EXPOSED },

				{ "EVENT_KEYDOWN_0", EVENT_KEYDOWN_0 },
				{ "EVENT_KEYDOWN_00", EVENT_KEYDOWN_00 },
				{ "EVENT_KEYDOWN_000", EVENT_KEYDOWN_000 },
				{ "EVENT_KEYDOWN_1", EVENT_KEYDOWN_1 },
				{ "EVENT_KEYDOWN_2", EVENT_KEYDOWN_2 },
				{ "EVENT_KEYDOWN_3", EVENT_KEYDOWN_3 },
				{ "EVENT_KEYDOWN_4", EVENT_KEYDOWN_4 },
				{ "EVENT_KEYDOWN_5", EVENT_KEYDOWN_5 },
				{ "EVENT_KEYDOWN_6", EVENT_KEYDOWN_6 },
				{ "EVENT_KEYDOWN_7", EVENT_KEYDOWN_7 },
				{ "EVENT_KEYDOWN_8", EVENT_KEYDOWN_8 },
				{ "EVENT_KEYDOWN_9", EVENT_KEYDOWN_9 },
				{ "EVENT_KEYDOWN_F1", EVENT_KEYDOWN_F1 },
				{ "EVENT_KEYDOWN_F2", EVENT_KEYDOWN_F2 },
				{ "EVENT_KEYDOWN_F3", EVENT_KEYDOWN_F3 },
				{ "EVENT_KEYDOWN_F4", EVENT_KEYDOWN_F4 },
				{ "EVENT_KEYDOWN_F5", EVENT_KEYDOWN_F5 },
				{ "EVENT_KEYDOWN_F6", EVENT_KEYDOWN_F6 },
				{ "EVENT_KEYDOWN_F7", EVENT_KEYDOWN_F7 },
				{ "EVENT_KEYDOWN_F8", EVENT_KEYDOWN_F8 },
				{ "EVENT_KEYDOWN_F9", EVENT_KEYDOWN_F9 },
				{ "EVENT_KEYDOWN_F10", EVENT_KEYDOWN_F10 },
				{ "EVENT_KEYDOWN_F11", EVENT_KEYDOWN_F11 },
				{ "EVENT_KEYDOWN_F12", EVENT_KEYDOWN_F12 },
				{ "EVENT_KEYDOWN_F13", EVENT_KEYDOWN_F13 },
				{ "EVENT_KEYDOWN_F14", EVENT_KEYDOWN_F14 },
				{ "EVENT_KEYDOWN_F15", EVENT_KEYDOWN_F15 },
				{ "EVENT_KEYDOWN_F16", EVENT_KEYDOWN_F16 },
				{ "EVENT_KEYDOWN_F17", EVENT_KEYDOWN_F17 },
				{ "EVENT_KEYDOWN_F18", EVENT_KEYDOWN_F18 },
				{ "EVENT_KEYDOWN_F19", EVENT_KEYDOWN_F19 },
				{ "EVENT_KEYDOWN_F20", EVENT_KEYDOWN_F20 },
				{ "EVENT_KEYDOWN_F21", EVENT_KEYDOWN_F21 },
				{ "EVENT_KEYDOWN_F22", EVENT_KEYDOWN_F22 },
				{ "EVENT_KEYDOWN_F23", EVENT_KEYDOWN_F23 },
				{ "EVENT_KEYDOWN_F24", EVENT_KEYDOWN_F24 },
				{ "EVENT_KEYDOWN_A", EVENT_KEYDOWN_A },
				{ "EVENT_KEYDOWN_B", EVENT_KEYDOWN_B },
				{ "EVENT_KEYDOWN_C", EVENT_KEYDOWN_C },
				{ "EVENT_KEYDOWN_D", EVENT_KEYDOWN_D },
				{ "EVENT_KEYDOWN_E", EVENT_KEYDOWN_E },
				{ "EVENT_KEYDOWN_F", EVENT_KEYDOWN_F },
				{ "EVENT_KEYDOWN_G", EVENT_KEYDOWN_G },
				{ "EVENT_KEYDOWN_H", EVENT_KEYDOWN_H },
				{ "EVENT_KEYDOWN_I", EVENT_KEYDOWN_I },
				{ "EVENT_KEYDOWN_J", EVENT_KEYDOWN_J },
				{ "EVENT_KEYDOWN_K", EVENT_KEYDOWN_K },
				{ "EVENT_KEYDOWN_L", EVENT_KEYDOWN_L },
				{ "EVENT_KEYDOWN_M", EVENT_KEYDOWN_M },
				{ "EVENT_KEYDOWN_N", EVENT_KEYDOWN_N },
				{ "EVENT_KEYDOWN_O", EVENT_KEYDOWN_O },
				{ "EVENT_KEYDOWN_P", EVENT_KEYDOWN_P },
				{ "EVENT_KEYDOWN_Q", EVENT_KEYDOWN_Q },
				{ "EVENT_KEYDOWN_R", EVENT_KEYDOWN_R },
				{ "EVENT_KEYDOWN_S", EVENT_KEYDOWN_S },
				{ "EVENT_KEYDOWN_T", EVENT_KEYDOWN_T },
				{ "EVENT_KEYDOWN_U", EVENT_KEYDOWN_U },
				{ "EVENT_KEYDOWN_V", EVENT_KEYDOWN_V },
				{ "EVENT_KEYDOWN_W", EVENT_KEYDOWN_W },
				{ "EVENT_KEYDOWN_X", EVENT_KEYDOWN_X },
				{ "EVENT_KEYDOWN_Y", EVENT_KEYDOWN_Y },
				{ "EVENT_KEYDOWN_Z", EVENT_KEYDOWN_Z },
				{ "EVENT_KEYDOWN_ESC", EVENT_KEYDOWN_ESC },
				{ "EVENT_KEYDOWN_ENTER", EVENT_KEYDOWN_ENTER },
				{ "EVENT_KEYDOWN_BACKSPACE", EVENT_KEYDOWN_BACKSPACE },
				{ "EVENT_KEYDOWN_UP", EVENT_KEYDOWN_UP },
				{ "EVENT_KEYDOWN_DOWN", EVENT_KEYDOWN_DOWN },
				{ "EVENT_KEYDOWN_LEFT", EVENT_KEYDOWN_LEFT },
				{ "EVENT_KEYDOWN_RIGHT", EVENT_KEYDOWN_RIGHT },
				{ "EVENT_KEYDOWN_INSERT", EVENT_KEYDOWN_INSERT },
				{ "EVENT_KEYDOWN_DELETE", EVENT_KEYDOWN_DELETE },
				{ "EVENT_KEYDOWN_HOME", EVENT_KEYDOWN_HOME },
				{ "EVENT_KEYDOWN_END", EVENT_KEYDOWN_END },
				{ "EVENT_KEYDOWN_PAGEUP", EVENT_KEYDOWN_PAGEUP },
				{ "EVENT_KEYDOWN_PAGEDOWN", EVENT_KEYDOWN_PAGEDOWN },
				{ "EVENT_KEYDOWN_LEFTCTRL", EVENT_KEYDOWN_LEFTCTRL },
				{ "EVENT_KEYDOWN_LEFTGUI", EVENT_KEYDOWN_LEFTGUI },
				{ "EVENT_KEYDOWN_LEFTALT", EVENT_KEYDOWN_LEFTALT },
				{ "EVENT_KEYDOWN_LEFTSHIFT", EVENT_KEYDOWN_LEFTSHIFT },
				{ "EVENT_KEYDOWN_RIGHTCTRL", EVENT_KEYDOWN_RIGHTCTRL },
				{ "EVENT_KEYDOWN_RIGHTGUI", EVENT_KEYDOWN_RIGHTGUI },
				{ "EVENT_KEYDOWN_RIGHTALT", EVENT_KEYDOWN_RIGHTALT },
				{ "EVENT_KEYDOWN_RIGHTSHIFT", EVENT_KEYDOWN_RIGHTSHIFT },
				{ "EVENT_KEYDOWN_SPACE", EVENT_KEYDOWN_SPACE },
				{ "EVENT_KEYDOWN_TAB", EVENT_KEYDOWN_TAB },
				{ "EVENT_KEYDOWN_CAPSLOCK", EVENT_KEYDOWN_CAPSLOCK },
				{ "EVENT_KEYDOWN_NUMLOCK", EVENT_KEYDOWN_NUMLOCK },
				{ "EVENT_KEYDOWN_PRINTSCREEN", EVENT_KEYDOWN_PRINTSCREEN },
				{ "EVENT_KEYDOWN_SCROLLLOCK", EVENT_KEYDOWN_SCROLLLOCK },
				{ "EVENT_KEYDOWN_PAUSE", EVENT_KEYDOWN_PAUSE },
				{ "EVENT_KEYDOWN_AUDIOMUTE", EVENT_KEYDOWN_AUDIOMUTE },
				{ "EVENT_KEYDOWN_AUDIOPREV", EVENT_KEYDOWN_AUDIOPREV },
				{ "EVENT_KEYDOWN_AUDIONEXT", EVENT_KEYDOWN_AUDIONEXT },
				{ "EVENT_KEYDOWN_AUDIOPLAY", EVENT_KEYDOWN_AUDIOPLAY },
				{ "EVENT_KEYDOWN_AUDIOSTOP", EVENT_KEYDOWN_AUDIOSTOP },
				{ "EVENT_KEYDOWN_VOLUMEUP", EVENT_KEYDOWN_VOLUMEUP },
				{ "EVENT_KEYDOWN_VOLUMEDOWN", EVENT_KEYDOWN_VOLUMEDOWN },
				{ "EVENT_KEYDOWN_BRIGHTNESSUP", EVENT_KEYDOWN_BRIGHTNESSUP },
				{ "EVENT_KEYDOWN_BRIGHTNESSDOWN", EVENT_KEYDOWN_BRIGHTNESSDOWN },
				{ "EVENT_KEYDOWN_BACKQUOTE", EVENT_KEYDOWN_BACKQUOTE },
				{ "EVENT_KEYDOWN_EXCLAM", EVENT_KEYDOWN_EXCLAM },
				{ "EVENT_KEYDOWN_AT", EVENT_KEYDOWN_AT },
				{ "EVENT_KEYDOWN_HASH", EVENT_KEYDOWN_HASH },
				{ "EVENT_KEYDOWN_DOOLAR", EVENT_KEYDOWN_DOOLAR },
				{ "EVENT_KEYDOWN_CARET", EVENT_KEYDOWN_CARET },
				{ "EVENT_KEYDOWN_AMPERSAND", EVENT_KEYDOWN_AMPERSAND },
				{ "EVENT_KEYDOWN_DBLAMPERSAND", EVENT_KEYDOWN_DBLAMPERSAND },
				{ "EVENT_KEYDOWN_ASTERISK", EVENT_KEYDOWN_ASTERISK },
				{ "EVENT_KEYDOWN_LEFTPAREN", EVENT_KEYDOWN_LEFTPAREN },
				{ "EVENT_KEYDOWN_RIGHTPAREN", EVENT_KEYDOWN_RIGHTPAREN },
				{ "EVENT_KEYDOWN_MINUS", EVENT_KEYDOWN_MINUS },
				{ "EVENT_KEYDOWN_UNDERSCORE", EVENT_KEYDOWN_UNDERSCORE },
				{ "EVENT_KEYDOWN_PLUS", EVENT_KEYDOWN_PLUS },
				{ "EVENT_KEYDOWN_EQUALS", EVENT_KEYDOWN_EQUALS },
				{ "EVENT_KEYDOWN_LEFTBRACKET", EVENT_KEYDOWN_LEFTBRACKET },
				{ "EVENT_KEYDOWN_RIGHTBRACKET", EVENT_KEYDOWN_RIGHTBRACKET },
				{ "EVENT_KEYDOWN_LEFTBRACE", EVENT_KEYDOWN_LEFTBRACE },
				{ "EVENT_KEYDOWN_RIGHTBRACE", EVENT_KEYDOWN_RIGHTBRACE },
				{ "EVENT_KEYDOWN_COLON", EVENT_KEYDOWN_COLON },
				{ "EVENT_KEYDOWN_SEMICOLON", EVENT_KEYDOWN_SEMICOLON },
				{ "EVENT_KEYDOWN_BACKSLASH", EVENT_KEYDOWN_BACKSLASH },
				{ "EVENT_KEYDOWN_QUOTE", EVENT_KEYDOWN_QUOTE },
				{ "EVENT_KEYDOWN_QUOTEDBL", EVENT_KEYDOWN_QUOTEDBL },
				{ "EVENT_KEYDOWN_LESS", EVENT_KEYDOWN_LESS },
				{ "EVENT_KEYDOWN_GREATER", EVENT_KEYDOWN_GREATER },
				{ "EVENT_KEYDOWN_COMMA", EVENT_KEYDOWN_COMMA },
				{ "EVENT_KEYDOWN_PERIOD", EVENT_KEYDOWN_PERIOD },
				{ "EVENT_KEYDOWN_QUESTION", EVENT_KEYDOWN_QUESTION },
				{ "EVENT_KEYDOWN_SLASH", EVENT_KEYDOWN_SLASH },
				{ "EVENT_KEYDOWN_VERTICALBAR", EVENT_KEYDOWN_VERTICALBAR },
				{ "EVENT_KEYDOWN_DBLVERTICALBAR", EVENT_KEYDOWN_DBLVERTICALBAR },
				{ "EVENT_KEYDOWN_WWW", EVENT_KEYDOWN_WWW },
				{ "EVENT_KEYDOWN_EMAIL", EVENT_KEYDOWN_EMAIL },
				{ "EVENT_KEYUP_0", EVENT_KEYUP_0 },
				{ "EVENT_KEYUP_00", EVENT_KEYUP_00 },
				{ "EVENT_KEYUP_000", EVENT_KEYUP_000 },
				{ "EVENT_KEYUP_1", EVENT_KEYUP_1 },
				{ "EVENT_KEYUP_2", EVENT_KEYUP_2 },
				{ "EVENT_KEYUP_3", EVENT_KEYUP_3 },
				{ "EVENT_KEYUP_4", EVENT_KEYUP_4 },
				{ "EVENT_KEYUP_5", EVENT_KEYUP_5 },
				{ "EVENT_KEYUP_6", EVENT_KEYUP_6 },
				{ "EVENT_KEYUP_7", EVENT_KEYUP_7 },
				{ "EVENT_KEYUP_8", EVENT_KEYUP_8 },
				{ "EVENT_KEYUP_9", EVENT_KEYUP_9 },
				{ "EVENT_KEYUP_F1", EVENT_KEYUP_F1 },
				{ "EVENT_KEYUP_F2", EVENT_KEYUP_F2 },
				{ "EVENT_KEYUP_F3", EVENT_KEYUP_F3 },
				{ "EVENT_KEYUP_F4", EVENT_KEYUP_F4 },
				{ "EVENT_KEYUP_F5", EVENT_KEYUP_F5 },
				{ "EVENT_KEYUP_F6", EVENT_KEYUP_F6 },
				{ "EVENT_KEYUP_F7", EVENT_KEYUP_F7 },
				{ "EVENT_KEYUP_F8", EVENT_KEYUP_F8 },
				{ "EVENT_KEYUP_F9", EVENT_KEYUP_F9 },
				{ "EVENT_KEYUP_F10", EVENT_KEYUP_F10 },
				{ "EVENT_KEYUP_F11", EVENT_KEYUP_F11 },
				{ "EVENT_KEYUP_F12", EVENT_KEYUP_F12 },
				{ "EVENT_KEYUP_F13", EVENT_KEYUP_F13 },
				{ "EVENT_KEYUP_F14", EVENT_KEYUP_F14 },
				{ "EVENT_KEYUP_F15", EVENT_KEYUP_F15 },
				{ "EVENT_KEYUP_F16", EVENT_KEYUP_F16 },
				{ "EVENT_KEYUP_F17", EVENT_KEYUP_F17 },
				{ "EVENT_KEYUP_F18", EVENT_KEYUP_F18 },
				{ "EVENT_KEYUP_F19", EVENT_KEYUP_F19 },
				{ "EVENT_KEYUP_F20", EVENT_KEYUP_F20 },
				{ "EVENT_KEYUP_F21", EVENT_KEYUP_F21 },
				{ "EVENT_KEYUP_F22", EVENT_KEYUP_F22 },
				{ "EVENT_KEYUP_F23", EVENT_KEYUP_F23 },
				{ "EVENT_KEYUP_F24", EVENT_KEYUP_F24 },
				{ "EVENT_KEYUP_A", EVENT_KEYUP_A },
				{ "EVENT_KEYUP_B", EVENT_KEYUP_B },
				{ "EVENT_KEYUP_C", EVENT_KEYUP_C },
				{ "EVENT_KEYUP_D", EVENT_KEYUP_D },
				{ "EVENT_KEYUP_E", EVENT_KEYUP_E },
				{ "EVENT_KEYUP_F", EVENT_KEYUP_F },
				{ "EVENT_KEYUP_G", EVENT_KEYUP_G },
				{ "EVENT_KEYUP_H", EVENT_KEYUP_H },
				{ "EVENT_KEYUP_I", EVENT_KEYUP_I },
				{ "EVENT_KEYUP_J", EVENT_KEYUP_J },
				{ "EVENT_KEYUP_K", EVENT_KEYUP_K },
				{ "EVENT_KEYUP_L", EVENT_KEYUP_L },
				{ "EVENT_KEYUP_M", EVENT_KEYUP_M },
				{ "EVENT_KEYUP_N", EVENT_KEYUP_N },
				{ "EVENT_KEYUP_O", EVENT_KEYUP_O },
				{ "EVENT_KEYUP_P", EVENT_KEYUP_P },
				{ "EVENT_KEYUP_Q", EVENT_KEYUP_Q },
				{ "EVENT_KEYUP_R", EVENT_KEYUP_R },
				{ "EVENT_KEYUP_S", EVENT_KEYUP_S },
				{ "EVENT_KEYUP_T", EVENT_KEYUP_T },
				{ "EVENT_KEYUP_U", EVENT_KEYUP_U },
				{ "EVENT_KEYUP_V", EVENT_KEYUP_V },
				{ "EVENT_KEYUP_W", EVENT_KEYUP_W },
				{ "EVENT_KEYUP_X", EVENT_KEYUP_X },
				{ "EVENT_KEYUP_Y", EVENT_KEYUP_Y },
				{ "EVENT_KEYUP_Z", EVENT_KEYUP_Z },
				{ "EVENT_KEYUP_ESC", EVENT_KEYUP_ESC },
				{ "EVENT_KEYUP_ENTER", EVENT_KEYUP_ENTER },
				{ "EVENT_KEYUP_BACKSPACE", EVENT_KEYUP_BACKSPACE },
				{ "EVENT_KEYUP_UP", EVENT_KEYUP_UP },
				{ "EVENT_KEYUP_DOWN", EVENT_KEYUP_DOWN },
				{ "EVENT_KEYUP_LEFT", EVENT_KEYUP_LEFT },
				{ "EVENT_KEYUP_RIGHT", EVENT_KEYUP_RIGHT },
				{ "EVENT_KEYUP_INSERT", EVENT_KEYUP_INSERT },
				{ "EVENT_KEYUP_DELETE", EVENT_KEYUP_DELETE },
				{ "EVENT_KEYUP_HOME", EVENT_KEYUP_HOME },
				{ "EVENT_KEYUP_END", EVENT_KEYUP_END },
				{ "EVENT_KEYUP_PAGEUP", EVENT_KEYUP_PAGEUP },
				{ "EVENT_KEYUP_PAGEDOWN", EVENT_KEYUP_PAGEDOWN },
				{ "EVENT_KEYUP_LEFTCTRL", EVENT_KEYUP_LEFTCTRL },
				{ "EVENT_KEYUP_LEFTGUI", EVENT_KEYUP_LEFTGUI },
				{ "EVENT_KEYUP_LEFTALT", EVENT_KEYUP_LEFTALT },
				{ "EVENT_KEYUP_LEFTSHIFT", EVENT_KEYUP_LEFTSHIFT },
				{ "EVENT_KEYUP_RIGHTCTRL", EVENT_KEYUP_RIGHTCTRL },
				{ "EVENT_KEYUP_RIGHTGUI", EVENT_KEYUP_RIGHTGUI },
				{ "EVENT_KEYUP_RIGHTALT", EVENT_KEYUP_RIGHTALT },
				{ "EVENT_KEYUP_RIGHTSHIFT", EVENT_KEYUP_RIGHTSHIFT },
				{ "EVENT_KEYUP_SPACE", EVENT_KEYUP_SPACE },
				{ "EVENT_KEYUP_TAB", EVENT_KEYUP_TAB },
				{ "EVENT_KEYUP_CAPSLOCK", EVENT_KEYUP_CAPSLOCK },
				{ "EVENT_KEYUP_NUMLOCK", EVENT_KEYUP_NUMLOCK },
				{ "EVENT_KEYUP_PRINTSCREEN", EVENT_KEYUP_PRINTSCREEN },
				{ "EVENT_KEYUP_SCROLLLOCK", EVENT_KEYUP_SCROLLLOCK },
				{ "EVENT_KEYUP_PAUSE", EVENT_KEYUP_PAUSE },
				{ "EVENT_KEYUP_AUDIOMUTE", EVENT_KEYUP_AUDIOMUTE },
				{ "EVENT_KEYUP_AUDIOPREV", EVENT_KEYUP_AUDIOPREV },
				{ "EVENT_KEYUP_AUDIONEXT", EVENT_KEYUP_AUDIONEXT },
				{ "EVENT_KEYUP_AUDIOPLAY", EVENT_KEYUP_AUDIOPLAY },
				{ "EVENT_KEYUP_AUDIOSTOP", EVENT_KEYUP_AUDIOSTOP },
				{ "EVENT_KEYUP_VOLUMEUP", EVENT_KEYUP_VOLUMEUP },
				{ "EVENT_KEYUP_VOLUMEDOWN", EVENT_KEYUP_VOLUMEDOWN },
				{ "EVENT_KEYUP_BRIGHTNESSUP", EVENT_KEYUP_BRIGHTNESSUP },
				{ "EVENT_KEYUP_BRIGHTNESSDOWN", EVENT_KEYUP_BRIGHTNESSDOWN },
				{ "EVENT_KEYUP_BACKQUOTE", EVENT_KEYUP_BACKQUOTE },
				{ "EVENT_KEYUP_EXCLAM", EVENT_KEYUP_EXCLAM },
				{ "EVENT_KEYUP_AT", EVENT_KEYUP_AT },
				{ "EVENT_KEYUP_HASH", EVENT_KEYUP_HASH },
				{ "EVENT_KEYUP_DOOLAR", EVENT_KEYUP_DOOLAR },
				{ "EVENT_KEYUP_CARET", EVENT_KEYUP_CARET },
				{ "EVENT_KEYUP_AMPERSAND", EVENT_KEYUP_AMPERSAND },
				{ "EVENT_KEYUP_DBLAMPERSAND", EVENT_KEYUP_DBLAMPERSAND },
				{ "EVENT_KEYUP_ASTERISK", EVENT_KEYUP_ASTERISK },
				{ "EVENT_KEYUP_LEFTPAREN", EVENT_KEYUP_LEFTPAREN },
				{ "EVENT_KEYUP_RIGHTPAREN", EVENT_KEYUP_RIGHTPAREN },
				{ "EVENT_KEYUP_MINUS", EVENT_KEYUP_MINUS },
				{ "EVENT_KEYUP_UNDERSCORE", EVENT_KEYUP_UNDERSCORE },
				{ "EVENT_KEYUP_PLUS", EVENT_KEYUP_PLUS },
				{ "EVENT_KEYUP_EQUALS", EVENT_KEYUP_EQUALS },
				{ "EVENT_KEYUP_LEFTBRACKET", EVENT_KEYUP_LEFTBRACKET },
				{ "EVENT_KEYUP_RIGHTBRACKET", EVENT_KEYUP_RIGHTBRACKET },
				{ "EVENT_KEYUP_LEFTBRACE", EVENT_KEYUP_LEFTBRACE },
				{ "EVENT_KEYUP_RIGHTBRACE", EVENT_KEYUP_RIGHTBRACE },
				{ "EVENT_KEYUP_COLON", EVENT_KEYUP_COLON },
				{ "EVENT_KEYUP_SEMICOLON", EVENT_KEYUP_SEMICOLON },
				{ "EVENT_KEYUP_BACKSLASH", EVENT_KEYUP_BACKSLASH },
				{ "EVENT_KEYUP_QUOTE", EVENT_KEYUP_QUOTE },
				{ "EVENT_KEYUP_QUOTEDBL", EVENT_KEYUP_QUOTEDBL },
				{ "EVENT_KEYUP_LESS", EVENT_KEYUP_LESS },
				{ "EVENT_KEYUP_GREATER", EVENT_KEYUP_GREATER },
				{ "EVENT_KEYUP_COMMA", EVENT_KEYUP_COMMA },
				{ "EVENT_KEYUP_PERIOD", EVENT_KEYUP_PERIOD },
				{ "EVENT_KEYUP_QUESTION", EVENT_KEYUP_QUESTION },
				{ "EVENT_KEYUP_SLASH", EVENT_KEYUP_SLASH },
				{ "EVENT_KEYUP_VERTICALBAR", EVENT_KEYUP_VERTICALBAR },
				{ "EVENT_KEYUP_DBLVERTICALBAR", EVENT_KEYUP_DBLVERTICALBAR },
				{ "EVENT_KEYUP_WWW", EVENT_KEYUP_WWW },
				{ "EVENT_KEYUP_EMAIL", EVENT_KEYUP_EMAIL },

				{ "EVENT_TEXTINPUT", EVENT_TEXTINPUT },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list);

			return 1;
		}
	},
	{ 
		"@JSON",			
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "LoadJSON", loadJSON },
				{ "LoadJSONFromFile", loadJSONFromFile },
				{ "DumpJSON", dumpJSON },
				{ "DumpJSONToFile", dumpJSONToFile },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list);

			return 1;
		}
	},
	{ 
		"@Media",			
		[](lua_State* pLuaVM) -> int
		{
			Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG);
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

			std::vector<luaL_Reg> func_list = {
				{ "LoadMusic", loadMusic },
				{ "PlayMusic", playMusic },
				{ "PlayMusicWithFadeIn", playMusicWithFadeIn },
				{ "SetMusicPosition", setMusicPosition },
				{ "SetMusicVolume", setMusicVolume },
				{ "GetMusicVolume", getMusicVolume },
				{ "StopMusic", stopMusic },
				{ "StopMusicWithFadeOut", stopMusicWithFadeOut },
				{ "PauseMusic", pauseMusic },
				{ "ResumeMusic", resumeMusic },
				{ "RewindMusic", rewindMusic },
				{ "CheckMusicPlaying", checkMusicPlaying },
				{ "CheckMusicPaused", checkMusicPaused },
				{ "GetMusicFadingType", getMusicFadingType },
				{ "LoadSoundFromFile", loadSoundFromFile },
				{ "LoadSoundFromData", loadSoundFromData },
			};

			std::vector<ParamEnum> enum_list = {
				{ "MUSIC_TYPE_WAV", MUSIC_TYPE_WAV },
				{ "MUSIC_TYPE_MP3", MUSIC_TYPE_MP3 },
				{ "MUSIC_TYPE_OGG", MUSIC_TYPE_OGG },
				{ "MUSIC_TYPE_CMD", MUSIC_TYPE_CMD },
				{ "MUSIC_TYPE_MOD", MUSIC_TYPE_MOD },
				{ "MUSIC_TYPE_MID", MUSIC_TYPE_MID },
				{ "MUSIC_TYPE_UNKONWN", MUSIC_TYPE_UNKONWN },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_MUSIC,
					{
						{ "GetType", music_GetType },
					},
					__gc_Music
				},
				{
					METANAME_SOUND,
					{
						{ "Play", sound_Play },
						{ "SetVolume", sound_SetVolume },
					},
					__gc_Sound
				},
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list, metatable_list);

			EE_SetPackageGCFunc(
				pLuaVM,
				[](lua_State* pLuaVM) -> int
				{
					Mix_CloseAudio(); Mix_Quit();

					return 0;
				});

			return 1;
		}
	},
	{ 
		"@Network",		
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "CreateClient", createClient },
				{ "CreateServer", createServer },
				{ "SplitLink", splitLink },
			};

			std::vector<ParamEnum> enum_list = {
				{ "ERRCODE_SUCCESS", ERRCODE_SUCCESS },
				{ "ERRCODE_UNKNOWN", ERRCODE_UNKNOWN },
				{ "ERRCODE_CONNECTION", ERRCODE_CONNECTION },
				{ "ERRCODE_BINDIPADDRESS", ERRCODE_BINDIPADDRESS },
				{ "ERRCODE_READ", ERRCODE_READ },
				{ "ERRCODE_WRITE", ERRCODE_WRITE },
				{ "ERRCODE_EXCEEDREDRICTCOUNT", ERRCODE_EXCEEDREDRICTCOUNT },
				{ "ERRCODE_CANCELED", ERRCODE_CANCELED },
				{ "ERRCODE_SSLCONNECTION", ERRCODE_SSLCONNECTION },
				{ "ERRCODE_SSLLOADINGCERTS", ERRCODE_SSLLOADINGCERTS },
				{ "ERRCODE_SSLSERVERVERIFY", ERRCODE_SSLSERVERVERIFY },
				{ "ERRCODE_UNSUPPORTEDMBC", ERRCODE_UNSUPPORTEDMBC },
				{ "ERRCODE_COMPRESSION", ERRCODE_COMPRESSION },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_CLIENT,
					{
						{ "CheckValid", client_CheckValid },
						{ "Get", client_Get },
						{ "Post", client_Post },
						{ "Put", client_Put },
						{ "Patch", client_Patch },
						{ "Delete", client_Delete },
						{ "Options", client_Options },
						{ "SetDefaultHeaders", client_SetDefaultHeaders },
						{ "SetConnectTimeout", client_SetConnectTimeout },
						{ "SetReadTimeout", client_SetReadTimeout },
						{ "SetWriteTimeout", client_SetWriteTimeout },
						{ "SetKeepAlive", client_SetKeepAlive },
						{ "SetFollowRedirect", client_SetFollowRedirect },
						{ "SetCompressRequest", client_SetCompressRequest },
						{ "SetCompressResponse", client_SetCompressResponse },
						{ "SetCACertPath", client_SetCACertPath },
						{ "SetProxy", client_SetProxy },
					},
					__gc_Client
				},
				{
					METANAME_SERVER,
					{
						{ "CheckValid", server_CheckValid },
						{ "CheckRunning", server_CheckRunning },
						{ "Get", server_Get },
						{ "Post", server_Post },
						{ "Put", server_Put },
						{ "Patch", server_Patch },
						{ "Delete", server_Delete },
						{ "Options", server_Options },
						{ "SetMountPoint", server_SetMountPoint },
						{ "RemoveMountPoint", server_RemoveMountPoint },
						{ "SetFileExtMapToMIMEType", server_SetFileExtMapToMIMEType },
						{ "SetExceptionHandler", server_SetExceptionHandler },
						{ "SetMaxKeepAliveCount", server_SetMaxKeepAliveCount },
						{ "SetKeepAliveTimeout", server_SetKeepAliveTimeout },
						{ "SetReadTimeout", server_SetReadTimeout },
						{ "SetWriteTimeout", server_SetWriteTimeout },
						{ "SetIdleInterval", server_SetIdleInterval },
						{ "SetMaxRequestLength", server_SetMaxRequestLength },
						{ "BindToAnyPort", server_BindToAnyPort },
						{ "ListenAfterBind", server_ListenAfterBind },
						{ "Listen", server_Listen },
						{ "Stop", server_Stop },
					},
					__gc_Server
				},
				{
					METANAME_SERVER_REQ,
					{
						{ "GetMethod", request_GetMethod },
						{ "GetRoute", request_GetRoute },
						{ "GetHeaders", request_GetHeaders },
						{ "GetBody", request_GetBody },
						{ "GetRemoteAddress", request_GetRemoteAddress },
						{ "GetRemotePort", request_GetRemotePort },
						{ "GetVersion", request_GetVersion },
						{ "GetParams", request_GetParams },
						{ "CheckHeaderKeyExist", request_CheckHeaderKeyExist },
						{ "GetHeaderValue", request_GetHeaderValue },
						{ "GetHeaderValueCount", request_GetHeaderValueCount },
						{ "CheckParamKeyExist", request_CheckParamKeyExist },
						{ "GetParamValue", request_GetParamValue },
						{ "GetParamValueCount", request_GetParamValueCount },
					}
				},
				{
					METANAME_SERVER_RES,
					{
						{ "SetVersion", response_SetVersion },
						{ "SetStatus", response_SetStatus },
						{ "GetHeaders", response_GetHeaders },
						{ "SetBody", response_SetBody },
						{ "CheckHeaderKeyExist", response_CheckHeaderKeyExist },
						{ "GetHeaderValue", response_GetHeaderValue },
						{ "GetHeaderValueCount", response_GetHeaderValueCount },
						{ "SetHeaderValue", response_SetHeaderValue },
						{ "SetHeaders", response_SetHeaders },
						{ "SetRedirect", response_SetRedirect },
						{ "SetContent", response_SetContent },
					}
				}
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list, metatable_list);

			return 1;
		}
	},
	{
		"@OS",			
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "GetBasePath", getBasePath },
				{ "SetClipboardText", setClipboardText },
				{ "GetClipboardText", getClipboardText },
				{ "GetPlatformType", getPlatformType },
				{ "GetCPUCount", getCPUCount },
				{ "GetSystemTotalRAM", getSystemTotalRAM },
				{ "GetAppStorageDirectory", getAppStorageDirectory },
				{ "GetSpecialPath", getSpecialPath },
				{ "GetPowerInfo", getPowerInfo },
				{ "ListDirectory", listDirectory },
				{ "CheckPathExist", checkPathExist },
				{ "GetPathInfo", getPathInfo },
				{ "JoinPath", joinPath },
				{ "SplitPath", splitPath },
			};

			std::vector<ParamEnum> enum_list = {
				{ "FILEATTRIB_ARCH", FILEATTRIB_ARCH },
				{ "FILEATTRIB_HIDDEN", FILEATTRIB_HIDDEN },
				{ "FILEATTRIB_NORMAL", FILEATTRIB_NORMAL },
				{ "FILEATTRIB_RDONLY", FILEATTRIB_RDONLY },
				{ "FILEATTRIB_SUBDIR", FILEATTRIB_SUBDIR },
				{ "FILEATTRIB_SYSTEM", FILEATTRIB_SYSTEM },

				{ "PATHMODE_FILE", PATHMODE_FILE },
				{ "PATHMODE_DIR", PATHMODE_DIR },
				{ "PATHMODE_FILEANDDIR", PATHMODE_FILEANDDIR },

				{ "POWERSTATE_UNKOWN", POWERSTATE_UNKOWN },
				{ "POWERSTATE_ONBATTERY", POWERSTATE_ONBATTERY },
				{ "POWERSTATE_NOBATTERY", POWERSTATE_NOBATTERY },
				{ "POWERSTATE_CHARGING", POWERSTATE_CHARGING },
				{ "POWERSTATE_CHARGEDN", POWERSTATE_CHARGEDN },

				{ "PATHATTRIB_DESKTOP", PATHATTRIB_DESKTOP },
				{ "PATHATTRIB_INTERNET", PATHATTRIB_INTERNET },
				{ "PATHATTRIB_PROGRAMS", PATHATTRIB_PROGRAMS },
				{ "PATHATTRIB_CONTROLS", PATHATTRIB_CONTROLS },
				{ "PATHATTRIB_PRINTERS", PATHATTRIB_PRINTERS },
				{ "PATHATTRIB_DOCUMENTS", PATHATTRIB_DOCUMENTS },
				{ "PATHATTRIB_FAVORITES", PATHATTRIB_FAVORITES },
				{ "PATHATTRIB_STARTUP", PATHATTRIB_STARTUP },
				{ "PATHATTRIB_RECENT", PATHATTRIB_RECENT },
				{ "PATHATTRIB_SENDTO", PATHATTRIB_SENDTO },
				{ "PATHATTRIB_RECYCLEBIN", PATHATTRIB_RECYCLEBIN },
				{ "PATHATTRIB_STARTMENU", PATHATTRIB_STARTMENU },
				{ "PATHATTRIB_MUSIC", PATHATTRIB_MUSIC },
				{ "PATHATTRIB_VIDEO", PATHATTRIB_VIDEO },
				{ "PATHATTRIB_DRIVES", PATHATTRIB_DRIVES },
				{ "PATHATTRIB_NETWORK", PATHATTRIB_NETWORK },
				{ "PATHATTRIB_NETHOOD", PATHATTRIB_NETHOOD },
				{ "PATHATTRIB_FONTS", PATHATTRIB_FONTS },
				{ "PATHATTRIB_TEMPLATES", PATHATTRIB_TEMPLATES },
				{ "PATHATTRIB_COMMON_STARTMENU", PATHATTRIB_COMMON_STARTMENU },
				{ "PATHATTRIB_COMMON_PROGRAMS", PATHATTRIB_COMMON_PROGRAMS },
				{ "PATHATTRIB_COMMON_STARTUP", PATHATTRIB_COMMON_STARTUP },
				{ "PATHATTRIB_COMMON_DESKTOP", PATHATTRIB_COMMON_DESKTOP },
				{ "PATHATTRIB_APPDATA", PATHATTRIB_APPDATA },
				{ "PATHATTRIB_PRINTHOOD", PATHATTRIB_PRINTHOOD },
				{ "PATHATTRIB_LOCAL_APPDATA", PATHATTRIB_LOCAL_APPDATA },
				{ "PATHATTRIB_COMMON_FAVORITES", PATHATTRIB_COMMON_FAVORITES },
				{ "PATHATTRIB_INTERNET_CACHE", PATHATTRIB_INTERNET_CACHE },
				{ "PATHATTRIB_COOKIES", PATHATTRIB_COOKIES },
				{ "PATHATTRIB_HISTORY", PATHATTRIB_HISTORY },
				{ "PATHATTRIB_COMMON_APPDATA", PATHATTRIB_COMMON_APPDATA },
				{ "PATHATTRIB_WINDOWS", PATHATTRIB_WINDOWS },
				{ "PATHATTRIB_SYSTEM", PATHATTRIB_SYSTEM },
				{ "PATHATTRIB_PROGRAM_FILES", PATHATTRIB_PROGRAM_FILES },
				{ "PATHATTRIB_PICTURES", PATHATTRIB_PICTURES },
				{ "PATHATTRIB_PROFILE", PATHATTRIB_PROFILE },
				{ "PATHATTRIB_SYSTEMX86", PATHATTRIB_SYSTEMX86 },
				{ "PATHATTRIB_PROGRAM_FILESX86", PATHATTRIB_PROGRAM_FILESX86 },
				{ "PATHATTRIB_PROGRAM_FILES_COMMON", PATHATTRIB_PROGRAM_FILES_COMMON },
				{ "PATHATTRIB_PROGRAM_FILES_COMMONX86", PATHATTRIB_PROGRAM_FILES_COMMONX86 },
				{ "PATHATTRIB_COMMON_TEMPLATES", PATHATTRIB_COMMON_TEMPLATES },
				{ "PATHATTRIB_COMMON_DOCUMENTS", PATHATTRIB_COMMON_DOCUMENTS },
				{ "PATHATTRIB_COMMON_ADMINTOOLS", PATHATTRIB_COMMON_ADMINTOOLS },
				{ "PATHATTRIB_ADMINTOOLS", PATHATTRIB_ADMINTOOLS },
				{ "PATHATTRIB_CONNECTIONS", PATHATTRIB_CONNECTIONS },
				{ "PATHATTRIB_COMMON_MUSIC", PATHATTRIB_COMMON_MUSIC },
				{ "PATHATTRIB_COMMON_PICTURES", PATHATTRIB_COMMON_PICTURES },
				{ "PATHATTRIB_COMMON_VIDEO", PATHATTRIB_COMMON_VIDEO },
				{ "PATHATTRIB_RESOURCES", PATHATTRIB_RESOURCES },
				{ "PATHATTRIB_RESOURCES_LOCALIZED", PATHATTRIB_RESOURCES_LOCALIZED },
				{ "PATHATTRIB_COMMON_OEM_LINKS", PATHATTRIB_COMMON_OEM_LINKS },
				{ "PATHATTRIB_CDBURN_AREA", PATHATTRIB_CDBURN_AREA },
				{ "PATHATTRIB_COMPUTERSNEARME", PATHATTRIB_COMPUTERSNEARME },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list);

			return 1;
		}
	},
	{ 
		"@String",		
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "GBKToUTF8", gbkToUTF8 },
				{ "UTF8ToGBK", utf8ToGBK },
				{ "SubStrUTF8", subStrUTF8 },
				{ "LenUTF8", lenUTF8 },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list);

			return 1;
		}
	},
	{ 
		"@Time",			
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "Pause", pause },
				{ "Sleep", sleep },
				{ "DynamicSleep", dynamicSleep },
				{ "GetInitTime", getInitTime },
				{ "GetAccurateCount", getAccurateCount },
				{ "GetCounterFrequency", getCounterFrequency },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list);

			return 1;
		}
	},
	{ 
		"@Window",
		[](lua_State* pLuaVM) -> int
		{
			Window_pMsgBoxColorScheme = new SDL_MessageBoxColorScheme();
			Window_pMsgBoxColorScheme->colors[0] = { 255, 0, 0 };
			Window_pMsgBoxColorScheme->colors[1] = { 0, 255, 0 };
			Window_pMsgBoxColorScheme->colors[2] = { 255, 255, 0 };
			Window_pMsgBoxColorScheme->colors[3] = { 0, 0, 255 };
			Window_pMsgBoxColorScheme->colors[4] = { 255, 0, 255 };

			Window_mapCursor = {
				{ WINDOW_CURSOR_ARROW,		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW) },
				{ WINDOW_CURSOR_IBEAM,		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM) },
				{ WINDOW_CURSOR_WAIT,		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT) },
				{ WINDOW_CURSOR_CROSSHAIR,	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR) },
				{ WINDOW_CURSOR_WAITARROW,	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW) },
				{ WINDOW_CURSOR_SIZENWSE,	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE) },
				{ WINDOW_CURSOR_SIZENESW,	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW) },
				{ WINDOW_CURSOR_SIZEWE,		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE) },
				{ WINDOW_CURSOR_SIZENS,		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS) },
				{ WINDOW_CURSOR_SIZEALL,	SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL) },
				{ WINDOW_CURSOR_NO,			SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO) },
				{ WINDOW_CURSOR_HAND,		SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND) },
			};

			std::vector<luaL_Reg> func_list = {
				{ "GetWindowHandle",	EAPI_Window_GetWindowHandle},
				{ "GetRendererHandle",	EAPI_Window_GetRendererHandle},
				{ "SetCursorShown",		EAPI_Window_SetCursorShown},
				{ "GetCursorShown",		EAPI_Window_GetCursorShown},
				{ "SetCursorStyle",		EAPI_Window_SetCursorStyle},
				{ "GetCursorStyle",		EAPI_Window_GetCursorStyle},
				{ "MessageBox",			EAPI_Window_MessageBox},
				{ "ConfirmBox",			EAPI_Window_ConfirmBox},
				{ "Create",				EAPI_Window_Create },
				{ "Close",				EAPI_Window_Close },
				{ "SetTitle",			EAPI_Window_SetTitle },
				{ "GetTitle",			EAPI_Window_GetTitle },
				{ "SetStyle",			EAPI_Window_SetStyle },
				{ "SetOpacity",			EAPI_Window_SetOpacity },
				{ "SetSize",			EAPI_Window_SetSize },
				{ "GetSize",			EAPI_Window_GetSize },
				{ "GetDrawableSize",	EAPI_Window_GetDrawableSize },
				{ "SetMaxSize",			EAPI_Window_SetMaxSize },
				{ "GetMaxSize",			EAPI_Window_GetMaxSize },
				{ "SetMinSize",			EAPI_Window_SetMinSize },
				{ "GetMinSize",			EAPI_Window_GetMinSize },
				{ "SetPosition",		EAPI_Window_SetPosition },
				{ "GetPosition",		EAPI_Window_GetPosition },
				{ "SetIcon",			EAPI_Window_SetIcon },
				{ "Clear",				EAPI_Window_Clear },
				{ "Update",				EAPI_Window_Update },
			};

			std::vector<ParamEnum> enum_list = {
				{ "DEFAULT_POS",		WINDOW_POSDEFAULT },

				{ "MSGBOX_ERR",			WINDOW_MSGBOX_ERROR },
				{ "MSGBOX_WARN",		WINDOW_MSGBOX_WARNING },
				{ "MSGBOX_INFO",		WINDOW_MSGBOX_INFO },

				{ "FULLSCREEN",			WINDOW_FULLSCREEN },
				{ "BORDERLESS",			WINDOW_BORDERLESS },
				{ "RESIZABLE",			WINDOW_RESIZABLE },
				{ "MAXIMIZED",			WINDOW_MAXIMIZED },
				{ "MINIMIZED",			WINDOW_MINIMIZED },
				{ "WINDOWED",			WINDOW_WINDOWED },
				{ "FIXED",				WINDOW_FIXED },

				{ "CURSOR_ARROW",		WINDOW_CURSOR_ARROW },
				{ "CURSOR_IBEAM",		WINDOW_CURSOR_IBEAM },
				{ "CURSOR_WAIT",		WINDOW_CURSOR_WAIT },
				{ "CURSOR_CROSSHAIR",	WINDOW_CURSOR_CROSSHAIR },
				{ "CURSOR_WAITARROW",	WINDOW_CURSOR_WAITARROW },
				{ "CURSOR_SIZENWSE",	WINDOW_CURSOR_SIZENWSE },
				{ "CURSOR_SIZENESW",	WINDOW_CURSOR_SIZENESW },
				{ "CURSOR_SIZEWE",		WINDOW_CURSOR_SIZEWE },
				{ "CURSOR_SIZENS",		WINDOW_CURSOR_SIZENS },
				{ "CURSOR_SIZEALL",		WINDOW_CURSOR_SIZEALL },
				{ "CURSOR_NO},",		WINDOW_CURSOR_NO },
				{ "CURSOR_HAND",		WINDOW_CURSOR_HAND },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_HANDLEWINDOW
				},
				{
					METANAME_HANDLERENDERER
				}
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list, metatable_list);

			EE_SetPackageGCFunc(
				pLuaVM,
				[](lua_State* pLuaVM) -> int
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

					delete Window_pMsgBoxColorScheme;
					Window_pMsgBoxColorScheme = nullptr;

					for (auto& pair : Window_mapCursor)
					{
						SDL_FreeCursor(pair.second);
						pair.second = nullptr;
					}
					std::map<int, SDL_Cursor*>().swap(Window_mapCursor);

					return 0;
				});

			return 1;
		}
	},
	{ 
		"@Compress",		
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "CompressData", compressData },
				{ "DecompressData", decompressData },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list);

			return 1;
		}
	},
	{
		"@XML",
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "CreateEmpty", createEmpty },
				{ "LoadXML", loadXMLFromString },
				{ "LoadXMLFromFile", loadXMLFromFile },
				{ "LoadXMLFromData", loadXMLFromData },
			};

			std::vector<ParamEnum> enum_list = {
				{ "NODETYPE_EMPTY", NODETYPE_EMPTY },
				{ "NODETYPE_DOCUMENT", NODETYPE_DOCUMENT },
				{ "NODETYPE_ELEMENT", NODETYPE_ELEMENT },
				{ "NODETYPE_PLAINCHARADATA", NODETYPE_PLAINCHARADATA },
				{ "NODETYPE_CHARADATA", NODETYPE_CHARADATA },
				{ "NODETYPE_COMMENT", NODETYPE_COMMENT },
				{ "NODETYPE_PROCESSINS", NODETYPE_PROCESSINS },
				{ "NODETYPE_DECLARATION", NODETYPE_DECLARATION },
				{ "NODETYPE_DOCTYPE", NODETYPE_DOCTYPE },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_DOCUMENT,
					{
						{ "GetChild", document_GetChild },
						{ "AppendChild", document_AppendChild },
						{ "SaveAsFile", document_SaveAsFile },
						{ "Traverse", document_Traverse },
						{ "Print", document_Print },
					},
					__gc_Document
				},
				{
					METANAME_NODE,
					{
						{ "GetRoot", node_GetRoot },
						{ "GetParent", node_GetParent },
						{ "GetNextSibling", node_GetNextSibling },
						{ "GetPreviousSibling", node_GetPreviousSibling },
						{ "GetChild", node_GetChild },
						{ "GetFirstChild", node_GetFirstChild },
						{ "GetLastChild", node_GetLastChild },
						{ "GetChildren", node_GetChildren },
						{ "GetName", node_GetName },
						{ "GetValue", node_GetValue },
						{ "GetChildValue", node_GetChildValue },
						{ "GetType", node_GetType },
						{ "GetText", node_GetText },
						{ "GetTextAsInteger", node_GetTextAsInteger },
						{ "GetTextAsNumber", node_GetTextAsNumber },
						{ "GetTextAsBoolean", node_GetTextAsBoolean },
						{ "GetAttribute", node_GetAttribute },
						{ "GetFirstAttribute", node_GetFirstAttribute },
						{ "GetLastAttribute", node_GetLastAttribute },
						{ "GetAttributes", node_GetAttributes },
						{ "SetName", node_SetName },
						{ "SetValue", node_SetValue },
						{ "SetText", node_SetText },
						{ "SetTextAsInteger", node_SetTextAsInteger },
						{ "SetTextAsNumber", node_SetTextAsNumber },
						{ "SetTextAsBoolean", node_SetTextAsBoolean },
						{ "AppendAttribute", node_AppendAttribute },
						{ "PrependAttribute", node_PrependAttribute },
						{ "InsertAttributeAfter", node_InsertAttributeAfter },
						{ "InsertAttributeBefore", node_InsertAttributeBefore },
						{ "AppendAttributeCopy", node_AppendAttributeCopy },
						{ "PrependAttributeCopy", node_PrependAttributeCopy },
						{ "InsertAttributeCopyAfter", node_InsertAttributeCopyAfter },
						{ "InsertAttributeCopyBefore", node_InsertAttributeCopyBefore },
						{ "AppendChild", node_AppendChild },
						{ "MoveNodeToChildAppend", node_MoveNodeToChildAppend },
						{ "MoveNodeToChildPrepend", node_MoveNodeToChildPrepend },
						{ "MoveNodeToChildInsertAfter", node_MoveNodeToChildInsertAfter },
						{ "MoveNodeToChildInsertBefore", node_MoveNodeToChildInsertBefore },
						{ "RemoveAttribute", node_RemoveAttribute },
						{ "RemoveAttributes", node_RemoveAttributes },
						{ "RemoveChild", node_RemoveChild },
						{ "RemoveChildren", node_RemoveChildren },
						{ "FindDirectChild", node_FindDirectChild },
						{ "FindDescendedChild", node_FindDescendedChild },
						{ "FindAttribute", node_FindAttribute },
						{ "Traverse", node_Traverse },
						{ "Print", node_Print },
					},
					__gc_Node
				},
				{
					METANAME_ATTRIBUTE,
					{
						{ "GetName", attribute_GetName },
						{ "GetValue", attribute_GetValue },
						{ "GetValueAsInteger", attribute_GetValueAsInteger },
						{ "GetValueAsNumber", attribute_GetValueAsNumber },
						{ "GetValueAsBoolean", attribute_GetValueAsBoolean },
						{ "SetName", attribute_SetName },
						{ "SetValue", attribute_SetValue },
						{ "SetIntegerValue", attribute_SetIntegerValue },
						{ "SetNumberValue", attribute_SetNumberValue },
						{ "SetBooleanValue", attribute_SetBooleanValue },
						{ "GetNextAttribute", attribute_GetNextAttribute },
						{ "GetPreviousAttribute", attribute_GetPreviousAttribute },
					},
					__gc_Attribute
				}
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list, metatable_list);

			return 1;
		}
	},
	{
		"@Debug",
		[](lua_State* pLuaVM) -> int
		{
			std::vector<luaL_Reg> func_list = {
				{ "GetCoreError", EAPI_Debug_GetCoreError },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list);

			return 1;
		}
	}
};

#endif // !_REGISTER_H_
