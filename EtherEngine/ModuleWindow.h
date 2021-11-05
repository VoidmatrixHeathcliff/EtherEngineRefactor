#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "Module.h"
#include "Utils.h"
#include "ModuleGraphic.h"

#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>

#ifdef __WINDOWS__
#define  WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#include <tchar.h>
#else
#include ""
#endif

#define WINDOW_POSITION_DEFAULT INT_MIN

#define MSGBOX_ERROR					1001
#define MSGBOX_WARNING					1002
#define MSGBOX_INFO						1003

#define WINDOW_FULLSCREEN				1004
#define WINDOW_FULLSCREEN_DESKTOP		1005
#define WINDOW_BORDERLESS				1006
#define WINDOW_RESIZABLE				1007
#define WINDOW_MAXIMIZED				1008
#define WINDOW_MINIMIZED				1009

#define WINDOW_MODE_WINDOWED			1013
#define WINDOW_MODE_FULLSCREEN			1014
#define WINDOW_MODE_FULLSCREEN_DESKTOP	1015
#define WINDOW_MODE_BORDERLESS			1016
#define WINDOW_MODE_BORDERED			1017
#define WINDOW_MODE_RESIZABLE			1018
#define WINDOW_MODE_SIZEFIXED			1019

extern SDL_Window* pGlobalWindow;
extern SDL_Renderer* pGlobalRenderer;

// ��ʾģ̬����ʾ��Ϣ����
// 3�������������ͣ�Macro number������ʾ���ڱ��⣨string������ʾ��Ϣ���ݣ�string��
// 0����ֵ
ETHER_API showMessageBox(lua_State* L);

// ��ʾģ̬��ȷ����Ϣ����
// 3��5�������������ͣ�Macro number������ʾ���ڱ��⣨string������ʾ��Ϣ���ݣ�string����ȷ�ϰ�ť�ı���string����ѡ����ȡ����ť�ı���string����ѡ��
// 1����ֵ��ȷ���򷵻� true��boolean�������򷵻� false��boolean��
ETHER_API showConfirmMessageBox(lua_State* L);

// ��ʾ�ļ���ѡ����
// 1��������ʾ��Ϣ����
// 1����ʱ��ѡ��ɹ��򷵻��ļ���·����string�������򷵻�nil
ETHER_API showFolderSelector(lua_State* L);

// �������ڣ������˴�����Ϊͼ�λ��ƵȲ�����������
// 5���������ڱ��⣬����������ʾ����ľ��Σ����������������Եı�table��
// 0����ֵ
ETHER_API createWindow(lua_State* L);

// �رմ���
// 0����
// 0����ֵ
ETHER_API closeWindow(lua_State* L);

// ���ô��ڱ���
// 1���������ڱ��⣨string��
// 0����ֵ
ETHER_API setWindowTitle(lua_State* L);

// ��ȡ���ڱ���
// 0����
// 1����ֵ�����ڱ��⣨string��
ETHER_API getWindowTitle(lua_State* L);

// ���ô���ģʽ
// 1����������ģʽ��Macro number��
// 0����ֵ
ETHER_API setWindowMode(lua_State* L);

// ���ô���͸����
// 1������͸���ȴ�С��number��ȡֵ��Χ0-1��
// 0����ֵ
ETHER_API setWindowOpacity(lua_State* L);

// ���ô��ڴ�С
// 2���������ڿ�ȣ�number)�����ڸ߶ȣ�number��
// 0����ֵ
ETHER_API setWindowSize(lua_State* L);

// ��ȡ���ڴ�С
// 0����
// 2����ֵ�����ڿ�ȣ�number�������ڸ߶ȣ�number��
ETHER_API getWindowSize(lua_State* L);

// ���ô������ߴ�Ĵ�С
// 2��������������ȣ�number�����������߶ȣ�number��
// 0����ֵ
ETHER_API setWindowMaxSize(lua_State* L);

// ��ȡ���ڿ���Ⱦ�ߴ��С��֧�ָ� DPI��
// 0 ����
// 2����ֵ�����ڿ���Ⱦ��ȣ�number�������ڿ���Ⱦ�߶ȣ�number��
ETHER_API getWindowDrawableSize(lua_State* L);

// ��ȡ�������ߴ�Ĵ�С
// 0����
// 2����ֵ����������ȣ�number�����������߶ȣ�number��
ETHER_API getWindowMaxSize(lua_State* L);

