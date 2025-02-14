#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H

#include <SdFat.h>

File openAudioFile(const char* filename);
void writeWAVHeader(File &audiofile);


#endif