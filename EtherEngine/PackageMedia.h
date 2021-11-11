#ifndef _MEDIA_H_
#define _MEDIA_H_

#include "Config.h"
#include "Utils.h"

#include <lua.hpp>
#include <SDL.h>
#include <SDL_mixer.h>

#include <string>

static const int MEDIA_MUSIC_WAV		= 0;
static const int MEDIA_MUSIC_MP3		= 1;
static const int MEDIA_MUSIC_OGG		= 2;
static const int MEDIA_MUSIC_CMD		= 3;
static const int MEDIA_MUSIC_MOD		= 4;
static const int MEDIA_MUSIC_MID		= 5;
static const int MEDIA_MUSIC_UNKNOWN	= 6;

static const int MEDIA_FADING_NONE		= 0;
static const int MEDIA_FADING_IN		= 1;
static const int MEDIA_FADING_OUT		= 2;

struct EE_Media_Music
{
	Mix_Music* music = nullptr;
	std::string buffer;
};

// ���������ļ�
// 1�����������ļ�·��
// 1����ֵ���ɹ��򷵻��������ݣ�userdata-Music����ʧ���򷵻�nil
ETHER_API int EAPI_Media_LoadMusicFromFile(lua_State* L);

ETHER_API int EAPI_Media_LoadMusicFromBuffer(lua_State* pLuaVM);

// ��������GC����
// 1�������������ݣ�userdata-Music��
// 0����ֵ
ETHER_API int EAPI_Media_Music_GC(lua_State* L);

// ��ȡָ�����ֵ�����
// 1�������������ݣ�userdata-Music��nil��ʾ���ڲ��ŵ����֣�
// 1����ֵ���������ͣ�MACRO number��
ETHER_API int EAPI_Media_Music_Type(lua_State* L);

// �����Ѽ��ص�����
// 2�������������ݣ�userdata-Music�������ֲ��ŵĴ�����number��-1Ϊѭ�����ţ�
// 0����ֵ
ETHER_API int EAPI_Media_PlayMusic(lua_State* L);

ETHER_API int EAPI_Media_StopMusic(lua_State* L);

// ֹͣ���ڲ��ŵ�����
// 0����
// 0����ֵ
ETHER_API int EAPI_Media_StopMusic(lua_State* L);

// ���õ����ֵĲ���λ��
// 1����������λ�ã�number��
// 0����ֵ
// ��ע���˲�����֧���������ָ�ʽ�����֣��Ҳ������������ͬ��
// MOD�����õ�ǰ����λ��Ϊ�˱���ģ���ж�Ӧ���������ݿ鴦��0 ����ת�����ֿ�ͷ
// OGG�����õ�ǰ����λ��Ϊ�����ֿ�ʼλ�ü����ָ��������
// MP3�����õ�ǰ����λ��Ϊ�ӵ�ǰ����λ�ü����ָ��������������ʹ�� RewindMusic ����ǰ���ֵĲ���λ�����õ���ʼ��
ETHER_API int EAPI_Media_SetMusicPosition(lua_State* L);

// �������ֲ��ŵ�����
// 1������������С��number��ȡֵ��ΧΪ0-128��
// 0����ֵ
ETHER_API int EAPI_Media_SetMusicVolume(lua_State* L);

// ��ȡ��ǰ���ֲ��ŵ�����
// 0����
// 1����ֵ��������С��number��ȡֵ��ΧΪ0-128��
ETHER_API int EAPI_Media_GetMusicVolume(lua_State* L);

// ��ͣ��ǰ���ڲ��ŵ�����
// 0����
// 0����ֵ
ETHER_API int EAPI_Media_PauseMusic(lua_State* L);

// �ָ���ǰ��ͣ״̬������
// 0����
// 0����ֵ
ETHER_API int EAPI_Media_ResumeMusic(lua_State* L);

// ���²��ŵ�ǰ���ڲ��ŵ�����
// 0����
// 0����ֵ
ETHER_API int EAPI_Media_RewindMusic(lua_State* L);

// �жϵ�ǰ�����Ƿ����ڲ���
// 0����
// 1����ֵ���Ƿ����ڲ��ţ�boolean��
ETHER_API int EAPI_Media_CheckMusicPlaying(lua_State* L);

// �жϵ�ǰ�����Ƿ�����ͣ
// 0����
// 1����ֵ���Ƿ�����ͣ��boolean��
ETHER_API int EAPI_Media_CheckMusicPaused(lua_State* L);

// ��ȡ��ǰ���ڲ��ŵ����ֵĵ���Ч������
// 0����
// 1����ֵ������Ч�����ͣ�MACRO number��
ETHER_API int EAPI_Media_GetMusicFadingType(lua_State* L);

// ���ļ��м�����Ч
// 1��������Ч�ļ�·��
// 1����ֵ���ɹ��򷵻���Ч���ݣ�userdata-Sound����ʧ���򷵻�nil
ETHER_API int EAPI_Media_LoadSoundFromFile(lua_State* L);

// �ӻ������м�����Ч
// 1���������������ݣ�string��
// 1����ֵ���ɹ��򷵻���Ч���ݣ�userdata-Sound����ʧ���򷵻�nil
ETHER_API int EAPI_Media_LoadSoundFromBuffer(lua_State* L);

// ��Ч����GC����
// 1��������Ч���ݣ�userdata-Sound��
// 0����ֵ
ETHER_API int EAPI_Media_Sound_GC(lua_State* L);

// �����Ѽ��ص���Ч
// 2��������Ч���ݣ�userdata-Sound������Ч���ŵĴ�����number��-1Ϊѭ�����ţ�
// 0����ֵ
ETHER_API int EAPI_Media_Sound_Play(lua_State* L);

// ������Ч���ŵ�����
// 2��������Ч���ݣ�userdata-Sound����������С��number��ȡֵ��ΧΪ0-128��
// 0����ֵ
ETHER_API int EAPI_Media_Sound_SetVolume(lua_State* L);

// ��ȡ��Ч���ŵ�����
// 1��������Ч���ݣ�userdata-Sound��
// 1����ֵ��������С��number��ȡֵ��ΧΪ0-128��
ETHER_API int EAPI_Media_Sound_GetVolume(lua_State* L);

#endif // !_MEDIA_H_

