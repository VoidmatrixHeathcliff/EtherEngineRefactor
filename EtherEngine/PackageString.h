#ifndef _STRING_H_
#define _STRING_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>

#include <string>

/*
* ��GBK�����ַ���ת��ΪUTF-8����
* 1������GBK�����ַ�����string��
* 1����ֵ��UTF-8�����ַ�����string��
*/
ETHER_API int EAPI_String_GBKToUTF8(lua_State* pLuaVM);

/*
* ��UTF-8�����ַ���ת��ΪGBK����
* 1������UTF-8�����ַ�����string��
* 1����ֵ��GBK�����ַ�����string��
*/
ETHER_API int EAPI_String_UTF8ToGBK(lua_State* pLuaVM);

/*
* ��ȡUTF-8�����ַ�������
* 1������UTF-8�����ַ�����string��
* 1����ֵ���ַ������ȣ�number��
*/
ETHER_API int EAPI_String_UTF8Len(lua_State* pLuaVM);

/*
* ��ȡUTF-8�����ַ���
* 2��3������UTF-8�����ַ�����string������ʼλ�ã�number������ȡ���ȣ�number��Ĭ�Ͻ�ȡ���ַ�����β��
* 1����ֵ����ȡ����ַ�����string��
*/
ETHER_API int EAPI_String_UTF8Sub(lua_State* pLuaVM);

ETHER_API int EAPI_String_UTF8Find(lua_State* pLuaVM);

ETHER_API int EAPI_String_UTF8RFind(lua_State* pLuaVM);

ETHER_API int EAPI_String_UTF8Insert(lua_State* pLuaVM);

#endif // !_STRING_H_
