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

// 加载音乐文件
// 1参数：音乐文件路径
// 1返回值：成功则返回音乐数据（userdata-Music），失败则返回nil
ETHER_API int EAPI_Media_LoadMusicFromFile(lua_State* L);

ETHER_API int EAPI_Media_LoadMusicFromBuffer(lua_State* pLuaVM);

// 音乐数据GC函数
// 1参数：音乐数据（userdata-Music）
// 0返回值
ETHER_API int EAPI_Media_Music_GC(lua_State* L);

// 获取指定音乐的类型
// 1参数：音乐数据（userdata-Music，nil表示正在播放的音乐）
// 1返回值：音乐类型（MACRO number）
ETHER_API int EAPI_Media_Music_Type(lua_State* L);

// 播放已加载的音乐
// 2参数：音乐数据（userdata-Music）、音乐播放的次数（number，-1为循环播放）
// 0返回值
ETHER_API int EAPI_Media_PlayMusic(lua_State* L);

ETHER_API int EAPI_Media_StopMusic(lua_State* L);

// 停止正在播放的音乐
// 0参数
// 0返回值
ETHER_API int EAPI_Media_StopMusic(lua_State* L);

// 设置当音乐的播放位置
// 1参数：播放位置（number）
// 0返回值
// 备注：此操作仅支持以下三种格式的音乐，且参数意义各不相同：
// MOD：设置当前播放位置为此编码模型中对应索引的数据块处，0 将跳转至音乐开头
// OGG：设置当前播放位置为从音乐开始位置计算的指定秒数处
// MP3：设置当前播放位置为从当前播放位置计算的指定秒数处，可以使用 RewindMusic 将当前音乐的播放位置重置到开始处
ETHER_API int EAPI_Media_SetMusicPosition(lua_State* L);

// 设置音乐播放的音量
// 1参数：音量大小（number，取值范围为0-128）
// 0返回值
ETHER_API int EAPI_Media_SetMusicVolume(lua_State* L);

// 获取当前音乐播放的音量
// 0参数
// 1返回值：音量大小（number，取值范围为0-128）
ETHER_API int EAPI_Media_GetMusicVolume(lua_State* L);

// 暂停当前正在播放的音乐
// 0参数
// 0返回值
ETHER_API int EAPI_Media_PauseMusic(lua_State* L);

// 恢复当前暂停状态的音乐
// 0参数
// 0返回值
ETHER_API int EAPI_Media_ResumeMusic(lua_State* L);

// 重新播放当前正在播放的音乐
// 0参数
// 0返回值
ETHER_API int EAPI_Media_RewindMusic(lua_State* L);

// 判断当前音乐是否正在播放
// 0参数
// 1返回值：是否正在播放（boolean）
ETHER_API int EAPI_Media_CheckMusicPlaying(lua_State* L);

// 判断当前音乐是否已暂停
// 0参数
// 1返回值：是否已暂停（boolean）
ETHER_API int EAPI_Media_CheckMusicPaused(lua_State* L);

// 获取当前正在播放的音乐的淡化效果类型
// 0参数
// 1返回值：淡化效果类型（MACRO number）
ETHER_API int EAPI_Media_GetMusicFadingType(lua_State* L);

// 从文件中加载音效
// 1参数：音效文件路径
// 1返回值：成功则返回音效数据（userdata-Sound），失败则返回nil
ETHER_API int EAPI_Media_LoadSoundFromFile(lua_State* L);

// 从缓冲区中加载音效
// 1参数：缓冲区数据（string）
// 1返回值：成功则返回音效数据（userdata-Sound），失败则返回nil
ETHER_API int EAPI_Media_LoadSoundFromBuffer(lua_State* L);

// 音效数据GC函数
// 1参数：音效数据（userdata-Sound）
// 0返回值
ETHER_API int EAPI_Media_Sound_GC(lua_State* L);

// 播放已加载的音效
// 2参数：音效数据（userdata-Sound）、音效播放的次数（number，-1为循环播放）
// 0返回值
ETHER_API int EAPI_Media_Sound_Play(lua_State* L);

// 设置音效播放的音量
// 2参数：音效数据（userdata-Sound），音量大小（number，取值范围为0-128）
// 0返回值
ETHER_API int EAPI_Media_Sound_SetVolume(lua_State* L);

// 获取音效播放的音量
// 1参数：音效数据（userdata-Sound）
// 1返回值：音量大小（number，取值范围为0-128）
ETHER_API int EAPI_Media_Sound_GetVolume(lua_State* L);

#endif // !_MEDIA_H_

