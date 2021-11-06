#ifndef _MODULE_H_
#define _MODULE_H_

#include "Macros.h"

#include <lua.hpp>

#include <string>
#include <vector>
using namespace std;

template <class T>
inline T* _ToUserdata(lua_State* L, int idx, const char* type_name)
{
	return (T*)(*(void**)luaL_checkudata(L, idx, type_name));
}

template <class T>
inline void _PushUserdata(lua_State* L, T* obj, const char* type_name)
{
	T** ppUserdata = (T**)lua_newuserdata(L, sizeof(T*));
	*ppUserdata = obj;
	luaL_getmetatable(L, type_name);
	lua_setmetatable(L, -2);
}

#endif // !_MODULE_H_
