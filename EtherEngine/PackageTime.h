#ifndef _TIME_H_
#define _TIME_H_

#include "Config.h"

#include <lua.hpp>
#include <SDL.h>

// 挂起程序指定时间
// 1参数：挂起时间（number，单位：毫秒）
// 0返回值
ETHER_API int EAPI_Time_Sleep(lua_State* pLuaVM);

// 获取程序从初始化运行到现在为止的时间
// 0参数
// 1返回值：运行时间（number，单位：毫秒）
ETHER_API int EAPI_Time_GetInitTime(lua_State* pLuaVM);

// 获取高分辨率计数器当前值
// 0参数
// 1返回值：计数器当前值（number）
ETHER_API int EAPI_Time_GetPerformanceCounter(lua_State* pLuaVM);

// 获取高分辨率计数器频率
// 0参数
// 1返回值：计数器频率（number，单位：赫兹）
ETHER_API int EAPI_Time_GetPerformanceFrequency(lua_State* pLuaVM);

#endif // !_TIME_H_
