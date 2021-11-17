#ifndef _JSON_H_
#define _JSON_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>
#include <cJSON.h>

#include <string>

extern bool JSON_bStrictArray;

/*
* ���ѱ���� JSON �������Ϊ Lua ����
* 1�������ѱ���� JSON �����ַ�����string��
* 1����ֵ�������� Lua ����nil / boolean / number / string / table��
*/
ETHER_API int EAPI_JSON_Load(lua_State* pLuaVM);

/*
* �� Lua �������� JSON �ַ���
* 1��2������Lua ����nil / boolean / number / string / table��������ѡ���Ƿ�Ա������ַ�����ʽ����Ĭ�ϲ����и�ʽ����boolean��
* 1����ֵ���ѱ���� JSON �����ַ�����string��
*/
ETHER_API int EAPI_JSON_Dump(lua_State* pLuaVM);

ETHER_API int EAPI_JSON_SetStrictArrayMode(lua_State* pLuaVM);

ETHER_API int EAPI_JSON_GetStrictArrayMode(lua_State* pLuaVM);

#endif // !_JSON_H_
