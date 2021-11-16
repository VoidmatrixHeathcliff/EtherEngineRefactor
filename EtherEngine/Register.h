#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "Utils.h"
#include "PackageWindow.h"
#include "PackageGraphic.h"
#include "PackageMedia.h"
#include "PackageInput.h"
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
#include <SDL.h>

#include <vector>
#include <string>
#include <utility>
#include <functional>

SDL_Window*		pGlobalWindow = nullptr;
SDL_Renderer*	pGlobalRenderer = nullptr;

SDL_Event*		pGlobalEvent = nullptr;

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
			lua_createtable(pLuaVM, 0, (int)mt.func_list.size());
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

	lua_createtable(pLuaVM, 0, (int)(func_list.size() + enum_list.size()));

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
				{ "SetRenderMode",		EAPI_Graphic_SetRenderMode },
				{ "ImageFile",			EAPI_Graphic_LoadImageFromFile },
				{ "ImageBuffer",		EAPI_Graphic_LoadImageFromBuffer },
				{ "CreateTexture",		EAPI_Graphic_CreateTexture },
				{ "RenderTexture",		EAPI_Graphic_RenderTexture },
				{ "RenderTextureEx",	EAPI_Graphic_RenderTextureEx },
				{ "SetDrawColor",		EAPI_Graphic_SetDrawColor },
				{ "GetDrawColor",		EAPI_Graphic_GetDrawColor },
				{ "DrawPoint",			EAPI_Graphic_DrawPoint },
				{ "DrawLine",			EAPI_Graphic_DrawLine },
				{ "DrawRectangle",		EAPI_Graphic_DrawRectangle },
				{ "DrawRoundRectangle", EAPI_Graphic_DrawRoundRectangle },
				{ "DrawCircle",			EAPI_Graphic_DrawCircle },
				{ "DrawEllipse",		EAPI_Graphic_DrawEllipse },
				{ "DrawPie",			EAPI_Graphic_DrawPie },
				{ "DrawTriangle",		EAPI_Graphic_DrawTriangle },
				{ "DrawPolygon",		EAPI_Graphic_DrawPolygon },
				{ "DrawBezier",			EAPI_Graphic_DrawBezier },
				{ "FontFile",			EAPI_Graphic_LoadFontFromFile },
				{ "FontBuffer",			EAPI_Graphic_LoadFontFromBuffer },
				{ "GetTextSize",		EAPI_Graphic_GetTextSize },
				{ "TextImageFast",		EAPI_Graphic_TextImageFast },
				{ "TextImageQuality",	EAPI_Graphic_TextImageQuality },
				{ "TextImageShaded",	EAPI_Graphic_TextImageShaded },
			};

			std::vector<ParamEnum> enum_list = {
				{ "FLIP_HORIZONTAL",	GRAPHIC_FLIP_HORIZONTAL },
				{ "FLIP_VERTICAL",		GRAPHIC_FLIP_VERTICAL },
				{ "FLIP_NONE",			GRAPHIC_FLIP_NONE },

				{ "FONT_BOLD",			GRAPHIC_FONT_BOLD },
				{ "FONT_ITALIC",		GRAPHIC_FONT_ITALIC },
				{ "FONT_UNDERLINE",		GRAPHIC_FONT_UNDERLINE },
				{ "FONT_STRIKETHROUGH", GRAPHIC_FONT_STRIKETHROUGH },
				{ "FONT_NORMAL",		GRAPHIC_FONT_NORMAL },

				{ "RENDER_NEAREST",		GRAPHIC_RENDER_NEAREST },
				{ "RENDER_LINEAR",		GRAPHIC_RENDER_LINEAR },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_IMAGE,
					{
						{ "SetColorKey",		EAPI_Graphic_Image_SetColorKey },
						{ "Size",				EAPI_Graphic_Image_Size },
					},
					EAPI_Graphic_Image_GC
				},
				{
					METANAME_TEXTURE,
					{
						{ "SetAlpha",			EAPI_Graphic_Texture_SetAlpha },
						{ "Size",				EAPI_Graphic_Texture_Size },
					},
					EAPI_Graphic_Texture_GC
				},
				{
					METANAME_FONT,
					{
						{ "GetStyle",			EAPI_Graphic_Font_GetStyle },
						{ "SetStyle",			EAPI_Graphic_Font_SetStyle },
						{ "GetOutlineWidth",	EAPI_Graphic_Font_GetOutlineWidth },
						{ "SetOutlineWidth",	EAPI_Graphic_Font_SetOutlineWidth },
						{ "GetKerning",			EAPI_Graphic_Font_GetKerning },
						{ "SetKerning",			EAPI_Graphic_Font_SetKerning },
						{ "Height",				EAPI_Graphic_Font_Height },
					},
					EAPI_Graphic_Font_GC
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
		"@Input",
		[](lua_State* pLuaVM) -> int
		{
			pGlobalEvent = new SDL_Event();

			std::vector<luaL_Reg> func_list = {
				{ "UpdateEvent",		EAPI_Input_UpdateEvent },
				{ "StartTextInput",		EAPI_Input_StartTextInput },
				{ "StartTextInput",		EAPI_Input_StopTextInput },
				{ "GetEventType",		EAPI_Input_GetEventType },
				{ "GetMouseButtonID",	EAPI_Input_GetMouseButtonID },
				{ "GetKeyCode",			EAPI_Input_GetKeyCode },
				{ "GetCursorPosition",	EAPI_Input_GetCursorPosition },
				{ "GetWheelScroll",		EAPI_Input_GetWheelScroll },
				{ "GetInputText",		EAPI_Input_GetText },
				{ "GetDropFile",		EAPI_Input_GetDropFile },
			};

			std::vector<ParamEnum> enum_list = {
				{ "EVENT_QUIT",				SDL_QUIT },
				{ "EVENT_MOUSEMOTION",		SDL_MOUSEMOTION },
				{ "EVENT_MOUSEWHEEL",		SDL_MOUSEWHEEL },
				{ "EVENT_TEXTINPUT",		SDL_TEXTINPUT },
				{ "EVENT_KEYDOWN",			SDL_KEYDOWN },
				{ "EVENT_KEYUP",			SDL_KEYUP },
				{ "EVENT_DROPFILE",			SDL_DROPFILE },
				{ "EVENT_DROPBEGIN",		SDL_DROPBEGIN },
				{ "EVENT_DROPCOMPLETE",		SDL_DROPCOMPLETE },
				{ "EVENT_WINDOW",			SDL_WINDOWEVENT },
				{ "EVENT_MOUSEBTNDOWN",		SDL_MOUSEBUTTONDOWN },
				{ "EVENT_MOUSEBTNUP",		SDL_MOUSEBUTTONUP },

				{ "WINDOWEVENT_SHOWN",		SDL_WINDOWEVENT_SHOWN },
				{ "WINDOWEVENT_HIDDEN",		SDL_WINDOWEVENT_HIDDEN },
				{ "WINDOWEVENT_EXPOSED",	SDL_WINDOWEVENT_EXPOSED },
				{ "WINDOWEVENT_MOVED",		SDL_WINDOWEVENT_MOVED },
				{ "WINDOWEVENT_RESIZED",	SDL_WINDOWEVENT_RESIZED },
				{ "WINDOWEVENT_MINIMIZED",	SDL_WINDOWEVENT_MINIMIZED },
				{ "WINDOWEVENT_MAXIMIZED",	SDL_WINDOWEVENT_MAXIMIZED },
				{ "WINDOWEVENT_ENTER",		SDL_WINDOWEVENT_ENTER },
				{ "WINDOWEVENT_LEAVE",		SDL_WINDOWEVENT_LEAVE },
				{ "WINDOWEVENT_FOCUSGOT ",	SDL_WINDOWEVENT_FOCUS_GAINED },
				{ "WINDOWEVENT_FOCUSLOST",	SDL_WINDOWEVENT_FOCUS_LOST },
				{ "WINDOWEVENT_CLOSE",		SDL_WINDOWEVENT_CLOSE },

				{ "MOUSEBTN_LEFT",			SDL_BUTTON_LEFT },
				{ "MOUSEBTN_RIGHT",			SDL_BUTTON_RIGHT },
				{ "MOUSEBTN_MIDDLE",		SDL_BUTTON_MIDDLE },

				{ "KEY_0",					SDLK_0 },
				{ "KEY_1",					SDLK_1 },
				{ "KEY_2",					SDLK_2 },
				{ "KEY_3",					SDLK_3 },
				{ "KEY_4",					SDLK_4 },
				{ "KEY_5",					SDLK_5 },
				{ "KEY_6",					SDLK_6 },
				{ "KEY_7",					SDLK_7 },
				{ "KEY_8",					SDLK_8 },
				{ "KEY_9",					SDLK_9 },
				{ "KEYPAD_0",				SDLK_KP_0 },
				{ "KEYPAD_00",				SDLK_KP_00 },
				{ "KEYPAD_000",				SDLK_KP_000 },
				{ "KEYPAD_1",				SDLK_KP_1 },
				{ "KEYPAD_2",				SDLK_KP_2 },
				{ "KEYPAD_3",				SDLK_KP_3 },
				{ "KEYPAD_4",				SDLK_KP_4 },
				{ "KEYPAD_5",				SDLK_KP_5 },
				{ "KEYPAD_6",				SDLK_KP_6 },
				{ "KEYPAD_7",				SDLK_KP_7 },
				{ "KEYPAD_8",				SDLK_KP_8 },
				{ "KEYPAD_9",				SDLK_KP_9 },
				{ "KEY_F1",					SDLK_F1 },
				{ "KEY_F2",					SDLK_F2 },
				{ "KEY_F3",					SDLK_F3 },
				{ "KEY_F4",					SDLK_F4 },
				{ "KEY_F5",					SDLK_F5 },
				{ "KEY_F6",					SDLK_F6 },
				{ "KEY_F7",					SDLK_F7 },
				{ "KEY_F8",					SDLK_F8 },
				{ "KEY_F9",					SDLK_F9 },
				{ "KEY_F11",				SDLK_F11 },
				{ "KEY_F12",				SDLK_F12 },
				{ "KEY_F13",				SDLK_F13 },
				{ "KEY_F14",				SDLK_F14 },
				{ "KEY_F15",				SDLK_F15 },
				{ "KEY_F16",				SDLK_F16 },
				{ "KEY_F17",				SDLK_F17 },
				{ "KEY_F18",				SDLK_F18 },
				{ "KEY_F19",				SDLK_F19 },
				{ "KEY_F20",				SDLK_F20 },
				{ "KEY_F21",				SDLK_F21 },
				{ "KEY_F22",				SDLK_F22 },
				{ "KEY_F23",				SDLK_F23 },
				{ "KEY_F24",				SDLK_F24 },
				{ "KEY_A",					SDLK_a },
				{ "KEY_B",					SDLK_b },
				{ "KEY_C",					SDLK_n },
				{ "KEY_D",					SDLK_d },
				{ "KEY_E",					SDLK_e },
				{ "KEY_F",					SDLK_f },
				{ "KEY_G",					SDLK_g },
				{ "KEY_H",					SDLK_h },
				{ "KEY_I",					SDLK_i },
				{ "KEY_J",					SDLK_j },
				{ "KEY_K",					SDLK_k },
				{ "KEY_L",					SDLK_l },
				{ "KEY_M",					SDLK_m },
				{ "KEY_N",					SDLK_n },
				{ "KEY_O",					SDLK_o },
				{ "KEY_P",					SDLK_p },
				{ "KEY_Q",					SDLK_q },
				{ "KEY_R",					SDLK_r },
				{ "KEY_S",					SDLK_s },
				{ "KEY_T",					SDLK_t },
				{ "KEY_U",					SDLK_u },
				{ "KEY_V",					SDLK_v },
				{ "KEY_W",					SDLK_w },
				{ "KEY_X",					SDLK_x },
				{ "KEY_Y",					SDLK_y },
				{ "KEY_Z",					SDLK_z },
				{ "KEYPAD_A",				SDLK_KP_A },
				{ "KEYPAD_B",				SDLK_KP_B },
				{ "KEYPAD_C",				SDLK_KP_C },
				{ "KEYPAD_D",				SDLK_KP_D },
				{ "KEYPAD_E",				SDLK_KP_E },
				{ "KEYPAD_F",				SDLK_KP_F },
				{ "KEY_ESC",				SDLK_ESCAPE },
				{ "KEY_ENTER",				SDLK_RETURN },
				{ "KEYPAD_ENTER",			SDLK_KP_ENTER },
				{ "KEY_BACKSPACE",			SDLK_BACKSPACE },
				{ "KEYPAD_BACKSPACE",		SDLK_KP_BACKSPACE },
				{ "KEY_RIGHT",				SDLK_RIGHT },
				{ "KEY_LEFT",				SDLK_LEFT },
				{ "KEY_DOWN",				SDLK_DOWN },
				{ "KEY_UP",					SDLK_UP },
				{ "KEY_INSERT",				SDLK_INSERT },
				{ "KEY_DELETE",				SDLK_DELETE },
				{ "KEY_HOME",				SDLK_HOME },
				{ "KEY_END",				SDLK_END },
				{ "KEY_PAGEUP",				SDLK_PAGEUP },
				{ "KEY_PAGEDOWN",			SDLK_PAGEDOWN },
				{ "KEY_LCTRL",				SDLK_LCTRL },
				{ "KEY_LGUI",				SDLK_LGUI },
				{ "KEY_LALT",				SDLK_LALT },
				{ "KEY_LSHIFT",				SDLK_LSHIFT },
				{ "KEY_RCTRL",				SDLK_RCTRL },
				{ "KEY_RGUI",				SDLK_RGUI },
				{ "KEY_RALT",				SDLK_RALT },
				{ "KEY_RSHIFT",				SDLK_RSHIFT },
				{ "KEY_SPACE",				SDLK_SPACE },
				{ "KEY_TAB",				SDLK_TAB },
				{ "KEYPAD_TAB",				SDLK_KP_TAB },
				{ "KEY_CAPSLOCK",			SDLK_CAPSLOCK },
				{ "KEY_NUMLOCK",			SDLK_NUMLOCKCLEAR },
				{ "KEY_PRINTSCREEN",		SDLK_PRINTSCREEN },
				{ "KEY_SCROLLLOCK",			SDLK_SCROLLLOCK },
				{ "KEY_PAUSE",				SDLK_PAUSE },
				{ "KEY_AUDIOMUTE",			SDLK_AUDIOMUTE },
				{ "KEY_AUDIOPREV",			SDLK_AUDIOPREV },
				{ "KEY_AUDIONEXT",			SDLK_AUDIONEXT },
				{ "KEY_AUDIOPLAY",			SDLK_AUDIOPLAY },
				{ "KEY_AUDIOSTOP",			SDLK_AUDIOSTOP },
				{ "KEY_VOLUMEUP",			SDLK_VOLUMEUP },
				{ "KEY_VOLUMEDOWN",			SDLK_VOLUMEDOWN },
				{ "KEY_BRIGHTNESSUP",		SDLK_BRIGHTNESSUP },
				{ "KEY_BRIGHTNESSDOWN",		SDLK_BRIGHTNESSDOWN },
				{ "KEY_BACKQUOTE",			SDLK_BACKQUOTE },
				{ "KEY_EXCLAIM",			SDLK_EXCLAIM },
				{ "KEYPAD_EXCLAIM",			SDLK_KP_EXCLAM },
				{ "KEY_AT",					SDLK_AT },
				{ "KEYPAD_AT",				SDLK_KP_AT },
				{ "KEY_HASH",				SDLK_HASH },
				{ "KEYPAD_HASH",			SDLK_KP_HASH },
				{ "KEY_DOLLAR",				SDLK_DOLLAR },
				{ "KEY_CARET",				SDLK_CARET },
				{ "KEYPAD_CARET",			SDLK_KP_POWER },
				{ "KEY_AMPERSAND",			SDLK_AMPERSAND },
				{ "KEYPAD_AMPERSAND",		SDLK_KP_AMPERSAND },
				{ "KEYPAD_DBLAMPERSAND",	SDLK_KP_DBLAMPERSAND },
				{ "KEY_ASTERISK",			SDLK_ASTERISK },
				{ "KEYPAD_ASTERISK",		SDLK_KP_MULTIPLY },
				{ "KEY_LPAREN",				SDLK_LEFTPAREN },
				{ "KEYPAD_LPAREN",			SDLK_KP_LEFTPAREN },
				{ "KEY_RPAREN",				SDLK_RIGHTPAREN },
				{ "KEYPAD_RPAREN",			SDLK_KP_RIGHTPAREN },
				{ "KEY_MINUS",				SDLK_MINUS },
				{ "KEYPAD_MINUS",			SDLK_KP_MINUS },
				{ "KEY_UNDERSCORE",			SDLK_UNDERSCORE },
				{ "KEY_PLUS",				SDLK_PLUS },
				{ "KEYPAD_PLUS",			SDLK_KP_PLUS },
				{ "KEY_EQUALS",				SDLK_EQUALS },
				{ "KEYPAD_EQUALS",			SDLK_KP_EQUALS },
				{ "KEY_LBRACKET",			SDLK_LEFTBRACKET },
				{ "KEY_RBRACKET",			SDLK_RIGHTBRACKET },
				{ "KEYPAD_LBRACE",			SDLK_KP_LEFTBRACE },
				{ "KEYPAD_RBRACE",			SDLK_KP_RIGHTBRACE },
				{ "KEY_COLON",				SDLK_COLON },
				{ "KEY_SEMICOLON",			SDLK_SEMICOLON },
				{ "KEY_BACKSLASH",			SDLK_BACKSLASH },
				{ "KEY_QUOTE",				SDLK_QUOTE },
				{ "KEY_DBLQUOTE",			SDLK_QUOTEDBL },
				{ "KEY_LESS",				SDLK_LESS },
				{ "KEY_GREATER",			SDLK_GREATER },
				{ "KEY_COMMA",				SDLK_COMMA },
				{ "KEY_PERIOD",				SDLK_PERIOD },
				{ "KEY_QUESTION",			SDLK_QUESTION },
				{ "KEY_SLASH",				SDLK_SLASH },
				{ "KEY_VERTICALBAR",		SDLK_KP_VERTICALBAR },
				{ "KEYPAD_DBLVERTICALBAR",	SDLK_KP_DBLVERTICALBAR },
				{ "KEY_WWW",				SDLK_WWW },
				{ "KEY_MAIL",				SDLK_MAIL },
			};

			EE_PushBuiltinPackageData(pLuaVM, func_list, enum_list);

			EE_SetPackageGCFunc(
				pLuaVM,
				[](lua_State* pLuaVM) -> int
				{
					delete pGlobalEvent; pGlobalEvent = nullptr;

					return 0;
				});

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
				{ "MusicFile",			EAPI_Media_LoadMusicFromFile },
				{ "MusicBuffer",		EAPI_Media_LoadMusicFromBuffer },
				{ "PlayMusic",			EAPI_Media_PlayMusic },
				{ "StopMusic",			EAPI_Media_StopMusic },
				{ "SetMusicPosition",	EAPI_Media_SetMusicPosition },
				{ "GetMusicVolume",		EAPI_Media_GetMusicVolume },
				{ "PauseMusic",			EAPI_Media_PauseMusic },
				{ "ResumeMusic",		EAPI_Media_ResumeMusic },
				{ "RewindMusic",		EAPI_Media_RewindMusic },
				{ "CheckMusicPlaying",	EAPI_Media_CheckMusicPlaying },
				{ "CheckMusicPaused",	EAPI_Media_CheckMusicPaused },
				{ "GetMusicFadingType",	EAPI_Media_GetMusicFadingType },
				{ "SoundFile",			EAPI_Media_LoadSoundFromFile },
				{ "SoundBuffer",		EAPI_Media_LoadSoundFromBuffer },
			};

			std::vector<ParamEnum> enum_list = {
				{ "MUSIC_WAV",		MEDIA_MUSIC_WAV },
				{ "MUSIC_MP3",		MEDIA_MUSIC_MP3 },
				{ "MUSIC_OGG",		MEDIA_MUSIC_OGG },
				{ "MUSIC_CMD",		MEDIA_MUSIC_CMD },
				{ "MUSIC_MOD",		MEDIA_MUSIC_MOD },
				{ "MUSIC_MID",		MEDIA_MUSIC_MID },
				{ "MUSIC_UNKNOWN",	MEDIA_MUSIC_UNKNOWN },
			};

			std::vector<MetaTableData> metatable_list = {
				{
					METANAME_MUSIC,
					{
						{ "Type",		EAPI_Media_Music_Type },
					},
					EAPI_Media_Music_GC
				},
				{
					METANAME_SOUND,
					{
						{ "Play",		EAPI_Media_Sound_Play },
						{ "SetVolume",	EAPI_Media_Sound_SetVolume },
						{ "GetVolume",	EAPI_Media_Sound_GetVolume },
					},
					EAPI_Media_Sound_GC
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
				{ "GBKToUTF8",	EAPI_String_GBKToUTF8 },
				{ "UTF8ToGBK",	EAPI_String_UTF8ToGBK },
				{ "UTF8Len",	EAPI_String_UTF8Len },
				{ "UTF8Sub",	EAPI_String_UTF8Sub },
				{ "UTF8Find",	EAPI_String_UTF8Find },
				{ "UTF8RFind",	EAPI_String_UTF8RFind },
				{ "UTF8Insert", EAPI_String_UTF8Insert },
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
				{ "Sleep",						EAPI_Time_Sleep },
				{ "GetInitTime",				EAPI_Time_GetInitTime },
				{ "GetPerformanceCounter",		EAPI_Time_GetPerformanceCounter },
				{ "GetPerformanceFrequency",	EAPI_Time_GetPerformanceFrequency },
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

			Window_pMapCursor = new std::map<int, SDL_Cursor*>();
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_ARROW, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_IBEAM, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_WAIT, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_CROSSHAIR, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_SIZENWSE, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_SIZENESW, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_SIZEWE, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_SIZENS, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_SIZEALL, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_NO, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO)));
			Window_pMapCursor->insert(std::make_pair(WINDOW_CURSOR_HAND, SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)));

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
				{ "CURSOR_NO",			WINDOW_CURSOR_NO },
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

					for (auto& pair : *Window_pMapCursor)
					{
						SDL_FreeCursor(pair.second);
						pair.second = nullptr;
					}
					delete Window_pMapCursor;

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
