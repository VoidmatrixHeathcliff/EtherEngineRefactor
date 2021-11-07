#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>

static const int WINDOW_POSDEFAULT = SDL_WINDOWPOS_UNDEFINED;

static const int MSGBOX_ERROR	= 0;
static const int MSGBOX_WARNING = 1;
static const int MSGBOX_INFO	= 2;

static const int WINDOW_FULLSCREEN	= 0;
static const int WINDOW_BORDERLESS	= 1;
static const int WINDOW_RESIZABLE	= 2;
static const int WINDOW_MAXIMIZED	= 3;
static const int WINDOW_MINIMIZED	= 4;
static const int WINDOW_WINDOWED	= 5;
static const int WINDOW_FIXED		= 6;
static const int WINDOW_BORDERED	= 7;

extern SDL_Window* pGlobalWindow;
extern SDL_Renderer* pGlobalRenderer;

ETHER_API int EAPI_Window_GetWindowHandle(lua_State* pLuaVM);

ETHER_API int EAPI_Window_GetRendererHandle(lua_State* pLuaVM);

// 显示模态的提示信息窗口
// 3参数：窗口类型（Macro number），提示窗口标题（string），提示信息内容（string）
// 0返回值
ETHER_API int EAPI_Window_MessageBox(lua_State* L);

// 显示模态的确认信息窗口
// 3或5参数：窗口类型（Macro number），提示窗口标题（string），提示信息内容（string），确认按钮文本（string，可选），取消按钮文本（string，可选）
// 1返回值：确认则返回 true（boolean），否则返回 false（boolean）
ETHER_API int EAPI_Window_ConfirmBox(lua_State* L);

// 创建窗口，并将此窗口作为图形绘制等操作的上下文
// 5参数：窗口标题，用以描述显示区域的矩形，用以描述窗口属性的表（table）
// 0返回值
ETHER_API int EAPI_Window_Create(lua_State* L);

// 关闭窗口
// 0参数
// 0返回值
ETHER_API int EAPI_Window_Close(lua_State* L);

// 设置窗口标题
// 1参数：窗口标题（string）
// 0返回值
ETHER_API int EAPI_Window_SetTitle(lua_State* L);

// 获取窗口标题
// 0参数
// 1返回值：窗口标题（string）
ETHER_API int EAPI_Window_GetTitle(lua_State* L);

// 设置窗口模式
// 1参数：窗口模式（Macro number）
// 0返回值
ETHER_API int EAPI_Window_SetStyle(lua_State* L);

// 设置窗口透明度
// 1参数：透明度大小（number，取值范围0-1）
// 0返回值
ETHER_API int EAPI_Window_SetOpacity(lua_State* L);

// 设置窗口大小
// 2参数：窗口宽度（number)，窗口高度（number）
// 0返回值
ETHER_API int EAPI_Window_SetSize(lua_State* L);

// 获取窗口大小
// 0参数
// 2返回值：窗口宽度（number），窗口高度（number）
ETHER_API int EAPI_Window_GetSize(lua_State* L);

// 获取窗口可渲染尺寸大小（支持高 DPI）
// 0 参数
// 2返回值：窗口可渲染宽度（number），窗口可渲染高度（number）
ETHER_API int EAPI_Window_GetDrawableSize(lua_State* L);

// 设置窗口最大尺寸的大小
// 2参数：窗口最大宽度（number），窗口最大高度（number）
// 0返回值
ETHER_API int EAPI_Window_SetMaxSize(lua_State* L);

// 获取窗口最大尺寸的大小
// 0参数
// 2返回值：窗口最大宽度（number），窗口最大高度（number）
ETHER_API int EAPI_Window_GetMaxSize(lua_State* L);

// 设置窗口最小尺寸的大小
// 2参数：窗口最小宽度（number），窗口最小高度（number）
// 0返回值
ETHER_API int EAPI_Window_SetMinSize(lua_State* L);

// 获取窗口最小尺寸的大小
// 0参数
// 2返回值：窗口最小宽度（number），窗口最小高度（number）
ETHER_API int EAPI_Window_GetMinSize(lua_State* L);

// 设置窗口位置
// 1参数：窗口位置坐标（table）
// 0返回值
ETHER_API int EAPI_Window_SetPosition(lua_State* L);

// 获取窗口位置
// 0参数
// 1返回值：窗口位置坐标（table）
ETHER_API int EAPI_Window_GetPosition(lua_State* L);

// 设置窗口图标 (Windows平台下窗口将优先使用可执行文件图标资源)
// 1参数：图像数据（userdata-IMAGE）
// 0返回值
ETHER_API int EAPI_Window_SetIcon(lua_State* L);

// 使用当前绘图颜色清空窗口内容
// 0参数
// 0返回值
ETHER_API int EAPI_Window_Clear(lua_State* L);

// 将渲染缓冲区的内容刷新到屏幕上
// 0参数
// 0返回值
ETHER_API int EAPI_Window_Update(lua_State* L);

#endif // !_WINDOW_H_