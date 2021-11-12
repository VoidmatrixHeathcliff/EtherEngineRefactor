#ifndef _PACKAGE_GRAPHIC_H_
#define _PACKAGE_GRAPHIC_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>

#include <vector>

static const int GRAPHIC_FONT_BOLD			= 0;
static const int GRAPHIC_FONT_ITALIC		= 1;
static const int GRAPHIC_FONT_UNDERLINE		= 2;
static const int GRAPHIC_FONT_STRIKETHROUGH	= 3;
static const int GRAPHIC_FONT_NORMAL		= 4;

static const int GRAPHIC_FLIP_HORIZONTAL	= 0;
static const int GRAPHIC_FLIP_VERTICAL		= 1;
static const int GRAPHIC_FLIP_NONE			= 2;

static const int GRAPHIC_RENDER_NEAREST		= 0;
static const int GRAPHIC_RENDER_LINEAR		= 1;

extern SDL_Renderer* pGlobalRenderer;

ETHER_API int EAPI_Graphic_SetRenderMode(lua_State* pLuaVM);

// ���ļ��м���ͼ��
// 1������ͼ���ļ�·����string��
// 1����ֵ���ɹ��򷵻�ͼ�����ݣ�userdata-IMAGE����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_LoadImageFromFile(lua_State* pLuaVM);

// �ӻ������м���ͼ��
// 1���������������ݣ�string��
// 1����ֵ���ɹ��򷵻�ͼ�����ݣ�userdata-IMAGE����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_LoadImageFromBuffer(lua_State* pLuaVM);

// ͼ������GC����
// 1������ͼ�����ݣ�userdata-IMAGE��
// 0����ֵ
ETHER_API int EAPI_Graphic_Image_GC(lua_State* pLuaVM);

// ���õ�ͼƬ�ļ��Ƿ�����ָ����ColorKey�����õ�Color����͸����
// 3������ͼ�����ݣ�userdata-IMAGE�����Ƿ����ã�boolean����ColorKey��table��
// 0����ֵ
ETHER_API int EAPI_Graphic_Image_SetColorKey(lua_State* pLuaVM);

// ��ȡ�Ѽ���ͼ��ߴ�
// 1������ͼ�����ݣ�userdata-IMAGE��
// 2����ֵ��ͼ���ȣ�number����ͼ��߶ȣ�number��
ETHER_API int EAPI_Graphic_Image_Size(lua_State* pLuaVM);

// ��ͼ�����ݴ������ڴ��ڵĿ���Ⱦ����
// 1������ͼ�����ݣ�userdata-IMAGE��
// 1����ֵ���������ݣ�userdata-TEXTURE��
ETHER_API int EAPI_Graphic_CreateTexture(lua_State* pLuaVM);

// ��������GC����
// 1�������������ݣ�userdata-TEXTURE��
// 0����ֵ
ETHER_API int EAPI_Graphic_Texture_GC(lua_State* pLuaVM);

