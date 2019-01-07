/*
MIT License

Copyright (c) 2018 Cody Tilkins

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "AL/al.h"
#include "AL/alc.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


#include "wave.h"


#if defined(_WIN32) || defined(_WIN64)
#	define LUA_DLL	__declspec(dllexport)
#else
#	define LUA_DLL	__attribute__((visibility("default")))
#endif

#define LUA_DLL_ENTRY 	LUA_DLL int



#define luaL_checktable(L, I)	luaL_checktype((L), (I), LUA_TTABLE)

void* luaL_checkuserdata(lua_State* L, int i) {
	luaL_checktype(L, i, LUA_TUSERDATA);
	return lua_touserdata(L, i);
}


// Not sure the validity of <= 501
#if LUA_VERSION_NUM <= 501
#	define luaL_tablelen(L, I) lua_objlen((L), (I))
#elif LUA_VERSION_NUM >= 502
#	define luaL_tablelen(L, I) lua_rawlen((L), (I))
#endif



// -----

static int lua_alEnable(lua_State* L) {
	alEnable(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alDisable(lua_State* L) {
	alDisable(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alIsEnabled(lua_State* L) {
	char obool = alIsEnabled(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushnumber(L, obool);
	return 1;
}

// --

static int lua_alGetString(lua_State* L) {
	const char* ostr = alGetString(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushstring(L, ostr);
	return 1;
}

static int lua_alGetBooleanv(lua_State* L) {
	char obool = 0;
	alGetBooleanv(luaL_checknumber(L, 1), &obool);
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}

static int lua_alGetIntegerv(lua_State* L) {
	int oint = 0;
	alGetIntegerv(luaL_checknumber(L, 1), &oint);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oint);
	return 1;
}

static int lua_alGetFloatv(lua_State* L) {
	float oflt = 0;
	alGetFloatv(luaL_checknumber(L, 1), &oflt);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oflt);
	return 1;
}

static int lua_alGetDoublev(lua_State* L) {
	double odoub = 0;
	alGetDoublev(luaL_checknumber(L, 1), &odoub);
	lua_checkstack(L, 1);
	lua_pushnumber(L, odoub);
	return 1;
}

static int lua_alGetBoolean(lua_State* L) {
	char obool = alGetBoolean(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}

static int lua_alGetInteger(lua_State* L) {
	int oint = alGetInteger(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushnumber(L, oint);
	return 1;
}

static int lua_alGetFloat(lua_State* L) {
	float oflt = alGetFloat(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushnumber(L, oflt);
	return 1;
}

static int lua_alGetDouble(lua_State* L) {
	double odoub = alGetDouble(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushnumber(L, odoub);
	return 1;
}

// --

static int lua_alGetError(lua_State* L) {
	int oerr = alGetError();
	lua_checkstack(L, 1);
	lua_pushnumber(L, oerr);
	return 1;
}

// --

static int lua_alIsExtensionPresent(lua_State* L) {
	char obool = alIsExtensionPresent(luaL_checkstring(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}


// This isn't implemented >:O
// static int alGetProcAddress(lua_State* L) {
	// const char* pstr = luaL_checkstring(L, 1);
	
	// return 1;
// }

static int lua_alGetEnumValue(lua_State* L) {
	int oenum = alGetEnumValue(luaL_checkstring(L, 1));
	lua_checkstack(L, 1);
	lua_pushnumber(L, oenum);
	return 1;
}

// --

static int lua_alListenerf(lua_State* L) {
	alListenerf(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
	return 0;
}

static int lua_alListener3f(lua_State* L) {
	alListener3f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	return 0;
}

static int lua_alListener6f(lua_State* L) {
	int penum = luaL_checknumber(L, 1);
	float oflts[] = {
		luaL_checknumber(L, 2),
		luaL_checknumber(L, 3),
		luaL_checknumber(L, 4),
		luaL_checknumber(L, 5),
		luaL_checknumber(L, 6),
		luaL_checknumber(L, 7)};
	alListenerfv(penum, oflts);
	return 0;
}


static int lua_alListeneri(lua_State* L) {
	alListeneri(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
	return 0;
}

static int lua_alListener3i(lua_State* L) {
	alListener3i(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	return 0;
}

static int lua_alGetListenerf(lua_State* L) {
	float oflt = 0;
	alGetListenerf(luaL_checknumber(L, 1), &oflt);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oflt);
	return 1;
}

static int lua_alGetListener3f(lua_State* L) {
	float oflt1 = 0;
	float oflt2 = 0;
	float oflt3 = 0;
	alGetListener3f(luaL_checknumber(L, 1), &oflt1, &oflt2, &oflt3);
	lua_checkstack(L, 3);
	lua_pushnumber(L, oflt1);
	lua_pushnumber(L, oflt2);
	lua_pushnumber(L, oflt3);
	return 3;
}

static int lua_alGetListeneri(lua_State* L) {
	int oint = 0;
	alGetListeneri(luaL_checknumber(L, 1), &oint);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oint);
	return 1;
}

static int lua_alGetListener3i(lua_State* L) {
	int oint1 = 0;
	int oint2 = 0;
	int oint3 = 0;
	alGetListener3i(luaL_checknumber(L, 1), &oint1, &oint2, &oint3);
	lua_checkstack(L, 3);
	lua_pushnumber(L, oint1);
	lua_pushnumber(L, oint2);
	lua_pushnumber(L, oint3);
	return 3;
}

// --

static int lua_alGenSources(lua_State* L) {
	int psize = luaL_checknumber(L, 1);
	
	unsigned int* ints = malloc(1 * psize * sizeof(unsigned int));
	alGenSources(psize, ints);
	
	lua_createtable(L, psize, 0);
	for(int i=0; i<psize; i++) {
		lua_pushnumber(L, ints[i]);
		lua_rawseti(L, -2, i+1);
	}
	
	free(ints);
	
	return 1;
}

static int lua_alDeleteSources(lua_State* L) {
	int psize = luaL_checknumber(L, 1);
	luaL_checktable(L, 2);
	
	unsigned int* ints = malloc(1 * psize * sizeof(unsigned int));
	for(int i=0; i<psize; i++) {
		lua_rawgeti(L, 2, i+1);
		ints[i] = lua_tonumber(L, -1);
		lua_pop(L, 1); // number
	}
	
	alDeleteSources(psize, ints);
	
	free(ints);
	return 0;
}

static int lua_alIsSource(lua_State* L) {
	char obool = alIsSource(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}

// --

static int lua_alSourcef(lua_State* L) {
	alSourcef(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int lua_alSource3f(lua_State* L) {
	alSource3f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int lua_alSourcei(lua_State* L) {
	alSourcei(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int lua_alSource3i(lua_State* L) {
	alSource3i(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int lua_alGetSourcef(lua_State* L) {
	float oflt = 0;
	alGetSourcef(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oflt);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oflt);
	return 1;
}

static int lua_alGetSource3f(lua_State* L) {
	float oflt1 = 0;
	float oflt2 = 0;
	float oflt3 = 0;
	alGetSource3f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oflt1, &oflt2, &oflt3);
	lua_checkstack(L, 3);
	lua_pushnumber(L, oflt1);
	lua_pushnumber(L, oflt2);
	lua_pushnumber(L, oflt3);
	return 3;
}

static int lua_alGetSourcei(lua_State* L) {
	int oint = 0;
	alGetSourcei(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oint);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oint);
	return 1;
}

static int lua_alGetSource3i(lua_State* L) {
	int oint1 = 0;
	int oint2 = 0;
	int oint3 = 0;
	alGetSource3i(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oint1, &oint2, &oint3);
	lua_checkstack(L, 3);
	lua_pushnumber(L, oint1);
	lua_pushnumber(L, oint2);
	lua_pushnumber(L, oint3);
	return 3;
}

// --

static int lua_alSourcePlay(lua_State* L) {
	alSourcePlay(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alSourceStop(lua_State* L) {
	alSourceStop(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alSourceRewind(lua_State* L) {
	alSourceRewind(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alSourcePause(lua_State* L) {
	alSourcePause(luaL_checknumber(L, 1));
	return 0;
}

// --

static int lua_alSourceQueueBuffers(lua_State* L) {
	unsigned int pid = luaL_checknumber(L, 1);
	int psize = 0;
	if(lua_isnumber(L, 1)) {
		psize = luaL_checknumber(L, 2);
		luaL_checktable(L, 3);
	}
	if(psize == 0) {
		luaL_checktable(L, 2);
		psize = luaL_tablelen(L, 2);
	}
	unsigned int* uints = malloc(1 * psize * sizeof(unsigned int));
	for(int i=0; i<psize; i++) {
		lua_rawgeti(L, -1, i+1);
		uints[i] = lua_tonumber(L, -1);
		lua_pop(L, 1); // number
	}
	alSourceQueueBuffers(pid, psize, uints);
	free(uints);
	return 0;
}

static int lua_alSourceUnqueueBuffers(lua_State* L) {
	unsigned int pid = luaL_checknumber(L, 1);
	int psize = 0;
	if(lua_isnumber(L, 1)) {
		psize = luaL_checknumber(L, 2);
		luaL_checktable(L, 3);
	}
	if(psize == 0) {
		luaL_checktable(L, 2);
		psize = luaL_tablelen(L, 2);
	}
	unsigned int* uints = malloc(1 * psize * sizeof(unsigned int));
	for(int i=0; i<psize; i++) {
		lua_rawgeti(L, -1, i+1);
		uints[i] = lua_tonumber(L, -1);
		lua_pop(L, 1); // number
	}
	alSourceUnqueueBuffers(pid, psize, uints);
	free(uints);
	return 0;
}

// --

static int lua_alGenBuffers(lua_State* L) {
	int psize = luaL_checknumber(L, 1);
	
	unsigned int* ints = malloc(1 * psize * sizeof(unsigned int));
	alGenBuffers(psize, ints);
	
	lua_createtable(L, 0, psize);
	for(int i=0; i<psize; i++) {
		lua_pushnumber(L, ints[i]);
		lua_rawseti(L, -2, i+1);
	}
	
	free(ints);
	
	return 1;
}

static int lua_alDeleteBuffers(lua_State* L) {
	int psize = luaL_checknumber(L, 1);
	luaL_checktable(L, 2);
	
	unsigned int* ints = malloc(1 * psize * sizeof(unsigned int));
	for(int i=0; i<psize; i++) {
		lua_rawgeti(L, 2, i+1);
		ints[i] = lua_tonumber(L, -1);
		lua_pop(L, 1); // number
	}
	
	alDeleteBuffers(psize, ints);
	
	free(ints);
	
	return 0;
}

static int lua_alIsBuffer(lua_State* L) {
	char obool = alIsBuffer(luaL_checknumber(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}

static int lua_alBufferData(lua_State* L) {
	size_t size = 0;
	alBufferData(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checklstring(L, 3, &size), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

// --

static int lua_alBufferf(lua_State* L) {
	alBufferf(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int lua_alBuffer3f(lua_State* L) {
	alBuffer3f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int lua_alBufferi(lua_State* L) {
	alBufferi(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
	return 0;
}

static int lua_alBuffer3i(lua_State* L) {
	alBuffer3i(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5));
	return 0;
}

static int lua_alGetBufferf(lua_State* L) {
	float oflt = 0;
	alGetBufferf(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oflt);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oflt);
	return 1;
}

static int lua_alGetBuffer3f(lua_State* L) {
	float oflt1 = 0;
	float oflt2 = 0;
	float oflt3 = 0;
	alGetBuffer3f(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oflt1, &oflt2, &oflt3);
	lua_checkstack(L, 3);
	lua_pushnumber(L, oflt1);
	lua_pushnumber(L, oflt2);
	lua_pushnumber(L, oflt3);
	return 3;
}

static int lua_alGetBufferi(lua_State* L) {
	int oint = 0;
	alGetBufferi(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oint);
	lua_checkstack(L, 1);
	lua_pushnumber(L, oint);
	return 1;
}

static int lua_alGetBuffer3i(lua_State* L) {
	int oint1 = 0;
	int oint2 = 0;
	int oint3 = 0;
	alGetBuffer3i(luaL_checknumber(L, 1), luaL_checknumber(L, 2), &oint1, &oint2, &oint3);
	lua_checkstack(L, 3);
	lua_pushnumber(L, oint1);
	lua_pushnumber(L, oint2);
	lua_pushnumber(L, oint3);
	return 3;
}

// --

static int lua_alDopplerFactor(lua_State* L) {
	alDopplerFactor(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alDopplerVelocity(lua_State* L) {
	alDopplerVelocity(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alSpeedOfSound(lua_State* L) {
	alSpeedOfSound(luaL_checknumber(L, 1));
	return 0;
}

static int lua_alDistanceModel(lua_State* L) {
	alDistanceModel(luaL_checknumber(L, 1));
	return 0;
}

// -----

static int lua_alcCreateContext(lua_State* L) {
	ALCdevice* device = *(ALCdevice**)luaL_checkuserdata(L, 1);
	int* ints = 0;
	if(!lua_isnil(L, 2)) {
		luaL_checktable(L, 2);
		size_t len = luaL_tablelen(L, 2);
		ints = malloc(1 * len * sizeof(int));
		for(int i=0; i<len; i++) {
			lua_rawgeti(L, 2, i+1);
			ints[i] = lua_tonumber(L, 3);
			lua_pop(L, 1); // number
		}
	}
	
	ALCcontext* context = alcCreateContext(device, ints);
	free(ints);
	lua_checkstack(L, 1);
	ALCcontext** data = (ALCcontext**)lua_newuserdata(L, sizeof(ALCcontext*));
	*data = context;
	return 1;
}

static int lua_alcMakeContextCurrent(lua_State* L) {
	char obool = alcMakeContextCurrent(*(ALCcontext**)luaL_checkuserdata(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}


static int lua_alcProcessContext(lua_State* L) {
	alcProcessContext(*(ALCcontext**)luaL_checkuserdata(L, 1));
	return 0;
}

static int lua_alcSuspendContext(lua_State* L) {
	alcSuspendContext(*(ALCcontext**)luaL_checkuserdata(L, 1));
	return 0;
}

static int lua_alcDestroyContext(lua_State* L) {
	alcDestroyContext(*(ALCcontext**)luaL_checkuserdata(L, 1));
	return 0;
}

static int lua_alcGetCurrentContext(lua_State* L) {
	ALCcontext* context = alcGetCurrentContext();
	lua_checkstack(L, 1);
	ALCcontext** data = (ALCcontext**)lua_newuserdata(L, sizeof(ALCcontext*));
	*data = context;
	return 1;
}

static int lua_alcGetContextsDevice(lua_State* L) {
	ALCdevice* device = alcGetContextsDevice(*(ALCcontext**)luaL_checkuserdata(L, 1));
	lua_checkstack(L, 1);
	ALCdevice** data = (ALCdevice**)lua_newuserdata(L, sizeof(ALCdevice*));
	*data = device;
	return 1;
}

static int lua_alcOpenDevice(lua_State* L) {
	const char* pstr = 0;
	if(!lua_isnil(L, 1))
		pstr = luaL_checkstring(L, 1);
	ALCdevice* device = alcOpenDevice(pstr);
	lua_checkstack(L, 1);
	ALCdevice** data = (ALCdevice**)lua_newuserdata(L, sizeof(ALCdevice*));
	*data = device;
	return 1;
}

static int lua_alcCloseDevice(lua_State* L) {
	char obool = alcCloseDevice(*(ALCdevice**)luaL_checkuserdata(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}

static int lua_alcGetError(lua_State* L) {
	int oenum = alcGetError(*(ALCdevice**)luaL_checkuserdata(L, 1));
	lua_checkstack(L, 1);
	lua_pushnumber(L, oenum);
	return 1;
}

static int lua_alcIsExtensionPresent(lua_State* L) {
	char obool = alcIsExtensionPresent(*(ALCdevice**)luaL_checkuserdata(L, 1), luaL_checkstring(L, 2));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}


// NOT IMPLEMENTED >:O
// static int lua_alcGetProcAddress(lua_State* L) {
	// // unimp
	// return 0;
//}

static int lua_alcGetEnumValue(lua_State* L) {
	int oenum = alcGetEnumValue(*(ALCdevice**)luaL_checkuserdata(L, 1), luaL_checkstring(L, 2));
	lua_checkstack(L, 1);
	lua_pushnumber(L, oenum);
	return 1;
}

static int lua_alcGetString(lua_State* L) {
	const char* ostr = alcGetString(*(ALCdevice**)luaL_checkuserdata(L, 1), luaL_checknumber(L, 2));
	lua_checkstack(L, 1);
	lua_pushstring(L, ostr);
	return 1;
}

static int lua_alcGetIntegerv(lua_State* L) {
	int psize = luaL_checknumber(L, 3);
	luaL_checktable(L, 4);
	int* ints = 0;
	alcGetIntegerv(*(ALCdevice**)luaL_checkuserdata(L, 1), luaL_checknumber(L, 2), psize, ints);
	lua_checkstack(L, 1);
	lua_createtable(L, 0, psize);
	for(int i=0; i<psize; i++) {
		lua_pushnumber(L, ints[i]);
		lua_rawseti(L, -2, i+1);
	}
	return 1;
}

static int lua_alcCaptureOpenDevice(lua_State* L) {
	ALCdevice* device = alcCaptureOpenDevice(luaL_checkstring(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	lua_checkstack(L, 1);
	ALCdevice** data = (ALCdevice**)lua_newuserdata(L, sizeof(ALCdevice*));
	*data = device;
	return 1;
}

static int lua_alcCaptureCloseDevice(lua_State* L) {
	char obool = alcCaptureCloseDevice(*(ALCdevice**)luaL_checkuserdata(L, 1));
	lua_checkstack(L, 1);
	lua_pushboolean(L, obool);
	return 1;
}

static int lua_alcCaptureStart(lua_State* L) {
	alcCaptureStart(*(ALCdevice**)luaL_checkuserdata(L, 1));
	return 0;
}

static int lua_alcCaptureStop(lua_State* L) {
	alcCaptureStop(*(ALCdevice**)luaL_checkuserdata(L, 1));
	return 0;
}

static int lua_alcCaptureSamples(lua_State* L) {
	ALCdevice* device = *(ALCdevice**)luaL_checkuserdata(L, 1);
	int psamples = luaL_checknumber(L, 3);
	int samples = 0;
	alcGetIntegerv(device, ALC_CAPTURE_SAMPLES, 1, &samples);
	if(psamples <= samples)
		samples = psamples;
	char* obuffer = malloc(1 * samples * sizeof(char));
	alcCaptureSamples(device, obuffer, samples);
	lua_pushlstring(L, obuffer, 1 * samples * sizeof(char));
	free(obuffer);
	return 1;
}

// -----

static int lua_loadwav(lua_State* L) {
	const char* path = luaL_checkstring(L, 1);
	WaveData* w_data = wave_load(path);
	printf("Loaded wave file? %p\n", w_data);
	lua_checkstack(L, 1);
	
	lua_createtable(L, 5, 0);
	
	lua_pushnumber(L, w_data->channels);
	lua_setfield(L, -2, "channels");
	
	lua_pushnumber(L, w_data->bps);
	lua_setfield(L, -2, "bps");
	
	lua_pushnumber(L, w_data->sample_rate);
	lua_setfield(L, -2, "sample_rate");
	
	lua_pushnumber(L, w_data->sound_size);
	lua_setfield(L, -2, "sound_size");
	
	lua_pushlstring(L, (char*)w_data->sound_data, w_data->sound_size);
	lua_setfield(L, -2, "sound_data");
	
	wave_free(w_data);
	
	return 1;
}

// -----

typedef struct olual_CFReg {
	const char* const name;
	lua_CFunction cf;
} olual_CFReg;

typedef struct olual_CDReg {
	const char* const name;
	int data;
} olual_CDReg;


static const olual_CFReg al_funcs[57] = {
	{"alEnable", lua_alEnable},
	{"alDisable", lua_alDisable},
	{"alIsEnabled", lua_alIsEnabled},
	{"alGetString", lua_alGetString},
	{"alGetBooleanv", lua_alGetBooleanv},
	{"alGetIntegerv", lua_alGetIntegerv},
	{"alGetFloatv", lua_alGetFloatv},
	{"alGetDoublev", lua_alGetDoublev},
	{"alGetBoolean", lua_alGetBoolean},
	{"alGetInteger", lua_alGetInteger},
	{"alGetFloat", lua_alGetFloat},
	{"alGetDouble", lua_alGetDouble},
	{"alGetError", lua_alGetError},
	{"alIsExtensionPresent", lua_alIsExtensionPresent},
	{"alGetEnumValue", lua_alGetEnumValue},
	{"alListenerf", lua_alListenerf},
	{"alListener3f", lua_alListener3f},
	{"alListener6f", lua_alListener6f},
	{"alListeneri", lua_alListeneri},
	{"alListener3i", lua_alListener3i},
	{"alGetListenerf", lua_alGetListenerf},
	{"alGetListener3f", lua_alGetListener3f},
	{"alGetListeneri", lua_alGetListeneri},
	{"alGetListener3i", lua_alGetListener3i},
	{"alGenSources", lua_alGenSources},
	{"alDeleteSources", lua_alDeleteSources},
	{"alIsSource", lua_alIsSource},
	{"alSourcef", lua_alSourcef},
	{"alSource3f", lua_alSource3f},
	{"alSourcei", lua_alSourcei},
	{"alSource3i", lua_alSource3i},
	{"alGetSourcef", lua_alGetSourcef},
	{"alGetSource3f", lua_alGetSource3f},
	{"alGetSourcei", lua_alGetSourcei},
	{"alGetSource3i", lua_alGetSource3i},
	{"alSourcePlay", lua_alSourcePlay},
	{"alSourceStop", lua_alSourceStop},
	{"alSourceRewind", lua_alSourceRewind},
	{"alSourcePause", lua_alSourcePause},
	{"alSourceQueueBuffers", lua_alSourceQueueBuffers},
	{"alSourceUnqueueBuffers", lua_alSourceUnqueueBuffers},
	{"alGenBuffers", lua_alGenBuffers},
	{"alDeleteBuffers", lua_alDeleteBuffers},
	{"alIsBuffer", lua_alIsBuffer},
	{"alBufferData", lua_alBufferData},
	{"alBufferf", lua_alBufferf},
	{"alBuffer3f", lua_alBuffer3f},
	{"alBufferi", lua_alBufferi},
	{"alBuffer3i", lua_alBuffer3i},
	{"alGetBufferf", lua_alGetBufferf},
	{"alGetBuffer3f", lua_alGetBuffer3f},
	{"alGetBufferi", lua_alGetBufferi},
	{"alGetBuffer3i", lua_alGetBuffer3i},
	{"alDopplerFactor", lua_alDopplerFactor},
	{"alDopplerVelocity", lua_alDopplerVelocity},
	{"alSpeedOfSound", lua_alSpeedOfSound},
	{"alDistanceModel", lua_alDistanceModel}
};

static const olual_CFReg alc_funcs[19] = {
	{"alcCreateContext", lua_alcCreateContext},
	{"alcMakeContextCurrent", lua_alcMakeContextCurrent},
	{"alcProcessContext", lua_alcProcessContext},
	{"alcSuspendContext", lua_alcSuspendContext},
	{"alcDestroyContext", lua_alcDestroyContext},
	{"alcGetCurrentContext", lua_alcGetCurrentContext},
	{"alcGetContextsDevice", lua_alcGetContextsDevice},
	{"alcOpenDevice", lua_alcOpenDevice},
	{"alcCloseDevice", lua_alcCloseDevice},
	{"alcGetError", lua_alcGetError},
	{"alcIsExtensionPresent", lua_alcIsExtensionPresent},
	{"alcGetEnumValue", lua_alcGetEnumValue},
	{"alcGetString", lua_alcGetString},
	{"alcGetIntegerv", lua_alcGetIntegerv},
	{"alcCaptureOpenDevice", lua_alcCaptureOpenDevice},
	{"alcCaptureCloseDevice", lua_alcCaptureCloseDevice},
	{"alcCaptureStart", lua_alcCaptureStart},
	{"alcCaptureStop", lua_alcCaptureStop},
	{"alcCaptureSamples", lua_alcCaptureSamples}
};


static const olual_CDReg al_consts[72] = {
	{"AL_INVALID", -1},
	{"AL_NONE", 0},
	{"AL_FALSE", 0},
	{"AL_TRUE", 1},
	{"AL_SOURCE_RELATIVE", 0x202},
	{"AL_CONE_INNER_ANGLE", 0x1001},
	{"AL_CONE_OUTER_ANGLE", 0x1002},
	{"AL_PITCH", 0x1003},
	{"AL_POSITION", 0x1004},
	{"AL_DIRECTION", 0x1005},
	{"AL_VELOCITY", 0x1006},
	{"AL_LOOPING", 0x1007},
	{"AL_BUFFER", 0x1009},
	{"AL_GAIN", 0x100A},
	{"AL_MIN_GAIN", 0x100D},
	{"AL_MAX_GAIN", 0x100E},
	{"AL_ORIENTATION", 0x100F},
	{"AL_CHANNEL_MASK", 0x3000},
	{"AL_SOURCE_STATE", 0x1010},
	{"AL_INITIAL", 0x1011},
	{"AL_PLAYING", 0x1012},
	{"AL_PAUSED", 0x1013},
	{"AL_STOPPED", 0x1014},
	{"AL_BUFFERS_QUEUED", 0x1015},
	{"AL_BUFFERS_PROCESSED", 0x1016},
	{"AL_SEC_OFFSET", 0x1024},
	{"AL_SAMPLE_OFFSET", 0x1025},
	{"AL_BYTE_OFFSET", 0x1026},
	{"AL_SOURCE_TYPE", 0x1027},
	{"AL_STATIC", 0x1028},
	{"AL_STREAMING", 0x1029},
	{"AL_UNDETERMINED", 0x1030},
	{"AL_FORMAT_MONO8", 0x1100},
	{"AL_FORMAT_MONO16", 0x1101},
	{"AL_FORMAT_STEREO8", 0x1102},
	{"AL_FORMAT_STEREO16", 0x1103},
	{"AL_REFERENCE_DISTANCE", 0x1020},
	{"AL_ROLLOFF_FACTOR", 0x1021},
	{"AL_CONE_OUTER_GAIN", 0x1022},
	{"AL_MAX_DISTANCE", 0x1023},
	{"AL_FREQUENCY", 0x2001},
	{"AL_BITS", 0x2002},
	{"AL_CHANNELS", 0x2003},
	{"AL_SIZE", 0x2004},
	{"AL_UNUSED", 0x2010},
	{"AL_PENDING", 0x2011},
	{"AL_PROCESSED", 0x2012},
	{"AL_NO_ERROR", 0},
	{"AL_INVALID_NAME", 0xA001},
	{"AL_ILLEGAL_ENUM", 0xA002},
	{"AL_INVALID_ENUM", 0xA002},
	{"AL_INVALID_VALUE", 0xA003},
	{"AL_ILLEGAL_COMMAND", 0xA004},
	{"AL_INVALID_OPERATION", 0xA004},
	{"AL_OUT_OF_MEMORY", 0xA005},
	{"AL_VENDOR", 0xB001},
	{"AL_VERSION", 0xB002},
	{"AL_RENDERER", 0xB003},
	{"AL_EXTENSIONS", 0xB004},
	{"AL_DOPPLER_FACTOR", 0xC000},
	{"AL_DOPPLER_VELOCITY", 0xC001},
	{"AL_SPEED_OF_SOUND", 0xC003},
	{"AL_DISTANCE_MODEL", 0xD000},
	{"AL_INVERSE_DISTANCE", 0xD001},
	{"AL_INVERSE_DISTANCE_CLAMPED", 0xD002},
	{"AL_LINEAR_DISTANCE", 0xD003},
	{"AL_LINEAR_DISTANCE_CLAMPED", 0xD004},
	{"AL_EXPONENT_DISTANCE", 0xD005},
	{"AL_EXPONENT_DISTANCE_CLAMPED", 0xD006},
	{"AL_VERSION_1_0", 1},
	{"AL_VERSION_1_1", 1},
	{"AL_NONE", 0}
};

static const olual_CDReg alc_consts[27] = {
	{"ALC_INVALID", 0},
	{"ALC_VERSION_0_1", 1},
	{"ALC_FALSE", 0},
	{"ALC_TRUE", 1},
	{"ALC_FREQUENCY", 0x1007},
	{"ALC_REFRESH", 0x1008},
	{"ALC_SYNC", 0x1009},
	{"ALC_MONO_SOURCES", 0x1010},
	{"ALC_STEREO_SOURCES", 0x1011},
	{"ALC_NO_ERROR", 0},
	{"ALC_INVALID_DEVICE", 0xA001},
	{"ALC_INVALID_CONTEXT", 0xA002},
	{"ALC_INVALID_ENUM", 0xA003},
	{"ALC_INVALID_VALUE", 0xA004},
	{"ALC_OUT_OF_MEMORY", 0xA005},
	{"ALC_DEFAULT_DEVICE_SPECIFIER", 0x1004},
	{"ALC_DEVICE_SPECIFIER", 0x1005},
	{"ALC_EXTENSIONS", 0x1006},
	{"ALC_MAJOR_VERSION", 0x1000},
	{"ALC_MINOR_VERSION", 0x1001},
	{"ALC_ATTRIBUTES_SIZE", 0x1002},
	{"ALC_ALL_ATTRIBUTES", 0x1003},
	{"ALC_DEFAULT_ALL_DEVICES_SPECIFIER", 0x1012},
	{"ALC_ALL_DEVICES_SPECIFIER", 0x1013},
	{"ALC_CAPTURE_DEVICE_SPECIFIER", 0x310},
	{"ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER", 0x311},
	{"ALC_CAPTURE_SAMPLES", 0x312}
};



LUA_DLL_ENTRY luaopen_libopenlual(lua_State* L)
{
	
	lua_createtable(L, 0, 57+19+72+27);
	
	lua_pushcfunction(L, lua_loadwav);
	lua_setfield(L, -2, "loadwav");
	
	for(size_t i=0; i<57; i++) {
		lua_pushcfunction(L, al_funcs[i].cf);
		lua_setfield(L, -2, al_funcs[i].name);
	}
	for(size_t i=0; i<19; i++) {
		lua_pushcfunction(L, alc_funcs[i].cf);
		lua_setfield(L, -2, alc_funcs[i].name);
	}
	
	for(size_t i=0; i<72; i++) {
		lua_pushnumber(L, al_consts[i].data);
		lua_setfield(L, -2, al_consts[i].name);
	}
	for(size_t i=0; i<27; i++) {
		lua_pushnumber(L, alc_consts[i].data);
		lua_setfield(L, -2, alc_consts[i].name);
	}
	
	return 1;
}

