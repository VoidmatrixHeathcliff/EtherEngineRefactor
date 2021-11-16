#ifndef _UTILS_H_
#define _UTILS_H_

#include "Config.h"

#include <lua.hpp>
#include <SDL.h>

#include <codecvt>
#include <locale>
#include <string>
#include <functional>

void EE_CheckPoint(lua_State* pLuaVM, int idx, SDL_Point& point);
void EE_CheckRect(lua_State* pLuaVM, int idx, SDL_Rect& rect);
void EE_CheckColor(lua_State* pLuaVM, int idx, SDL_Color& color);

template <class T>
inline T* EE_ToUserdata(lua_State* pLuaVM, int idx, 
	const char* type_name)
{
	return (T*)(*(void**)luaL_checkudata(pLuaVM, idx, type_name));
}

template <class T>
inline void EE_PushUserdata(lua_State* pLuaVM, T* obj, 
	const char* type_name)
{
	T** ppUserdata = (T**)lua_newuserdata(pLuaVM, sizeof(T*));
	*ppUserdata = obj;
	luaL_getmetatable(pLuaVM, type_name);
	lua_setmetatable(pLuaVM, -2);
}

inline void EE_CheckPoint(lua_State* pLuaVM, int idx, SDL_Point& point)
{
	luaL_argexpected(pLuaVM, lua_istable(pLuaVM, idx), idx, LUA_TABLIBNAME);

	lua_pushstring(pLuaVM, "x"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : x").c_str());
	point.x = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "y"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : y").c_str());
	point.y = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);
}

inline void EE_CheckRect(lua_State* pLuaVM, int idx, SDL_Rect& rect)
{
	luaL_argexpected(pLuaVM, lua_istable(pLuaVM, idx), idx, LUA_TABLIBNAME);

	lua_pushstring(pLuaVM, "x"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : x").c_str());
	rect.x = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "y"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : y").c_str());
	rect.y = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "w"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : w").c_str());
	rect.w = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "h"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : h").c_str());
	rect.h = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);
}

inline void EE_CheckColor(lua_State* pLuaVM, int idx, SDL_Color& color)
{
	luaL_argexpected(pLuaVM, lua_istable(pLuaVM, idx), idx, LUA_TABLIBNAME);

	lua_pushstring(pLuaVM, "r"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : r").c_str());
	color.r = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "g"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : g").c_str());
	color.g = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "b"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : b").c_str());
	color.b = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);

	lua_pushstring(pLuaVM, "a"); lua_rawget(pLuaVM, idx);
	luaL_argcheck(pLuaVM, lua_isnumber(pLuaVM, -1),
		idx, std::string(ERRMSG_INVALIDMEMBER).append(" : a").c_str());
	color.a = (int)lua_tonumber(pLuaVM, -1);
	lua_pop(pLuaVM, 1);
}

// 'key' at index - 2 and 'value' at index - 1
inline void EE_TraverseTable(lua_State* pLuaVM, int idx, 
	std::function<bool()> callback)
{
	bool _flag = true;
	lua_pushnil(pLuaVM);
	while (_flag && lua_next(pLuaVM, idx)) {
		_flag = callback();
		lua_pop(pLuaVM, 1);
	}
}

class EncodingConversion
{
public:
	EncodingConversion() = delete;
	EncodingConversion(const EncodingConversion&) = delete;
	EncodingConversion(EncodingConversion&&) = delete;
	~EncodingConversion() = delete;
	EncodingConversion& operator=(const EncodingConversion&) = delete;
	EncodingConversion& operator=(EncodingConversion&&) = delete;

	static std::string    ToString(const std::wstring& wstr);
	static std::wstring   ToWString(const std::string& str);
	static std::string    GBKToUTF8(const std::string& str);
	static std::string    UTF8ToGBK(const std::string& str);
	static std::u16string UTF8toUTF16(const std::string& str);
	static std::u32string UTF8toUTF32(const std::string& str);
	static std::string    UTF16toUTF8(const std::u16string& str);
	static std::u32string UTF16toUTF32(const std::u16string& str);
	static std::string    UTF32toUTF8(const std::u32string& str);
	static std::u16string UTF32toUTF16(const std::u32string& str);
private:
	static std::string    ToGBK(const std::wstring& wstr);
	static std::wstring   FromGBK(const std::string& str);
	static std::string    ToUTF8(const std::wstring& wstr);
	static std::wstring   FromUTF8(const std::string& str);
private:
	static const char* GBK_LOCALE_NAME;
};

#endif // !_UTILS_H_