// ��������͸����
// 2�������������ݣ�userdata-TEXTURE����͸������ֵ��number��ȡֵ��Χ0-255��
// 0����ֵ
ETHER_API int EAPI_Graphic_Texture_SetAlpha(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_Texture_Size(lua_State* pLuaVM);

// ������������Ⱦ��������
// 2�������������ݣ�userdata-TEXTURE��������������ʾ����ľ��Σ�table��
// 0����ֵ
ETHER_API int EAPI_Graphic_RenderTexture(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_RenderTextureEx(lua_State* pLuaVM);

// ���ô��ڻ�ͼ��ɫ
// 1������������ɫ��RGBA��table��ȡֵ��Χ��Ϊ0-255��
// 0����ֵ
ETHER_API int EAPI_Graphic_SetDrawColor(lua_State* pLuaVM);

// ��ȡ���ڻ�ͼ��ɫ
// 0����
// 1����ֵ������������ɫ��RGBA��table��ȡֵ��Χ��Ϊ0-255��
ETHER_API int EAPI_Graphic_GetDrawColor(lua_State* pLuaVM);

// ��ָ��λ�û��Ƶ�
// 1�����������꣨table��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawPoint(lua_State* pLuaVM);

// ��ָ��λ�û���ֱ��
// 2������������꣨table�����յ����꣨table��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawLine(lua_State* pLuaVM);

// ��ָ��λ�û�����������
// 1����������������ʾ����ľ��Σ�table��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawRectangle(lua_State* pLuaVM);

// ��ָ��λ�û���Բ����������
// 2����������������ʾ����ľ��Σ�table����Բ�ǰ뾶��С��number��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawRoundRectangle(lua_State* pLuaVM);

// ��ָ��λ�û��������Բ��
// 2������Բ�����꣨table����Բ�뾶��number��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawCircle(lua_State* pLuaVM);

// ��ָ��λ�û����������Բ
// 3��������Բ�������꣨table������Բx��뾶��number������Բy��뾶��number��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawEllipse(lua_State* pLuaVM);

// ��ָ��λ�û������������
// 4��������������Բ��Բ�����꣨table������������Բ�İ뾶��number�������ο�ʼ�Ƕȣ�number�������ν����Ƕȣ�number�������δ���������Բ��Ϊԭ��Ķ�ά�ѿ�������ϵ��x��������ʼ���ƣ��Ƕȵ�������Ϊ˳ʱ�뷽��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawPie(lua_State* pLuaVM);

// ��ָ��λ�û��������������
// 6��������һ���������꣨table�����ڶ����������꣨table�����������������꣨table��
// 0����ֵ
ETHER_API int EAPI_Graphic_DrawTriangle(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_DrawPolygon(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_DrawBezier(lua_State* pLuaVM);

// ���ļ��м�������
// 2�����������ļ�·����string���������С��number��
// 1����ֵ���ɹ��򷵻��������ݣ�userdata-FONT����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_LoadFontFromFile(lua_State* pLuaVM);

// �ӻ������м�������
// 2���������������ݣ�string���������С��number��
// 1����ֵ���ɹ��򷵻��������ݣ�userdata-FONT����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_LoadFontFromBuffer(lua_State* pLuaVM);

// ��������GC����
// 1�������������ݣ�userdata-FONT��
// 0����ֵ
ETHER_API int EAPI_Graphic_Font_GC(lua_State* pLuaVM);

// ��ȡ�Ѽ����������ʽ
// 1�������������ݣ�userdata-FONT��
// 1����ֵ��nil
ETHER_API int EAPI_Graphic_Font_GetStyle(lua_State* pLuaVM);

// �����Ѽ����������ʽ
// 2�������������ݣ�userdata-FONT������������������ʽ�ı�table��
// 0����ֵ
ETHER_API int EAPI_Graphic_Font_SetStyle(lua_State* pLuaVM);

// ��ȡ�Ѽ�������������߿��
// 1�������������ݣ�userdata-FONT��
// 1����ֵ�������߿�ȣ�number��
ETHER_API int EAPI_Graphic_Font_GetOutlineWidth(lua_State* pLuaVM);

// �����Ѽ�������������߿��
// 2�������������ݣ�userdata-FONT���������߿�ȣ�number��
// 0����ֵ
ETHER_API int EAPI_Graphic_Font_SetOutlineWidth(lua_State* pLuaVM);

// ��ȡ������
// 1�������������ݣ�userdata-FONT��
// 1����ֵ�������ࣨnumber��
ETHER_API int EAPI_Graphic_Font_GetKerning(lua_State* pLuaVM);

// ����������
// 2�������������ݣ�userdata-FONT���������ࣨnumber��
// 0����ֵ
ETHER_API int EAPI_Graphic_Font_SetKerning(lua_State* pLuaVM);

// ��ȡ����߶�
// 1�������������ݣ�userdata-FONT��
// 1����ֵ������߶ȣ�number��
ETHER_API int EAPI_Graphic_Font_Height(lua_State* pLuaVM);

// ��ȡUTF-8�����ʽ���ı��ߴ�
// 2�������������ݣ�userdata-FONT����UTF-8�����ʽ���ı����ݣ�string��
// 2�������ı���ȣ�number�����ı��߶ȣ�number��
ETHER_API int EAPI_Graphic_GetTextSize(lua_State* pLuaVM);

// ʹ��Solidģʽ����UTF-8�����ʽ���ı�ͼ��
// 3�������������ݣ�userdata-FONT����UTF-8�����ʽ���ı����ݣ�string�������������ı���ɫ��RGBA��table��ȡֵ��Χ��Ϊ0-255��
// 1����ֵ���ɹ��򷵻�ͼ�����ݣ�userdata-IMAGE����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_TextImageFast(lua_State* pLuaVM);

// ʹ��Blendedģʽ����UTF-8�����ʽ���ı�ͼ��
// 3�������������ݣ�userdata-FONT����UTF-8�����ʽ���ı����ݣ�string�������������ı���ɫ��RGBA��table��ȡֵ��Χ��Ϊ0-255��
// 1����ֵ���ɹ��򷵻�ͼ�����ݣ�userdata-IMAGE����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_TextImageQuality(lua_State* pLuaVM);

// ʹ��Shadedģʽ����UTF-8�����ʽ���ı�ͼ��
// 3�������������ݣ�userdata-FONT����UTF-8�����ʽ���ı����ݣ�string�������������ı���ɫ��RGBA��table��ȡֵ��Χ��Ϊ0-255�������������ı�������ɫ��RGBA��table��ȡֵ��Χ��Ϊ0-255��
// 1����ֵ���ɹ��򷵻�ͼ�����ݣ�userdata-IMAGE����ʧ���򷵻�nil
ETHER_API int EAPI_Graphic_TextImageShaded(lua_State* pLuaVM);

#endif // !_PACKAGE_GRAPHIC_H_
