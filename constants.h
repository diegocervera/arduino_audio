#ifndef CONSTANTS_H
#define CONSTANTS_H



// default number of output channels
extern const char channels;

// default PCM output frequency
extern const int frequency;

// Buffer to read samples into, each sample is 16-bits
const int BUFFER_SIZE = 512;
extern short sampleBuffer[BUFFER_SIZE];

// Number of audio samples read
extern volatile int samplesRead;



#endif