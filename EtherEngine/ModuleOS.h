#ifndef _OS_H_
#define _OS_H_

#include "Module.h"
#include "Macros.h"

#include <lua.hpp>
#include <SDL.h>

#include <string>
using namespace std;

#ifdef __WINDOWS__
#define  WIN32_LEAN_AND_MEAN
#include <shlobj.h>
#include <tchar.h>
#include <io.h>
#else
#include ""
#endif

#define FILEATTRIB_ARCH						0
#define FILEATTRIB_HIDDEN					1
#define FILEATTRIB_NORMAL					2
#define FILEATTRIB_RDONLY					3
#define FILEATTRIB_SUBDIR					4
#define FILEATTRIB_SYSTEM					5

#define PATHMODE_FILE						6
#define PATHMODE_DIR						7
#define PATHMODE_FILEANDDIR					8

#define POWERSTATE_UNKOWN					9
#define POWERSTATE_ONBATTERY				10
#define POWERSTATE_NOBATTERY				11
#define POWERSTATE_CHARGING					12
#define POWERSTATE_CHARGEDN					13

#define PATHATTRIB_DESKTOP					14          
#define PATHATTRIB_INTERNET					15          
#define PATHATTRIB_PROGRAMS					16          
#define PATHATTRIB_CONTROLS					17          
#define PATHATTRIB_PRINTERS					18          
#define PATHATTRIB_DOCUMENTS				19           
#define PATHATTRIB_FAVORITES				20          
#define PATHATTRIB_STARTUP					21          
#define PATHATTRIB_RECENT					22          
#define PATHATTRIB_SENDTO					23          
#define PATHATTRIB_RECYCLEBIN				24           
#define PATHATTRIB_STARTMENU				25                    
#define PATHATTRIB_MUSIC					26        
#define PATHATTRIB_VIDEO					27              
#define PATHATTRIB_DRIVES					28          
#define PATHATTRIB_NETWORK					29          
#define PATHATTRIB_NETHOOD					30          
#define PATHATTRIB_FONTS					31          
#define PATHATTRIB_TEMPLATES				32          
#define PATHATTRIB_COMMON_STARTMENU			33       
#define PATHATTRIB_COMMON_PROGRAMS			34  
#define PATHATTRIB_COMMON_STARTUP			35  
#define PATHATTRIB_COMMON_DESKTOP			36
#define PATHATTRIB_APPDATA					37  
#define PATHATTRIB_PRINTHOOD				38  
#define PATHATTRIB_LOCAL_APPDATA			39
#define PATHATTRIB_COMMON_FAVORITES			40
#define PATHATTRIB_INTERNET_CACHE			41
#define PATHATTRIB_COOKIES					42
#define PATHATTRIB_HISTORY					43
#define PATHATTRIB_COMMON_APPDATA			44
#define PATHATTRIB_WINDOWS					45
#define PATHATTRIB_SYSTEM					46
#define PATHATTRIB_PROGRAM_FILES			47
#define PATHATTRIB_PICTURES					48
#define PATHATTRIB_PROFILE					49 
#define PATHATTRIB_SYSTEMX86				50
#define PATHATTRIB_PROGRAM_FILESX86			51
#define PATHATTRIB_PROGRAM_FILES_COMMON		52
#define PATHATTRIB_PROGRAM_FILES_COMMONX86	53
#define PATHATTRIB_COMMON_TEMPLATES			54
#define PATHATTRIB_COMMON_DOCUMENTS			55
#define PATHATTRIB_COMMON_ADMINTOOLS		56
#define PATHATTRIB_ADMINTOOLS				57
#define PATHATTRIB_CONNECTIONS				58
#define PATHATTRIB_COMMON_MUSIC				59
#define PATHATTRIB_COMMON_PICTURES			60
#define PATHATTRIB_COMMON_VIDEO				61
#define PATHATTRIB_RESOURCES				62
#define PATHATTRIB_RESOURCES_LOCALIZED		63
#define PATHATTRIB_COMMON_OEM_LINKS			64
#define PATHATTRIB_CDBURN_AREA				65
#define PATHATTRIB_COMPUTERSNEARME			66

