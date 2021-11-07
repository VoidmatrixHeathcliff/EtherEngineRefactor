#ifndef _STRING_H_
#define _STRING_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>

/*
* ��GBK�����ַ���ת��ΪUTF-8����
* 1������GBK�����ַ�����string��
* 1����ֵ��UTF-8�����ַ�����string��
*/
ETHER_API int gbkToUTF8(lua_State* L);

/*
* ��UTF-8�����ַ���ת��ΪGBK����
* 1������UTF-8�����ַ�����string��
* 1����ֵ��GBK�����ַ�����string��
*/
ETHER_API int utf8ToGBK(lua_State* L);

/*
* ��ȡUTF-8�����ַ���
* 2��3������UTF-8�����ַ�����string������ʼλ�ã�number��������λ�ã�number����ѡĬ��Ϊ-1��
* 1����ֵ����ȡ����ַ�����string��
*/
ETHER_API int subStrUTF8(lua_State* L);

/*
* ��ȡUTF-8�����ַ�������
* 1������UTF-8�����ַ�����string��
* 1����ֵ���ַ������ȣ�number��
*/
ETHER_API int lenUTF8(lua_State* L);

#endif // !_STRING_H_