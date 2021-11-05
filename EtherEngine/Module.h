#ifndef _MODULE_H_
#define _MODULE_H_

#include "Macros.h"

#include <lua.hpp>

#include <string>
#include <vector>
using namespace std;

struct MetaData
{
	string name;
	vector<luaL_Reg> cFuns;
	lua_CFunction gcFun = [](lua_State* L) -> int { return 0; };
};

template<typename T>
class Module
{
public:
	static T& Instance()
	{
		static T* _instance = new T();
		return *_instance;
	}
	virtual ~Module() {};
	void PushMetaDataToGlobal(lua_State* L)
	{
		for (MetaData metadata : _vMetaData)
		{
			luaL_newmetatable(L, metadata.name.c_str());

			lua_pushstring(L, "__index");
			lua_newtable(L);
			for (luaL_Reg method : metadata.cFuns)
			{
				lua_pushstring(L, method.name);
				lua_pushcfunction(L, method.func);
				lua_settable(L, -3);
			}
			lua_settable(L, -3);

			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, metadata.gcFun);
			lua_settable(L, -3);
		}
	}
	void PushMoudleDataToStack(lua_State* L)
	{
		lua_newtable(L);

		for (luaL_Reg method : _vCMethods)
		{
			lua_pushstring(L, method.name);
			lua_pushcfunction(L, method.func);
			lua_settable(L, -3);
		}

		for (Macro macro : _vMacros)
		{
			lua_pushstring(L, macro.name);
			lua_pushinteger(L, macro.value);
			lua_settable(L, -3);
		}
	}

protected:
	Module() {};
	vector<luaL_Reg> _vCMethods;
	vector<Macro> _vMacros;
	vector<MetaData> _vMetaData;
	
};

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
