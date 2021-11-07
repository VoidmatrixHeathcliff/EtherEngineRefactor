#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>
#include <SDL.h>
#include <base64.h>

#include <math.h>
#include <string>
using namespace std;

struct ColorHSLA
{
	double h;
	double s;
	double l;
	double a;
};

double HLS2RGBvalue(double n1, double n2, double hue);

// ����ֵ������ָ������
// 1��3���������޶�ֵ��number�������޺����ޣ�number��Ĭ��Ϊ0��1��
// 1����ֵ���޶���ֵ
ETHER_API int clamp(lua_State* L);

// �жϵ��Ƿ�λ��ָ������������(�������α߽磩
// 2��3�����������꣨table������������table�����Ƿ�����߽磨boolean����ѡ��Ĭ��Ϊ true��
// 1����ֵ���Ƿ�λ�������ڣ�boolean��
ETHER_API int checkPointInRect(lua_State* L);

// �жϵ��Ƿ�λ��ָ��Բ��������(����Բ�α߽磩
// 3��4�����������꣨table����Բ�����꣨table����Բ�뾶��number�����Ƿ�����߽磨boolean����ѡ��Ĭ��Ϊ true��
// 1����ֵ���Ƿ�λ�������ڣ�boolean��
ETHER_API int checkPointInCircle(lua_State* L);

// �ж������������Ƿ��ཻ���������α߽磩
// 2��3����������1����table��������2����table�����Ƿ�����߽磨boolean����ѡ��Ĭ��Ϊ true��
// 1����ֵ���Ƿ��ཻ��boolean��
ETHER_API int checkRectsOverlap(lua_State* L);

// �ж���Բ�Ƿ��ཻ������Բ�α߽磩
// 4��5������Բ1Բ�����꣨table����Բ1�뾶��number����Բ2Բ�����꣨table����Բ2�뾶��number�����Ƿ�����߽磨boolean����ѡ��Ĭ��Ϊ true��
// 1����ֵ���Ƿ��ཻ��boolean��
ETHER_API int checkCirclesOverlap(lua_State* L);

// ��ȡ�����ֱ�߾���
// 2��������1���꣨table������2���꣨table��
// 1����ֵ�������ֱ�߾��루number��
ETHER_API int getPointsDistance(lua_State* L);

// ��ȡ��ֱ�߼����
// 2������ֱ��1������꣨table����ֱ��1�յ����꣨table����ֱ��2������꣨table����ֱ��2�յ����꣨table��
// 1����ֵ����ֱ�߼���루number��
ETHER_API int getLinesDistance(lua_State* L);

// ��ȡ�㵽ֱ�ߵĴ�ֱ����
// 3�����������꣨table����ֱ��������꣨table����ֱ���յ����꣨table��
// 1����ֵ���㵽ֱ�ߵľ��루number��
ETHER_API int getPointLineDistance(lua_State* L);

// RGBA��ɫ�ռ�תHSLA��ɫ�ռ�
// 1������ʹ��RGBA��������ɫ��table��
// 1����ֵ��ʹ��HSLA��������ɫ��table��
ETHER_API int rgbaToHSLA(lua_State* L);

// HSLA��ɫ�ռ�תRGBA��ɫ�ռ�
// 1������ʹ��HSLA��������ɫ��table��
// 1����ֵ��ʹ��RGBA��������ɫ��table��
ETHER_API int hslaToRGBA(lua_State* L);

// �����������ݱ���Ϊ Base64 ������ַ���
// 1���������������ݣ�string��
// 1����ֵ��Base64 ������ַ�����string��
ETHER_API int encodeBase64(lua_State* L);

// �� Base64 ������ַ�������Ϊ����������
// 1������Base64 ������ַ�����string��
// 1����ֵ�����������ݣ�string��
ETHER_API int decodeBase64(lua_State* L);

#endif // !_ALGORITHM_H_
