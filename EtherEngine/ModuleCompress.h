#ifndef _COMPRESS_H_
#define _COMPRESS_H_

#include "Module.h"

#include "zlib.h"
#include <lua.hpp>

#include <iostream>
using namespace std;

/*
* ѹ������
* 1��2������ԭʼ���ݣ�string����ѹ���ȼ�����ѡ����ΧΪ1-10��Ĭ��Ϊ6��
* 1����ֵ��ѹ�������ݣ�string��
* ��ע����ԭʼ������ͬ��һ������£�ѹ���ȼ�Խ�ߣ���ʱԽ����ѹ����ԽС��
* �ر�أ�ѹ���ȼ�Ϊ1ֻ�鵵����Ϊѹ����ʽ����������ѹ������
*/
ETHER_API compressData(lua_State* L);

/*
* ��ѹ����
* 1������ѹ�������ݣ�string��
* 1����ֵ��ԭʼ���ݣ�string��
*/
ETHER_API decompressData(lua_State* L);

class ModuleCompress : public Module<ModuleCompress>
{
	friend class Module<ModuleCompress>;

private:
	ModuleCompress()
	{
		_vCMethods = {
			{"CompressData", compressData},
			{"DecompressData", decompressData},
		};
	}

};

#endif // !_COMPRESS_H_
