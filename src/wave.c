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

#include "wave.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// loads data stored in wave file into a struct, returns said struct
WaveData* wave_load(const char* path) {
	
	WaveData* data = 0;
	unsigned char* buffer = 0;
	
  // open and read all file contents
	FILE* f = 0;
	if((f = fopen(path, "rb")) == 0) {
		puts("Could not open file.");
		goto exit;
	}
	
	fseek(f, 0, SEEK_END);
	long file_size = ftell(f);
	fseek(f, 0, SEEK_SET);
	
	buffer = malloc(file_size * sizeof(unsigned char));
	if(buffer == 0) {
		puts("Could not allocate memory.");
		fclose(f);
		goto exit;
	}
	fread(buffer, 1, file_size, f);
	fclose(f);
	
  // ensure the file type is a wave file, identified by RIFF
	if(memcmp(buffer, "RIFF", 4) != 0) {
		puts("Invalid file header!");
		goto exit;
	}
	
  // pull out different useful data parameters from buffer
	unsigned int channels = buffer[22] | (buffer[23] << 8);
	unsigned int sample_rate = buffer[24] | (buffer[25] << 8) | (buffer[26] << 16) | (buffer[27] << 24);
	unsigned int bps = buffer[34] | (buffer[35] << 8);
	
  // skip `useless to us` chunks
	size_t chunk_offset = 36;
	while(memcmp(buffer + chunk_offset, "data", 4) != 0) {
		chunk_offset += 4;
		chunk_offset += buffer[chunk_offset] | (buffer[chunk_offset+1] << 8) | (buffer[chunk_offset+2] << 16) | (buffer[chunk_offset+3] << 24);
		chunk_offset += 4;
	}
	unsigned char* b_data = &(buffer[chunk_offset+8]);
	
	data = malloc(1 * sizeof(WaveData));
	if(data == 0) {
		puts("Could not allocate memory.");
		goto exit;
	}
	data->channels = channels;
	data->bps = bps;
	data->sample_rate = sample_rate;
	data->sound_size = file_size - chunk_offset;
	data->data = buffer; // this needs to be free'd; it is leaked and cleaned by wave_free()
	data->sound_data = b_data;
	
	return data;
	
exit:
	free(buffer);
	free(data);
	return data;
}


void wave_free(void* wd) {
	WaveData* wavedata = (WaveData*) wd;
	
	free(wavedata->data);
	free(wavedata);
}
