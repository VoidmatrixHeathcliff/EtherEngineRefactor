#include "PackageString.h"

ETHER_API int EAPI_String_GBKToUTF8(lua_State* pLuaVM)
{
	try
	{
		lua_pushstring(pLuaVM, 
			EncodingConversion::GBKToUTF8(luaL_checkstring(pLuaVM, 1)).c_str());
	}
	catch (const std::exception&)
	{
		lua_pushnil(pLuaVM);
	}

	return 1;
}

ETHER_API int EAPI_String_UTF8ToGBK(lua_State* pLuaVM)
{
	try
	{
		lua_pushstring(pLuaVM, 
			EncodingConversion::UTF8ToGBK(luaL_checkstring(pLuaVM, 1)).c_str());
	}
	catch (const std::exception&)
	{
		lua_pushnil(pLuaVM);
	}

	return 1;
}

ETHER_API int EAPI_String_UTF8Len(lua_State* pLuaVM)
{
	std::string _Str = luaL_checkstring(pLuaVM, 1);

	size_t _idx = 0, _len = 0;
	while (_Str[_idx])
		_len += ((_Str[_idx++] & 0xc0) != 0x80);
	lua_pushinteger(pLuaVM, _len);

	return 1;
}

ETHER_API int EAPI_String_UTF8Sub(lua_State* pLuaVM)
{
	std::string _Str = luaL_checkstring(pLuaVM, 1);
	size_t _Off = (int)luaL_checknumber(pLuaVM, 2) <= 0 ? 0 : (int)lua_tonumber(pLuaVM, 2) - 1;
	size_t _Count = lua_gettop(pLuaVM) > 2 ? (int)luaL_checknumber(pLuaVM, 3) : std::string::npos;

	size_t _idx_utf8 = 0, _idx_begin_raw = 0;
	while (_idx_utf8 < _Off && _Str[_idx_begin_raw])
		_idx_utf8 += ((_Str[_idx_begin_raw++] & 0xc0) != 0x80);
	while ((_Str[_idx_begin_raw] & 0xc0) == 0x80 && _Str[_idx_begin_raw])
		_idx_begin_raw++;
	if (_Count == std::string::npos)
		lua_pushstring(pLuaVM, _Str.substr(_idx_begin_raw).c_str());
	else
	{
		size_t _idx_end_raw = _idx_begin_raw;
		while (_idx_utf8 < _Off + _Count && _Str[_idx_end_raw])
			_idx_utf8 += ((_Str[_idx_end_raw++] & 0xc0) != 0x80);
		while ((_Str[_idx_end_raw] & 0xc0) == 0x80 && _Str[_idx_end_raw])
			_idx_end_raw++;
		lua_pushstring(pLuaVM, _Str.substr(_idx_begin_raw, _idx_end_raw - _idx_begin_raw).c_str());
	}

	return 1;
}

ETHER_API int EAPI_String_UTF8Find(lua_State* pLuaVM)
{
	std::string _Str = luaL_checkstring(pLuaVM, 1);
	std::string _Dst = luaL_checkstring(pLuaVM, 2);
	size_t _Off = lua_gettop(pLuaVM) > 2 ? 
		((int)luaL_checknumber(pLuaVM, 3) <= 0 ? 0 : (int)lua_tonumber(pLuaVM, 3) - 1) : 0;

	size_t _idx_utf8 = 0, _idx_raw = 0;
	while (_idx_utf8 < _Off && _Str[_idx_raw])
		_idx_utf8 += ((_Str[_idx_raw++] & 0xc0) != 0x80);
	while ((_Str[_idx_raw] & 0xc0) == 0x80 && _Str[_idx_raw]) _idx_raw++;
	size_t _idx_dst_raw = _Str.find(_Dst, _idx_raw);
	if (_idx_dst_raw == std::string::npos) 
		lua_pushnil(pLuaVM);
	else
	{
		while (_idx_raw < _idx_dst_raw)
			_idx_utf8 += ((_Str[_idx_raw++] & 0xc0) != 0x80);
		lua_pushinteger(pLuaVM, _idx_utf8);
	}

	return 1;
}

ETHER_API int EAPI_String_UTF8RFind(lua_State* pLuaVM)
{
	std::string _Str = luaL_checkstring(pLuaVM, 1);
	std::string _Dst = luaL_checkstring(pLuaVM, 2);
	size_t _Off = lua_gettop(pLuaVM) > 2 ?
		((int)luaL_checknumber(pLuaVM, 3) <= 0 ? 0 : (int)lua_tonumber(pLuaVM, 3) - 1) : 0;

	size_t _idx_utf8 = 0, _idx_raw = 0;
	while (_idx_utf8 < _Off && _Str[_idx_raw])
		_idx_utf8 += ((_Str[_idx_raw++] & 0xc0) != 0x80);
	while ((_Str[_idx_raw] & 0xc0) == 0x80 && _Str[_idx_raw]) _idx_raw++;
	size_t _idx_dst_raw = _Str.rfind(_Dst, _idx_raw);
	if (_idx_dst_raw == std::string::npos)
		lua_pushnil(pLuaVM);
	else
	{
		while (_idx_raw < _idx_dst_raw)
			_idx_utf8 += ((_Str[_idx_raw++] & 0xc0) != 0x80);
		lua_pushinteger(pLuaVM, _idx_utf8);
	}

	return 1;
}

ETHER_API int EAPI_String_UTF8Insert(lua_State* pLuaVM)
{
	std::string _Str = luaL_checkstring(pLuaVM, 1);
	std::string _Ins = luaL_checkstring(pLuaVM, 2);
	size_t _Off = lua_gettop(pLuaVM) > 2 ?
		((int)luaL_checknumber(pLuaVM, 3) <= 0 ? 0 : (int)lua_tonumber(pLuaVM, 3) - 1) : 0;

	size_t _idx_utf8 = 0, _idx_raw = 0;
	while (_idx_utf8 < _Off && _Str[_idx_raw])
		_idx_utf8 += ((_Str[_idx_raw++] & 0xc0) != 0x80);
	while ((_Str[_idx_raw] & 0xc0) == 0x80 && _Str[_idx_raw]) _idx_raw++;
	lua_pushstring(pLuaVM, _Str.insert(_idx_raw, _Ins).c_str());

	return 1;
}