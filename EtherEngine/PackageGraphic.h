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

// 从文件中加载图像
// 1参数：图像文件路径（string）
// 1返回值：成功则返回图像数据（userdata-IMAGE），失败则返回nil
ETHER_API int EAPI_Graphic_LoadImageFromFile(lua_State* pLuaVM);

// 从缓冲区中加载图像
// 1参数：缓冲区数据（string）
// 1返回值：成功则返回图像数据（userdata-IMAGE），失败则返回nil
ETHER_API int EAPI_Graphic_LoadImageFromBuffer(lua_State* pLuaVM);

// 图像数据GC函数
// 1参数：图像数据（userdata-IMAGE）
// 0返回值
ETHER_API int EAPI_Graphic_Image_GC(lua_State* pLuaVM);

// 设置的图片文件是否启用指定的ColorKey，启用的Color将被透明化
// 3参数：图像数据（userdata-IMAGE），是否启用（boolean），ColorKey（table）
// 0返回值
ETHER_API int EAPI_Graphic_Image_SetColorKey(lua_State* pLuaVM);

// 获取已加载图像尺寸
// 1参数：图像数据（userdata-IMAGE）
// 2返回值：图像宽度（number），图像高度（number）
ETHER_API int EAPI_Graphic_Image_Size(lua_State* pLuaVM);

// 从图像数据创建属于窗口的可渲染纹理
// 1参数：图像数据（userdata-IMAGE）
// 1返回值：纹理数据（userdata-TEXTURE）
ETHER_API int EAPI_Graphic_Image_Texture(lua_State* pLuaVM);

// 纹理数据GC函数
// 1参数：纹理数据（userdata-TEXTURE）
// 0返回值
ETHER_API int EAPI_Graphic_Texture_GC(lua_State* pLuaVM);

