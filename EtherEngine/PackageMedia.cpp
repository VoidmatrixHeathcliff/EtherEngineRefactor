#include "PackageMedia.h"


ETHER_API int loadMusic(lua_State * L)
{
	Mix_Music* music = Mix_LoadMUS(luaL_checkstring(L, 1));
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, music, 1, "load music failed");
#endif
	Mix_Music** uppMusic = (Mix_Music**)lua_newuserdata(L, sizeof(Mix_Music*));
	*uppMusic = music;
	luaL_getmetatable(L, METANAME_MUSIC);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int __gc_Music(lua_State * L)
{
	Mix_Music* music = GetMusicDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckMusicDataAt1stPos(music);
#endif
	Mix_FreeMusic(music);
	music = nullptr;

	return 0;
}


ETHER_API int playMusic(lua_State * L)
{
	Mix_Music* music = GetMusicDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckMusicDataAt1stPos(music);
#endif
	Mix_PlayMusic(music, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int playMusicWithFadeIn(lua_State * L)
{
	Mix_Music* music = GetMusicDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckMusicDataAt1stPos(music);
#endif
	Mix_FadeInMusic(music, luaL_checknumber(L, 2), luaL_checknumber(L, 3));

	return 0;
}


ETHER_API int stopMusic(lua_State* L)
{
	Mix_HaltMusic();

	return 0;
}


ETHER_API int stopMusicWithFadeOut(lua_State * L)
{
	Mix_FadeOutMusic(luaL_checknumber(L, 1));

	return 0;
}


ETHER_API int setMusicPosition(lua_State* L)
{
	Mix_SetMusicPosition(luaL_checknumber(L, 1));

	return 0;
}


ETHER_API int setMusicVolume(lua_State * L)
{
	Mix_VolumeMusic(luaL_checknumber(L, 1));

	return 0;
}


ETHER_API int getMusicVolume(lua_State * L)
{
	lua_pushnumber(L, Mix_VolumeMusic(-1));

	return 1;
}


ETHER_API int pauseMusic(lua_State * L)
{
	Mix_PauseMusic();

	return 0;
}


ETHER_API int resumeMusic(lua_State * L)
{
	Mix_ResumeMusic();

	return 0;
}


ETHER_API int rewindMusic(lua_State * L)
{
	Mix_RewindMusic();

	return 0;
}


ETHER_API int checkMusicPlaying(lua_State* L)
{
	lua_pushboolean(L, Mix_PlayingMusic());

	return 1;
}


ETHER_API int checkMusicPaused(lua_State* L)
{
	lua_pushboolean(L, Mix_PausedMusic());

	return 1;
}


ETHER_API int getMusicFadingType(lua_State* L)
{
	switch (Mix_FadingMusic())
	{
	case MIX_NO_FADING:
		lua_pushinteger(L, FADING_TYPE_NONE);
		break;
	case MIX_FADING_OUT:
		lua_pushinteger(L, FADING_TYPE_OUT);
		break;
	case MIX_FADING_IN:
		lua_pushinteger(L, FADING_TYPE_IN);
		break;
	default:
		break;
	}

	return 1;
}


ETHER_API int music_GetType(lua_State * L)
{
	Mix_Music* music = GetMusicDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckMusicDataAt1stPos(music);
#endif
	switch (Mix_GetMusicType(music))
	{
	case MUS_WAV:
		lua_pushnumber(L, MUSIC_TYPE_WAV);
		break;
	case MUS_MP3:
		lua_pushnumber(L, MUSIC_TYPE_MP3);
		break;
	case MUS_OGG:
		lua_pushnumber(L, MUSIC_TYPE_OGG);
		break;
	case MUS_CMD:
		lua_pushnumber(L, MUSIC_TYPE_CMD);
		break;
	case MUS_MOD:
		lua_pushnumber(L, MUSIC_TYPE_MOD);
		break;
	case MUS_MID:
		lua_pushnumber(L, MUSIC_TYPE_MID);
		break;
	default:
		lua_pushnumber(L, MUSIC_TYPE_UNKONWN);
		break;
	}

	return 1;
}


ETHER_API int loadSoundFromFile(lua_State * L)
{
	Mix_Chunk* sound = Mix_LoadWAV(luaL_checkstring(L, 1));
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, sound, 1, "load sound failed");
#endif
	Mix_Chunk** uppSound = (Mix_Chunk**)lua_newuserdata(L, sizeof(Mix_Chunk*));
	*uppSound = sound;
	luaL_getmetatable(L, METANAME_SOUND);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int loadSoundFromData(lua_State* L)
{
	size_t size = 0;
	Mix_Chunk* sound = Mix_LoadWAV_RW(SDL_RWFromMem((void*)luaL_checklstring(L, 1, &size), size), 1);
#ifdef _ETHER_DEBUG_
	luaL_argcheck(L, sound, 1, "load sound failed");
#endif
	Mix_Chunk** uppSound = (Mix_Chunk**)lua_newuserdata(L, sizeof(Mix_Chunk*));
	*uppSound = sound;
	luaL_getmetatable(L, METANAME_SOUND);
	lua_setmetatable(L, -2);

	return 1;
}


ETHER_API int __gc_Sound(lua_State * L)
{
	Mix_Chunk* sound = GetSoundDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckSoundDataAt1stPos(sound);
#endif
	Mix_FreeChunk(sound);
	sound = nullptr;

	return 0;
}


ETHER_API int sound_Play(lua_State * L)
{
	Mix_Chunk* sound = GetSoundDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckSoundDataAt1stPos(sound);
#endif
	Mix_PlayChannel(-1, sound, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int sound_SetVolume(lua_State* L)
{
	Mix_Chunk* sound = GetSoundDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckSoundDataAt1stPos(sound);
#endif
	Mix_VolumeChunk(sound, luaL_checknumber(L, 2));

	return 0;
}


ETHER_API int sound_GetVolume(lua_State* L)
{
	Mix_Chunk* sound = GetSoundDataAt1stPos();
#ifdef _ETHER_DEBUG_
	CheckSoundDataAt1stPos(sound);
#endif
	lua_pushnumber(L, sound->volume);

	return 1;
}