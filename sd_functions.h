#ifndef SD_FUNCTIONS_H
#define SD_FUNCTIONS_H

#include <SdFat.h>

void initSD();
File openAudioFile(const char* filename);
void writeWAVHeader(File &audiofile);
void writeSamples(File &audioFile, volatile int &samplesRead, unsigned long &totalBytes);
void closeWAVFile(File &audioFile, unsigned long &totalBytes);

#endif