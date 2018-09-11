
#pragma once

typedef struct WaveData {
	unsigned int channels;
	unsigned int bps;
	unsigned int sample_rate;
	unsigned int sound_size;
	unsigned char* data;
	unsigned char* sound_data;
} WaveData;

WaveData* wave_load(const char* path);

void wave_free(void* wd);
