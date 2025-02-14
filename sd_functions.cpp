#include "sd_functions.h"
#include "init_functions.h"
#include "constants.h"
#include <SdFat.h>


File openAudioFile(const char* filename) {
  // Open audio file for writing
  if (SD.exists(filename)) {
    SD.remove(filename);
  }

  File audioFile = SD.open(filename, FILE_WRITE);
  if (!audioFile) {
  println("Error opening audio file!");
  while (1); // Stop if file open fails
  }
  print("Recording to: ");
  println(filename);
  return audioFile;
}

void writeWAVHeader(File &audioFile){
  wav_header header;
  header.num_channels = channels;
  header.sample_rate = frequency;
  header.byte_rate = frequency * channels * 2;
  header.block_align = channels * 2;
  audioFile.write((const uint8_t*)&header, sizeof(header));
  println("WAV Header Written");
}