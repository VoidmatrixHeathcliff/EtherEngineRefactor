#ifndef _TIME_H_
#define _TIME_H_

#include "Config.h"

#include <lua.hpp>
#include <SDL.h>

// ��ͣ����
// 0����
// 0����ֵ
ETHER_API int pause(lua_State* L);

// �������ָ��ʱ��
// 1����������ʱ�䣨number����λ�����룩
// 0����ֵ
ETHER_API int sleep(lua_State* L);

// ���Թ��������������ʱ����
// 2������������ʱ������number����λ�����룩���Ѿ��ȹ���ʱ�䣨number����λ�����룩
// 0����ֵ
ETHER_API int dynamicSleep(lua_State* L);

// ��ȡ����ӳ�ʼ�����е�����Ϊֹ��ʱ��
// 0����
// 1����ֵ������ʱ�䣨number����λ�����룩
ETHER_API int getInitTime(lua_State* L);

// ��ȡ�߷ֱ��ʼ�������ǰֵ
// 0����
// 1����ֵ����������ǰֵ��number��
ETHER_API int getAccurateCount(lua_State* L);

// ��ȡ�߷ֱ��ʼ�����Ƶ��
// 0����
// 1����ֵ��������Ƶ�ʣ�number����λ�����ȣ�
ETHER_API int getCounterFrequency(lua_State* L);

#endif // !_TIME_H_
