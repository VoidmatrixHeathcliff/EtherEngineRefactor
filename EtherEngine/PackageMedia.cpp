#include "PackageMedia.h"

ETHER_API int EAPI_Media_LoadMusicFromFile(lua_State* pLuaVM)
{
	Mix_Music* _pMusic = nullptr;
	if (_pMusic = Mix_LoadMUS(luaL_checkstring(pLuaVM, 1)))
		EE_PushUserdata<Mix_Music>(pLuaVM, _pMusic, METANAME_MUSIC);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

//ETHER_API int EAPI_Media_LoadMusicFromFile(lua_State* pLuaVM)
//{
//	EE_Media_Music* _pMusic = new EE_Media_Music();
//	if (_pMusic->music = Mix_LoadMUS(luaL_checkstring(pLuaVM, 1)))
//		EE_PushUserdata<EE_Media_Music>(pLuaVM, _pMusic, METANAME_MUSIC);
//	else
//		lua_pushnil(pLuaVM);
//
//	return 1;
//}

ETHER_API int EAPI_Media_LoadMusicFromBuffer(lua_State* pLuaVM)
{
	size_t _szData = 0;
	const char* _pData = luaL_checklstring(pLuaVM, 1, &_szData);
	Mix_Music* _pMusic = nullptr;
	if (_pMusic = Mix_LoadMUS_RW(SDL_RWFromMem((void*)_pData, (int)_szData), 1))
		EE_PushUserdata<Mix_Music>(pLuaVM, _pMusic, METANAME_MUSIC);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

ETHER_API int EAPI_Media_Music_GC(lua_State* pLuaVM)
{
	Mix_Music* _pMusic = EE_ToUserdata<Mix_Music>(pLuaVM, 1, METANAME_MUSIC);

	Mix_FreeMusic(_pMusic); _pMusic = nullptr;

	return 0;
}

ETHER_API int EAPI_Media_Music_Type(lua_State* pLuaVM)
{
	Mix_Music* _pMusic = EE_ToUserdata<Mix_Music>(pLuaVM, 1, METANAME_MUSIC);

	switch (Mix_GetMusicType(_pMusic))
	{
	case MUS_WAV:	lua_pushinteger(pLuaVM, MEDIA_MUSIC_WAV); break;
	case MUS_MP3:	lua_pushinteger(pLuaVM, MEDIA_MUSIC_MP3); break;
	case MUS_OGG:	lua_pushinteger(pLuaVM, MEDIA_MUSIC_OGG); break;
	case MUS_CMD:	lua_pushinteger(pLuaVM, MEDIA_MUSIC_CMD); break;
	case MUS_MOD:	lua_pushinteger(pLuaVM, MEDIA_MUSIC_MOD); break;
	case MUS_MID:	lua_pushinteger(pLuaVM, MEDIA_MUSIC_MID); break;
	default:		lua_pushinteger(pLuaVM, MEDIA_MUSIC_UNKNOWN); break;
	}

	return 1;
}

ETHER_API int EAPI_Media_PlayMusic(lua_State* pLuaVM)
{
	Mix_Music* _pMusic = EE_ToUserdata<Mix_Music>(pLuaVM, 1, METANAME_MUSIC);

	if (lua_gettop(pLuaVM) < 3)
		Mix_PlayMusic(_pMusic, (int)luaL_checknumber(pLuaVM, 2));
	else
		Mix_FadeInMusic(_pMusic, (int)luaL_checknumber(pLuaVM, 2), 
			(int)luaL_checknumber(pLuaVM, 3));

	return 0;
}

ETHER_API int EAPI_Media_StopMusic(lua_State* pLuaVM)
{
	if (lua_gettop(pLuaVM) < 1)
		Mix_HaltMusic();
	else
		Mix_FadeOutMusic((int)luaL_checknumber(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Media_SetMusicPosition(lua_State* pLuaVM)
{
	Mix_SetMusicPosition(luaL_checknumber(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Media_SetMusicVolume(lua_State* pLuaVM)
{
	Mix_VolumeMusic((int)luaL_checknumber(pLuaVM, 1));

	return 0;
}

ETHER_API int EAPI_Media_GetMusicVolume(lua_State* pLuaVM)
{
	lua_pushinteger(pLuaVM, Mix_VolumeMusic(-1));

	return 1;
}

ETHER_API int EAPI_Media_PauseMusic(lua_State* pLuaVM)
{
	Mix_PauseMusic();

	return 0;
}

ETHER_API int EAPI_Media_ResumeMusic(lua_State* pLuaVM)
{
	Mix_ResumeMusic();

	return 0;
}

ETHER_API int EAPI_Media_RewindMusic(lua_State* pLuaVM)
{
	Mix_RewindMusic();

	return 0;
}

ETHER_API int EAPI_Media_CheckMusicPlaying(lua_State* pLuaVM)
{
	lua_pushboolean(pLuaVM, Mix_PlayingMusic());

	return 1;
}

ETHER_API int EAPI_Media_CheckMusicPaused(lua_State* pLuaVM)
{
	lua_pushboolean(pLuaVM, Mix_PausedMusic());

	return 1;
}

ETHER_API int EAPI_Media_GetMusicFadingType(lua_State* pLuaVM)
{
	switch (Mix_FadingMusic())
	{
	case MIX_NO_FADING:		lua_pushinteger(pLuaVM, MEDIA_FADING_NONE); break;
	case MIX_FADING_OUT:	lua_pushinteger(pLuaVM, MEDIA_FADING_OUT); break;
	case MIX_FADING_IN:		lua_pushinteger(pLuaVM, MEDIA_FADING_IN); break;
	default:				lua_pushinteger(pLuaVM, MEDIA_FADING_NONE); break;
	}

	return 1;
}

ETHER_API int EAPI_Media_LoadSoundFromFile(lua_State* pLuaVM)
{
	Mix_Chunk* _pSound = nullptr;
	if (_pSound = Mix_LoadWAV(luaL_checkstring(pLuaVM, 1)))
		EE_PushUserdata<Mix_Chunk>(pLuaVM, _pSound, METANAME_SOUND);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

ETHER_API int EAPI_Media_LoadSoundFromBuffer(lua_State* pLuaVM)
{
	size_t _szData = 0;
	const char* _pData = luaL_checklstring(pLuaVM, 1, &_szData);
	Mix_Chunk* _pSound = nullptr;
	if (_pSound = Mix_LoadWAV_RW(SDL_RWFromMem((void*)_pData, (int)_szData), 1))
		EE_PushUserdata<Mix_Chunk>(pLuaVM, _pSound, METANAME_SOUND);
	else
		lua_pushnil(pLuaVM);

	return 1;
}

ETHER_API int EAPI_Media_Sound_GC(lua_State* pLuaVM)
{
	Mix_Chunk* _pSound = EE_ToUserdata<Mix_Chunk>(pLuaVM, 1, METANAME_SOUND);

	Mix_FreeChunk(_pSound); _pSound = nullptr;

	return 0;
}

ETHER_API int EAPI_Media_Sound_Play(lua_State* pLuaVM)
{
	Mix_Chunk* _pSound = EE_ToUserdata<Mix_Chunk>(pLuaVM, 1, METANAME_SOUND);

	Mix_PlayChannel(-1, _pSound, (int)luaL_checknumber(pLuaVM, 2));

	return 0;
}

ETHER_API int EAPI_Media_Sound_SetVolume(lua_State* pLuaVM)
{
	Mix_Chunk* _pSound = EE_ToUserdata<Mix_Chunk>(pLuaVM, 1, METANAME_SOUND);

	Mix_VolumeChunk(_pSound, (int)luaL_checknumber(pLuaVM, 2));

	return 0;
}

ETHER_API int EAPI_Media_Sound_GetVolume(lua_State* pLuaVM)
{
	Mix_Chunk* _pSound = EE_ToUserdata<Mix_Chunk>(pLuaVM, 1, METANAME_SOUND);

	lua_pushinteger(pLuaVM, _pSound->volume);

	return 1;
}