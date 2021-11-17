#include "PackageJSON.h"

bool JSON_bStrictArray = false;

void EE_ConvertJSONToLuaObject(cJSON* pJSONNode, lua_State* pLuaVM)
{
	switch (pJSONNode->type)
	{
	case cJSON_False:
		lua_pushboolean(pLuaVM, false);
		break;
	case cJSON_True:
		lua_pushboolean(pLuaVM, true);
		break;
	case cJSON_Number:
		lua_pushnumber(pLuaVM, pJSONNode->valuedouble);
		break;
	case cJSON_String:
		lua_pushstring(pLuaVM, pJSONNode->valuestring);
		break;
	case cJSON_Array:
	{
		lua_createtable(pLuaVM, cJSON_GetArraySize(pJSONNode), 0);
		cJSON* _pElement = nullptr; int _idx = 1;
		cJSON_ArrayForEach(_pElement, pJSONNode)
		{
			lua_pushinteger(pLuaVM, _idx++);
			EE_ConvertJSONToLuaObject(_pElement, pLuaVM);
			lua_rawset(pLuaVM, -3);
		}
	}
	break;
	case cJSON_Object:
	{
		lua_createtable(pLuaVM, 0, cJSON_GetArraySize(pJSONNode));
		cJSON* _pElement = nullptr;
		cJSON_ArrayForEach(_pElement, pJSONNode)
		{
			lua_pushstring(pLuaVM, _pElement->string);
			EE_ConvertJSONToLuaObject(_pElement, pLuaVM);
			lua_rawset(pLuaVM, -3);
		}
	}
	break;
	default:
		lua_pushnil(pLuaVM);
		break;
	}
}

cJSON* EE_ConvertLuaObjectToJSON(int idx, lua_State* pLuaVM)
{
	cJSON* _pJSONNode = nullptr;
	switch (lua_type(pLuaVM, idx))
	{
	case LUA_TNIL:
		_pJSONNode = cJSON_CreateNull();
		break;
	case LUA_TBOOLEAN:
		_pJSONNode = cJSON_CreateBool(lua_toboolean(pLuaVM, idx));
		break;
	case LUA_TNUMBER:
		_pJSONNode = cJSON_CreateNumber(lua_tonumber(pLuaVM, idx));
		break;
	case LUA_TSTRING:
		_pJSONNode = cJSON_CreateString(lua_tostring(pLuaVM, idx));
		break;
	case LUA_TTABLE:
	{
		if (JSON_bStrictArray)
		{
			bool _hasStringKey = false;
			EE_TraverseTable(
				pLuaVM, idx,
				[&]()->bool {
					return !(_hasStringKey = (lua_type(pLuaVM, -2) == LUA_TSTRING));
				}
			);
			_pJSONNode = _hasStringKey ? cJSON_CreateObject() : cJSON_CreateArray();
		}
		else
			_pJSONNode = cJSON_CreateObject();
		if (_pJSONNode->type == cJSON_Array)
			EE_TraverseTable(
				pLuaVM, idx,
				[&]()->bool {
					cJSON_AddItemToArray(_pJSONNode, EE_ConvertLuaObjectToJSON(-1, pLuaVM));
					return true;
				}
		);
		else
			EE_TraverseTable(
				pLuaVM, idx,
				[&]()->bool {
					std::string _key;
					switch (lua_type(pLuaVM, -2))
					{
					case LUA_TSTRING:
						_key = lua_tostring(pLuaVM, -2);
						break;
					case LUA_TNUMBER:
						_key = lua_isinteger(pLuaVM, -2) ? std::to_string(lua_tointeger(pLuaVM, -2)) : std::to_string(lua_tonumber(pLuaVM, -2));
						break;
					default:
						luaL_argerror(pLuaVM, 1, std::string("JSON dump failed key type: ").append(lua_typename(pLuaVM, -2)).c_str());
						break;
					}
						
					cJSON_AddItemToObject(_pJSONNode, _key.c_str(), EE_ConvertLuaObjectToJSON(-1, pLuaVM));
					return true;
				}
			);
	}
		break;
	default:
		luaL_argerror(pLuaVM, 1, std::string("JSON dump failed value type: ").append(lua_typename(pLuaVM, idx)).c_str());
		break;
	}
	return _pJSONNode;
}

ETHER_API int EAPI_JSON_Load(lua_State* pLuaVM)
{
	cJSON* _pJSONRoot = cJSON_Parse(luaL_checkstring(pLuaVM, 1));
	luaL_argcheck(pLuaVM, _pJSONRoot, 1, std::string("JSON load failed before: ").append(cJSON_GetErrorPtr()).c_str());
	EE_ConvertJSONToLuaObject(_pJSONRoot, pLuaVM);
	cJSON_Delete(_pJSONRoot);

	return 1;
}

ETHER_API int EAPI_JSON_Dump(lua_State* pLuaVM)
{
	cJSON* _pJSONRoot = EE_ConvertLuaObjectToJSON(1, pLuaVM);
	char* _strJSON = lua_toboolean(pLuaVM, 2) ? cJSON_Print(_pJSONRoot) : cJSON_PrintUnformatted(_pJSONRoot);
	lua_pushstring(pLuaVM, _strJSON);
	cJSON_Delete(_pJSONRoot); free(_strJSON);

	return 1;
}

ETHER_API int EAPI_JSON_SetStrictArrayMode(lua_State* pLuaVM)
{
	JSON_bStrictArray = lua_toboolean(pLuaVM, 1);

	return 0;
}

ETHER_API int EAPI_JSON_GetStrictArrayMode(lua_State* pLuaVM)
{
	lua_pushboolean(pLuaVM, JSON_bStrictArray);

	return 1;
}