// ��ȡ�������е�Ŀ¼
// 0����
// 1����ֵ����������Ŀ¼��string��
ETHER_API getBasePath(lua_State* L);

// ���ü��а��ı�
// 1���������а��ı���string��
// 0����ֵ
ETHER_API setClipboardText(lua_State* L);

// ��ȡ���а��ı�
// 0����
// 1����ֵ�����а��ı���string��
ETHER_API getClipboardText(lua_State* L);

// ��ȡƽ̨����
// 0����
// 1����ֵ��ƽ̨���ͣ�string��
ETHER_API getPlatformType(lua_State* L);

// ��ȡ CPU �߼�������
// 0����
// 1����ֵ��CPU �߼���������number��
ETHER_API getCPUCount(lua_State* L);

// ��ȡϵͳ���ڴ��С
// 0����
// 1����ֵ��ϵͳ�ڴ��С��number����λΪMB��
ETHER_API getSystemTotalRAM(lua_State* L);

// ��ȡӦ���ڵ�ǰϵͳ�û��µĴ洢Ŀ¼������������򴴽�
// 2��������֯����string����Ӧ������string��
// 1����ֵ�������ļ���·����string��
ETHER_API getAppStorageDirectory(lua_State* L);

// ��ȡϵͳ����·��
// 1������·����ʶ��Macro number��
// 1����������·����string��
ETHER_API getSpecialPath(lua_State* L);

/*
* ��ȡ�豸��ǰ��Դ��Ϣ
* 0����
* 1����ֵ���豸��ǰ��Դ��Ϣ��table����ṹ���£���
	{
		state: ����״̬��Macro number��,
		remain_time���ɹ���Ϊʣ��ʹ��ʱ����number������λΪ�룬ʧ�ܻ�ǰ���ǵ�ع�����Ϊ -1��number��
		remain_percentage���ɹ��򷵻�ʣ������ٷֱȣ�number����ȡֵ��ΧΪ 0 - 100��ʧ����Ϊ -1��number��
	}
*/
ETHER_API getPowerInfo(lua_State* L);

/*
* ��ȡָ��Ŀ¼���ļ��б�
* 1������Ŀ¼·����string��
* 1����ֵ���ļ��б�table��
*/
ETHER_API listDirectory(lua_State* L);

/*
* �ж�ָ��·���Ƿ����
* 1��2������Ŀ¼���ļ����ڵ�·����string��������ѡ���ж�ģʽ��Macro number��
* 1����ֵ���Ƿ���ڣ�boolean��
*/
ETHER_API checkPathExist(lua_State* L);

/*
* ��ȡָ��·������Ϣ
* 1������Ŀ¼���ļ����ڵ�·����string��
* 1����ֵ���ɹ��򷵻�·����Ϣ��table����ṹ���£���
	{
		name���ļ�����string����
		size���ļ���С��number����
		time��{
			create������ʱ�䣨number����
			access�����һ�η���ʱ�䣨number����
			write�����һ���޸�ʱ�䣨number����
		}��
		attributes���ļ����ԣ�table����
	}
* ��ʧ���򷵻�nil
*/
ETHER_API getPathInfo(lua_State* L);

/*
* �ϲ�Ŀ¼���ļ���Ϊ����·��
* 2������Ŀ¼��string�����ļ�����string��
* 1����ֵ������·����string��
*/
ETHER_API joinPath(lua_State* L);

/*
* ��·���ָ�Ϊ�ļ���·�����ļ���
* 1������·����string��
* 2����ֵ���ļ���·����string�����ļ�����string��
*/
ETHER_API splitPath(lua_State* L);

