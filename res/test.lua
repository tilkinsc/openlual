--[[
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
--]]

-- package.loadlib("bin\\Release\\libopenlual.dll", "luaopen_libopenlual")()

print("Setting up context...")

local device = al.alcOpenDevice(nil)
local context = al.alcCreateContext(device, nil)
al.alcMakeContextCurrent(context)


print("Generating buffer...")

local albuf = al.alGenBuffers(1);


print("Loading wave...")

local wd = wave.loadwav("monody.wav")
for i, v in next, wd do
	if(i ~= "sound_data")then
		print(i, v)
	end
end


local fmt = wd.channels == 1
	and (wd.bps == 8 and al.AL_FORMAT_MONO8 or al.AL_FORMAT_MONO16)
	or (wd.bps == 8 and al.AL_FORMAT_STEREO8 or al.AL_FORMAT_STEREO16)
print("Chosen fmt", fmt)

print("Appending buffer data...")

al.alBufferData(albuf[1], fmt, wd.sound_data, wd.sound_size, wd.sample_rate)


print("Setting up listener...")

al.alDistanceModel(al.AL_INVERSE_DISTANCE)

local sl_ori = {0, 0, -1, 0, 1, 0}
local sl_pos = {0, 0, 0}
local sl_vel = {0, 0, 0}

al.alListener6f(al.AL_ORIENTATION, sl_ori[1], sl_ori[2], sl_ori[3], sl_ori[4], sl_ori[5], sl_ori[6])
al.alListener3f(al.AL_POSITION, sl_pos[1], sl_pos[2], sl_pos[3])
al.alListener3f(al.AL_VELOCITY, sl_vel[1], sl_vel[2], sl_vel[3])
al.alListenerf(al.AL_GAIN, 1)



local ss_vel = {0, 0, 0}
local ss_pos = {0, 0, 0}
local ss_dir = {0, 1, 0}

print("Generating source...")

local alsrc = al.alGenSources(1)

print("Loading source params")

al.alSourcef(alsrc[1], al.AL_ROLLOFF_FACTOR,		1)
al.alSourcef(alsrc[1], al.AL_REFERENCE_DISTANCE,	0)
al.alSourcef(alsrc[1], al.AL_MAX_DISTANCE,			10)
al.alSourcef(alsrc[1], al.AL_PITCH,					1)
al.alSourcef(alsrc[1], al.AL_GAIN,					1)
al.alSourcef(alsrc[1], al.AL_LOOPING,				al.AL_TRUE)

al.alSource3f(alsrc[1], al.AL_VELOCITY,		ss_vel[1], ss_vel[2], ss_vel[3])
al.alSource3f(alsrc[1], al.AL_POSITION,		ss_pos[1], ss_pos[2], ss_pos[3])
al.alSource3f(alsrc[1], al.AL_DIRECTION,	ss_dir[1], ss_dir[2], ss_dir[3])

al.alSourcei(alsrc[1], al.AL_BUFFER, albuf[1])

al.alSourcePlay(alsrc[1])
