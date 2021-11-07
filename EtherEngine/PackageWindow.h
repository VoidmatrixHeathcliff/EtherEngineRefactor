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

// ��ʾģ̬����ʾ��Ϣ����
// 3�������������ͣ�Macro number������ʾ���ڱ��⣨string������ʾ��Ϣ���ݣ�string��
// 0����ֵ
ETHER_API int EAPI_Window_MessageBox(lua_State* L);

// ��ʾģ̬��ȷ����Ϣ����
// 3��5�������������ͣ�Macro number������ʾ���ڱ��⣨string������ʾ��Ϣ���ݣ�string����ȷ�ϰ�ť�ı���string����ѡ����ȡ����ť�ı���string����ѡ��
// 1����ֵ��ȷ���򷵻� true��boolean�������򷵻� false��boolean��
ETHER_API int EAPI_Window_ConfirmBox(lua_State* L);

// �������ڣ������˴�����Ϊͼ�λ��ƵȲ�����������
// 5���������ڱ��⣬����������ʾ����ľ��Σ����������������Եı�table��
// 0����ֵ
ETHER_API int EAPI_Window_Create(lua_State* L);

// �رմ���
// 0����
// 0����ֵ
ETHER_API int EAPI_Window_Close(lua_State* L);

// ���ô��ڱ���
// 1���������ڱ��⣨string��
// 0����ֵ
ETHER_API int EAPI_Window_SetTitle(lua_State* L);

// ��ȡ���ڱ���
// 0����
// 1����ֵ�����ڱ��⣨string��
ETHER_API int EAPI_Window_GetTitle(lua_State* L);

// ���ô���ģʽ
// 1����������ģʽ��Macro number��
// 0����ֵ
ETHER_API int EAPI_Window_SetStyle(lua_State* L);

// ���ô���͸����
// 1������͸���ȴ�С��number��ȡֵ��Χ0-1��
// 0����ֵ
ETHER_API int EAPI_Window_SetOpacity(lua_State* L);

// ���ô��ڴ�С
// 2���������ڿ�ȣ�number)�����ڸ߶ȣ�number��
// 0����ֵ
ETHER_API int EAPI_Window_SetSize(lua_State* L);

// ��ȡ���ڴ�С
// 0����
// 2����ֵ�����ڿ�ȣ�number�������ڸ߶ȣ�number��
ETHER_API int EAPI_Window_GetSize(lua_State* L);

// ��ȡ���ڿ���Ⱦ�ߴ��С��֧�ָ� DPI��
// 0 ����
// 2����ֵ�����ڿ���Ⱦ��ȣ�number�������ڿ���Ⱦ�߶ȣ�number��
ETHER_API int EAPI_Window_GetDrawableSize(lua_State* L);

// ���ô������ߴ�Ĵ�С
// 2��������������ȣ�number�����������߶ȣ�number��
// 0����ֵ
ETHER_API int EAPI_Window_SetMaxSize(lua_State* L);

// ��ȡ�������ߴ�Ĵ�С
// 0����
// 2����ֵ����������ȣ�number�����������߶ȣ�number��
ETHER_API int EAPI_Window_GetMaxSize(lua_State* L);

// ���ô�����С�ߴ�Ĵ�С
// 2������������С��ȣ�number����������С�߶ȣ�number��
// 0����ֵ
ETHER_API int EAPI_Window_SetMinSize(lua_State* L);

// ��ȡ������С�ߴ�Ĵ�С
// 0����
// 2����ֵ��������С��ȣ�number����������С�߶ȣ�number��
ETHER_API int EAPI_Window_GetMinSize(lua_State* L);

// ���ô���λ��
// 1����������λ�����꣨table��
// 0����ֵ
ETHER_API int EAPI_Window_SetPosition(lua_State* L);

// ��ȡ����λ��
// 0����
// 1����ֵ������λ�����꣨table��
ETHER_API int EAPI_Window_GetPosition(lua_State* L);

// ���ô���ͼ�� (Windowsƽ̨�´��ڽ�����ʹ�ÿ�ִ���ļ�ͼ����Դ)
// 1������ͼ�����ݣ�userdata-IMAGE��
// 0����ֵ
ETHER_API int EAPI_Window_SetIcon(lua_State* L);

// ʹ�õ�ǰ��ͼ��ɫ��մ�������
// 0����
// 0����ֵ
ETHER_API int EAPI_Window_Clear(lua_State* L);

// ����Ⱦ������������ˢ�µ���Ļ��
// 0����
// 0����ֵ
ETHER_API int EAPI_Window_Update(lua_State* L);

#endif // !_WINDOW_H_