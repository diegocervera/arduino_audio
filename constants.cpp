#include "constants.h"

const char channels = 1;
const int frequency = 16000;
short sampleBuffer[BUFFER_SIZE];
volatile int samplesRead = 0;

const char* filename = "audio.wav";
File audioFile;
SdFat SD;
