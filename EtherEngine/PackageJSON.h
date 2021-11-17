#ifndef _JSON_H_
#define _JSON_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>
#include <cJSON.h>

#include <string>

extern bool JSON_bStrictArray;

/*
* 将已编码的 JSON 对象解码为 Lua 对象
* 1参数：已编码的 JSON 对象字符串（string）
* 1返回值：解码后的 Lua 对象（nil / boolean / number / string / table）
*/
ETHER_API int EAPI_JSON_Load(lua_State* pLuaVM);

/*
* 将 Lua 对象编码成 JSON 字符串
* 1或2参数：Lua 对象（nil / boolean / number / string / table），【可选】是否对编码后的字符串格式化，默认不进行格式化（boolean）
* 1返回值：已编码的 JSON 对象字符串（string）
*/
ETHER_API int EAPI_JSON_Dump(lua_State* pLuaVM);

ETHER_API int EAPI_JSON_SetStrictArrayMode(lua_State* pLuaVM);

ETHER_API int EAPI_JSON_GetStrictArrayMode(lua_State* pLuaVM);

#endif // !_JSON_H_
