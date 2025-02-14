#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SdFat.h>
#include <stdint.h>



// default number of output channels
extern const char channels;

// default PCM output frequency
extern const int frequency;

// Buffer to read samples into, each sample is 16-bits
const int BUFFER_SIZE = 512;
extern short sampleBuffer[BUFFER_SIZE];

// Number of audio samples read
extern volatile int samplesRead;

// SD card object
extern SdFat SD;


struct wav_header {
  char riff_header[4] = {'R', 'I', 'F', 'F'};
  uint32_t riff_chunk_size;
  char wave_header[4] = {'W', 'A', 'V', 'E'};
  char fmt_header[4] = {'f', 'm', 't', ' '};
  uint32_t fmt_chunk_size = 16;
  uint16_t audio_format = 1; // PCM = 1
  uint16_t num_channels = channels;
  uint32_t sample_rate = frequency;
  uint32_t byte_rate = frequency * channels * 2;        // SampleRate * NumChannels * BitsPerSample/8
  uint16_t block_align = channels * 2;       // NumChannels * BitsPerSample/8
  uint16_t bits_per_sample = 16;
  char data_header[4] = {'d', 'a', 't', 'a'};
  uint32_t data_chunk_size;
};




#endif