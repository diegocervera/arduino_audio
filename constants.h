#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SdFat.h>



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

// File name
extern const char* filename;

// File object for the audio file
extern File audioFile;




#endif