// 设置纹理透明度
// 2参数：纹理数据（userdata-TEXTURE），透明度数值（number，取值范围0-255）
// 0返回值
ETHER_API int EAPI_Graphic_Texture_SetAlpha(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_Texture_Size(lua_State* pLuaVM);

// 将纹理拷贝至渲染缓冲区内
// 2参数：纹理数据（userdata-TEXTURE），用以描述显示区域的矩形（table）
// 0返回值
ETHER_API int EAPI_Graphic_RenderTexture(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_RenderTextureEx(lua_State* pLuaVM);

// 设置窗口绘图颜色
// 1参数：描述颜色的RGBA表（table，取值范围均为0-255）
// 0返回值
ETHER_API int EAPI_Graphic_SetDrawColor(lua_State* pLuaVM);

// 获取窗口绘图颜色
// 0参数
// 1返回值：用以描述颜色的RGBA表（table，取值范围均为0-255）
ETHER_API int EAPI_Graphic_GetDrawColor(lua_State* pLuaVM);

// 在指定位置绘制点
// 1参数：点坐标（table）
// 0返回值
ETHER_API int EAPI_Graphic_DrawPoint(lua_State* pLuaVM);

// 在指定位置绘制直线
// 2参数：起点坐标（table），终点坐标（table）
// 0返回值
ETHER_API int EAPI_Graphic_DrawLine(lua_State* pLuaVM);

// 在指定位置绘制无填充矩形
// 1参数：用以描述显示区域的矩形（table）
// 0返回值
ETHER_API int EAPI_Graphic_DrawRectangle(lua_State* pLuaVM);

// 在指定位置绘制圆角无填充矩形
// 2参数：用以描述显示区域的矩形（table），圆角半径大小（number）
// 0返回值
ETHER_API int EAPI_Graphic_DrawRoundRectangle(lua_State* pLuaVM);

// 在指定位置绘制无填充圆形
// 2参数：圆心坐标（table），圆半径（number）
// 0返回值
ETHER_API int EAPI_Graphic_DrawCircle(lua_State* pLuaVM);

// 在指定位置绘制无填充椭圆
// 3参数：椭圆中心坐标（table），椭圆x轴半径（number），椭圆y轴半径（number）
// 0返回值
ETHER_API int EAPI_Graphic_DrawEllipse(lua_State* pLuaVM);

// 在指定位置绘制无填充扇形
// 4参数：扇形所在圆的圆心坐标（table），扇形所在圆的半径（number），扇形开始角度（number），扇形结束角度（number）（扇形从以其所在圆心为原点的二维笛卡尔坐标系的x轴正方向开始绘制，角度的正方向为顺时针方向）
// 0返回值
ETHER_API int EAPI_Graphic_DrawPie(lua_State* pLuaVM);

// 在指定位置绘制无填充三角形
// 6参数：第一个顶点坐标（table），第二个顶点坐标（table），第三个顶点坐标（table）
// 0返回值
ETHER_API int EAPI_Graphic_DrawTriangle(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_DrawPolygon(lua_State* pLuaVM);

ETHER_API int EAPI_Graphic_DrawBezier(lua_State* pLuaVM);

// 从文件中加载字体
// 2参数：字体文件路径（string）、字体大小（number）
// 1返回值：成功则返回字体数据（userdata-FONT），失败则返回nil
ETHER_API int EAPI_Graphic_LoadFontFromFile(lua_State* pLuaVM);

// 从缓冲区中加载字体
// 2参数：缓冲区数据（string）、字体大小（number）
// 1返回值：成功则返回字体数据（userdata-FONT），失败则返回nil
ETHER_API int EAPI_Graphic_LoadFontFromBuffer(lua_State* pLuaVM);

// 字体数据GC函数
// 1参数：字体数据（userdata-FONT）
// 0返回值
ETHER_API int EAPI_Graphic_Font_GC(lua_State* pLuaVM);

// 获取已加载字体的样式
// 1参数：字体数据（userdata-FONT）
// 1返回值：nil
ETHER_API int EAPI_Graphic_Font_GetStyle(lua_State* pLuaVM);

// 设置已加载字体的样式
// 2参数：字体数据（userdata-FONT），用以描述字体样式的表（table）
// 0返回值
ETHER_API int EAPI_Graphic_Font_SetStyle(lua_State* pLuaVM);

// 获取已加载字体的轮廓线宽度
// 1参数：字体数据（userdata-FONT）
// 1返回值：轮廓线宽度（number）
ETHER_API int EAPI_Graphic_Font_GetOutlineWidth(lua_State* pLuaVM);

// 设置已加载字体的轮廓线宽度
// 2参数：字体数据（userdata-FONT），轮廓线宽度（number）
// 0返回值
ETHER_API int EAPI_Graphic_Font_SetOutlineWidth(lua_State* pLuaVM);

// 获取字体间距
// 1参数：字体数据（userdata-FONT）
// 1返回值：字体间距（number）
ETHER_API int EAPI_Graphic_Font_GetKerning(lua_State* pLuaVM);

// 设置字体间距
// 2参数：字体数据（userdata-FONT），字体间距（number）
// 0返回值
ETHER_API int EAPI_Graphic_Font_SetKerning(lua_State* pLuaVM);

// 获取字体高度
// 1参数：字体数据（userdata-FONT）
// 1返回值：字体高度（number）
ETHER_API int EAPI_Graphic_Font_Height(lua_State* pLuaVM);

// 获取UTF-8编码格式的文本尺寸
// 2参数：字体数据（userdata-FONT），UTF-8编码格式的文本内容（string）
// 2参数：文本宽度（number），文本高度（number）
ETHER_API int EAPI_Graphic_GetTextSize(lua_State* pLuaVM);

// 使用Solid模式创建UTF-8编码格式的文本图像
// 3参数：字体数据（userdata-FONT），UTF-8编码格式的文本内容（string），用以描述文本颜色的RGBA表（table，取值范围均为0-255）
// 1返回值：成功则返回图像数据（userdata-IMAGE），失败则返回nil
ETHER_API int EAPI_Graphic_TextImageFast(lua_State* pLuaVM);

// 使用Blended模式创建UTF-8编码格式的文本图像
// 3参数：字体数据（userdata-FONT），UTF-8编码格式的文本内容（string），用以描述文本颜色的RGBA表（table，取值范围均为0-255）
// 1返回值：成功则返回图像数据（userdata-IMAGE），失败则返回nil
ETHER_API int EAPI_Graphic_TextImageQuality(lua_State* pLuaVM);

// 使用Shaded模式创建UTF-8编码格式的文本图像
// 3参数：字体数据（userdata-FONT），UTF-8编码格式的文本内容（string），用以描述文本颜色的RGBA表（table，取值范围均为0-255），用以描述文本背景颜色的RGBA表（table，取值范围均为0-255）
// 1返回值：成功则返回图像数据（userdata-IMAGE），失败则返回nil
ETHER_API int EAPI_Graphic_TextImageShaded(lua_State* pLuaVM);

#endif // !_PACKAGE_GRAPHIC_H_
