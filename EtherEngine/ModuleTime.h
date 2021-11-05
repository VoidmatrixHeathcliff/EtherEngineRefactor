#ifndef _TIME_H_
#define _TIME_H_

#include "Module.h"
#include "Macros.h"

#include <lua.hpp>
#include <SDL.h>

// ��ͣ����
// 0����
// 0����ֵ
ETHER_API pause(lua_State* L);

// �������ָ��ʱ��
// 1����������ʱ�䣨number����λ�����룩
// 0����ֵ
ETHER_API sleep(lua_State* L);

// ���Թ��������������ʱ����
// 2������������ʱ������number����λ�����룩���Ѿ��ȹ���ʱ�䣨number����λ�����룩
// 0����ֵ
ETHER_API dynamicSleep(lua_State* L);

// ��ȡ����ӳ�ʼ�����е�����Ϊֹ��ʱ��
// 0����
// 1����ֵ������ʱ�䣨number����λ�����룩
ETHER_API getInitTime(lua_State* L);

// ��ȡ�߷ֱ��ʼ�������ǰֵ
// 0����
// 1����ֵ����������ǰֵ��number��
ETHER_API getAccurateCount(lua_State* L);

// ��ȡ�߷ֱ��ʼ�����Ƶ��
// 0����
// 1����ֵ��������Ƶ�ʣ�number����λ�����ȣ�
ETHER_API getCounterFrequency(lua_State* L);

class ModuleTime : public Module<ModuleTime>
{
	friend class Module<ModuleTime>;

private:
	ModuleTime()
	{
		_vCMethods = {
		{ "Pause", pause },
		{ "Sleep", sleep },
		{ "DynamicSleep", dynamicSleep },
		{ "GetInitTime", getInitTime },
		{ "GetAccurateCount", getAccurateCount },
		{ "GetCounterFrequency", getCounterFrequency },
		};
	}

};

#endif // !_TIME_H_