class ModuleOS : public Module<ModuleOS>
{
	friend class Module<ModuleOS>;

private:
	ModuleOS()
	{
		_vCMethods = {
		{ "GetBasePath", getBasePath },
		{ "SetClipboardText", setClipboardText },
		{ "GetClipboardText", getClipboardText },
		{ "GetPlatformType", getPlatformType },
		{ "GetCPUCount", getCPUCount },
		{ "GetSystemTotalRAM", getSystemTotalRAM },
		{ "GetAppStorageDirectory", getAppStorageDirectory },
		{ "GetSpecialPath", getSpecialPath },
		{ "GetPowerInfo", getPowerInfo },
		{ "ListDirectory", listDirectory },
		{ "CheckPathExist", checkPathExist },
		{ "GetPathInfo", getPathInfo },
		{ "JoinPath", joinPath },
		{ "SplitPath", splitPath },
		};

		_vMacros = {
			{ "FILEATTRIB_ARCH", FILEATTRIB_ARCH },
			{ "FILEATTRIB_HIDDEN", FILEATTRIB_HIDDEN },
			{ "FILEATTRIB_NORMAL", FILEATTRIB_NORMAL },
			{ "FILEATTRIB_RDONLY", FILEATTRIB_RDONLY },
			{ "FILEATTRIB_SUBDIR", FILEATTRIB_SUBDIR },
			{ "FILEATTRIB_SYSTEM", FILEATTRIB_SYSTEM },

			{ "PATHMODE_FILE", PATHMODE_FILE },
			{ "PATHMODE_DIR", PATHMODE_DIR },
			{ "PATHMODE_FILEANDDIR", PATHMODE_FILEANDDIR },

			{ "POWERSTATE_UNKOWN", POWERSTATE_UNKOWN },
			{ "POWERSTATE_ONBATTERY", POWERSTATE_ONBATTERY },
			{ "POWERSTATE_NOBATTERY", POWERSTATE_NOBATTERY },
			{ "POWERSTATE_CHARGING", POWERSTATE_CHARGING },
			{ "POWERSTATE_CHARGEDN", POWERSTATE_CHARGEDN },

			{ "PATHATTRIB_DESKTOP", PATHATTRIB_DESKTOP },
			{ "PATHATTRIB_INTERNET", PATHATTRIB_INTERNET },
			{ "PATHATTRIB_PROGRAMS", PATHATTRIB_PROGRAMS },
			{ "PATHATTRIB_CONTROLS", PATHATTRIB_CONTROLS },
			{ "PATHATTRIB_PRINTERS", PATHATTRIB_PRINTERS },
			{ "PATHATTRIB_DOCUMENTS", PATHATTRIB_DOCUMENTS },
			{ "PATHATTRIB_FAVORITES", PATHATTRIB_FAVORITES },
			{ "PATHATTRIB_STARTUP", PATHATTRIB_STARTUP },
			{ "PATHATTRIB_RECENT", PATHATTRIB_RECENT },
			{ "PATHATTRIB_SENDTO", PATHATTRIB_SENDTO },
			{ "PATHATTRIB_RECYCLEBIN", PATHATTRIB_RECYCLEBIN },
			{ "PATHATTRIB_STARTMENU", PATHATTRIB_STARTMENU },
			{ "PATHATTRIB_MUSIC", PATHATTRIB_MUSIC },
			{ "PATHATTRIB_VIDEO", PATHATTRIB_VIDEO },
			{ "PATHATTRIB_DRIVES", PATHATTRIB_DRIVES },
			{ "PATHATTRIB_NETWORK", PATHATTRIB_NETWORK },
			{ "PATHATTRIB_NETHOOD", PATHATTRIB_NETHOOD },
			{ "PATHATTRIB_FONTS", PATHATTRIB_FONTS },
			{ "PATHATTRIB_TEMPLATES", PATHATTRIB_TEMPLATES },
			{ "PATHATTRIB_COMMON_STARTMENU", PATHATTRIB_COMMON_STARTMENU },
			{ "PATHATTRIB_COMMON_PROGRAMS", PATHATTRIB_COMMON_PROGRAMS },
			{ "PATHATTRIB_COMMON_STARTUP", PATHATTRIB_COMMON_STARTUP },
			{ "PATHATTRIB_COMMON_DESKTOP", PATHATTRIB_COMMON_DESKTOP },
			{ "PATHATTRIB_APPDATA", PATHATTRIB_APPDATA },
			{ "PATHATTRIB_PRINTHOOD", PATHATTRIB_PRINTHOOD },
			{ "PATHATTRIB_LOCAL_APPDATA", PATHATTRIB_LOCAL_APPDATA },
			{ "PATHATTRIB_COMMON_FAVORITES", PATHATTRIB_COMMON_FAVORITES },
			{ "PATHATTRIB_INTERNET_CACHE", PATHATTRIB_INTERNET_CACHE },
			{ "PATHATTRIB_COOKIES", PATHATTRIB_COOKIES },
			{ "PATHATTRIB_HISTORY", PATHATTRIB_HISTORY },
			{ "PATHATTRIB_COMMON_APPDATA", PATHATTRIB_COMMON_APPDATA },
			{ "PATHATTRIB_WINDOWS", PATHATTRIB_WINDOWS },
			{ "PATHATTRIB_SYSTEM", PATHATTRIB_SYSTEM },
			{ "PATHATTRIB_PROGRAM_FILES", PATHATTRIB_PROGRAM_FILES },
			{ "PATHATTRIB_PICTURES", PATHATTRIB_PICTURES },
			{ "PATHATTRIB_PROFILE", PATHATTRIB_PROFILE },
			{ "PATHATTRIB_SYSTEMX86", PATHATTRIB_SYSTEMX86 },
			{ "PATHATTRIB_PROGRAM_FILESX86", PATHATTRIB_PROGRAM_FILESX86 },
			{ "PATHATTRIB_PROGRAM_FILES_COMMON", PATHATTRIB_PROGRAM_FILES_COMMON },
			{ "PATHATTRIB_PROGRAM_FILES_COMMONX86", PATHATTRIB_PROGRAM_FILES_COMMONX86 },
			{ "PATHATTRIB_COMMON_TEMPLATES", PATHATTRIB_COMMON_TEMPLATES },
			{ "PATHATTRIB_COMMON_DOCUMENTS", PATHATTRIB_COMMON_DOCUMENTS },
			{ "PATHATTRIB_COMMON_ADMINTOOLS", PATHATTRIB_COMMON_ADMINTOOLS },
			{ "PATHATTRIB_ADMINTOOLS", PATHATTRIB_ADMINTOOLS },
			{ "PATHATTRIB_CONNECTIONS", PATHATTRIB_CONNECTIONS },
			{ "PATHATTRIB_COMMON_MUSIC", PATHATTRIB_COMMON_MUSIC },
			{ "PATHATTRIB_COMMON_PICTURES", PATHATTRIB_COMMON_PICTURES },
			{ "PATHATTRIB_COMMON_VIDEO", PATHATTRIB_COMMON_VIDEO },
			{ "PATHATTRIB_RESOURCES", PATHATTRIB_RESOURCES },
			{ "PATHATTRIB_RESOURCES_LOCALIZED", PATHATTRIB_RESOURCES_LOCALIZED },
			{ "PATHATTRIB_COMMON_OEM_LINKS", PATHATTRIB_COMMON_OEM_LINKS },
			{ "PATHATTRIB_CDBURN_AREA", PATHATTRIB_CDBURN_AREA },
			{ "PATHATTRIB_COMPUTERSNEARME", PATHATTRIB_COMPUTERSNEARME },
		};
	}

};

#endif // !_OS_H_