// ���ô�����С�ߴ�Ĵ�С
// 2������������С��ȣ�number����������С�߶ȣ�number��
// 0����ֵ
ETHER_API setWindowMinSize(lua_State* L);

// ��ȡ������С�ߴ�Ĵ�С
// 0����
// 2����ֵ��������С��ȣ�number����������С�߶ȣ�number��
ETHER_API getWindowMinSize(lua_State* L);

// ���ô���λ��
// 1����������λ�����꣨table��
// 0����ֵ
ETHER_API setWindowPosition(lua_State* L);

// ��ȡ����λ��
// 0����
// 1����ֵ������λ�����꣨table��
ETHER_API getWindowPosition(lua_State* L);

// ���ô���ͼ�� (Windowsƽ̨�´��ڽ�����ʹ�ÿ�ִ���ļ�ͼ����Դ)
// 1������ͼ�����ݣ�userdata-IMAGE��
// 0����ֵ
ETHER_API setWindowIcon(lua_State* L);

// ʹ�õ�ǰ��ͼ��ɫ��մ�������
// 0����
// 0����ֵ
ETHER_API clearWindow(lua_State* L);

// ����Ⱦ������������ˢ�µ���Ļ��
// 0����
// 0����ֵ
ETHER_API updateWindow(lua_State* L);

class ModuleWindow : public Module<ModuleWindow>
{
	friend class Module<ModuleWindow>;

private:
	ModuleWindow()
	{
		_vCMethods = {
		{ "ShowMessageBox", showMessageBox},
		{ "ShowConfirmMessageBox", showConfirmMessageBox},
		{ "ShowFolderSelector", showFolderSelector},
		{ "CreateWindow", createWindow },
		{ "CloseWindow", closeWindow },
		{ "SetWindowTitle", setWindowTitle },
		{ "GetWindowTitle", getWindowTitle },
		{ "SetWindowMode", setWindowMode },
		{ "SetWindowOpacity", setWindowOpacity },
		{ "SetWindowSize", setWindowSize },
		{ "GetWindowSize", getWindowSize },
		{ "GetWindowDrawableSize", getWindowDrawableSize },
		{ "SetWindowMaxSize", setWindowMaxSize },
		{ "GetWindowMaxSize", getWindowMaxSize },
		{ "SetWindowMinSize", setWindowMinSize },
		{ "GetWindowMinSize", getWindowMinSize },
		{ "SetWindowPosition", setWindowPosition },
		{ "GetWindowPosition", getWindowPosition },
		{ "SetWindowIcon", setWindowIcon },
		{ "ClearWindow", clearWindow },
		{ "UpdateWindow", updateWindow },
		};

		_vMacros = {
			{ "WINDOW_POSITION_DEFAULT", WINDOW_POSITION_DEFAULT },

			{ "MSGBOX_ERROR", MSGBOX_ERROR },
			{ "MSGBOX_WARNING", MSGBOX_WARNING },
			{ "MSGBOX_INFO", MSGBOX_INFO },

			{ "WINDOW_FULLSCREEN", WINDOW_FULLSCREEN },
			{ "WINDOW_FULLSCREEN_DESKTOP", WINDOW_FULLSCREEN_DESKTOP },
			{ "WINDOW_BORDERLESS", WINDOW_BORDERLESS },
			{ "WINDOW_RESIZABLE", WINDOW_RESIZABLE },
			{ "WINDOW_MAXIMIZED", WINDOW_MAXIMIZED },
			{ "WINDOW_MINIMIZED", WINDOW_MINIMIZED },

			{ "WINDOW_MODE_WINDOWED", WINDOW_MODE_WINDOWED },
			{ "WINDOW_MODE_FULLSCREEN", WINDOW_MODE_FULLSCREEN },
			{ "WINDOW_MODE_FULLSCREEN_DESKTOP", WINDOW_MODE_FULLSCREEN_DESKTOP },
			{ "WINDOW_MODE_BORDERLESS", WINDOW_MODE_BORDERLESS },
			{ "WINDOW_MODE_BORDERED", WINDOW_MODE_BORDERED },
			{ "WINDOW_MODE_RESIZABLE", WINDOW_MODE_RESIZABLE },
			{ "WINDOW_MODE_SIZEFIXED", WINDOW_MODE_SIZEFIXED },
		};
	}
};

#endif // !_WINDOW_H_