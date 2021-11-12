#ifndef _STRING_H_
#define _STRING_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>

#include <string>

/*
* 将GBK编码字符串转换为UTF-8编码
* 1参数：GBK编码字符串（string）
* 1返回值：UTF-8编码字符串（string）
*/
ETHER_API int EAPI_String_GBKToUTF8(lua_State* pLuaVM);

/*
* 将UTF-8编码字符串转换为GBK编码
* 1参数：UTF-8编码字符串（string）
* 1返回值：GBK编码字符串（string）
*/
ETHER_API int EAPI_String_UTF8ToGBK(lua_State* pLuaVM);

/*
* 获取UTF-8编码字符串长度
* 1参数：UTF-8编码字符串（string）
* 1返回值：字符串长度（number）
*/
ETHER_API int EAPI_String_UTF8Len(lua_State* pLuaVM);

/*
* 截取UTF-8编码字符串
* 2或3参数：UTF-8编码字符串（string），开始位置（number），截取长度（number，默认截取至字符串结尾）
* 1返回值：截取后的字符串（string）
*/
ETHER_API int EAPI_String_UTF8Sub(lua_State* pLuaVM);

ETHER_API int EAPI_String_UTF8Find(lua_State* pLuaVM);

ETHER_API int EAPI_String_UTF8RFind(lua_State* pLuaVM);

ETHER_API int EAPI_String_UTF8Insert(lua_State* pLuaVM);

#endif // !_STRING_H_
