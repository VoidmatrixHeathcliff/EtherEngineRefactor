#ifndef _TIME_H_
#define _TIME_H_

#include "Config.h"

#include <lua.hpp>
#include <SDL.h>

// �������ָ��ʱ��
// 1����������ʱ�䣨number����λ�����룩
// 0����ֵ
ETHER_API int EAPI_Time_Sleep(lua_State* pLuaVM);

// ��ȡ����ӳ�ʼ�����е�����Ϊֹ��ʱ��
// 0����
// 1����ֵ������ʱ�䣨number����λ�����룩
ETHER_API int EAPI_Time_GetInitTime(lua_State* pLuaVM);

// ��ȡ�߷ֱ��ʼ�������ǰֵ
// 0����
// 1����ֵ����������ǰֵ��number��
ETHER_API int EAPI_Time_GetPerformanceCounter(lua_State* pLuaVM);

// ��ȡ�߷ֱ��ʼ�����Ƶ��
// 0����
// 1����ֵ��������Ƶ�ʣ�number����λ�����ȣ�
ETHER_API int EAPI_Time_GetPerformanceFrequency(lua_State* pLuaVM);

#endif // !_TIME_H_
