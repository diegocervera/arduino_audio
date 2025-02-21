#include "sd_functions.h"
#include "init_functions.h"
#include "constants.h"
#include <SdFat.h>

// Define pins for SD card communication (adjust if needed)
#define SD_CS_PIN 10  // Chip Select pin for the SD card module

SdFat SD;

void initSD() {
  if (!SD.begin(SD_CS_PIN)) {
    println("SD card initialization failed!");
    problemOnInit();
    while (1); 
  }
  println("SD card initialised.");
}


File openAudioFile(const char* filename) {
  // Open audio file for writing
  if (SD.exists(filename)) {
    SD.remove(filename);
  }

  File audioFile = SD.open(filename, FILE_WRITE);
  if (!audioFile) {
  println("Error opening audio file!");
  problemOnInit();
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

void writeSamples(File &audioFile, volatile int &samplesRead, unsigned long &totalBytes){

    // Write the audio data to the SD card
    audioFile.write((const uint8_t*)sampleBuffer, samplesRead * 2); // *2 because short is 2 bytes
    totalBytes += samplesRead * 2;

    // Clear the read count
    samplesRead = 0;
}

void closeWAVFile(File &audioFile, unsigned long &totalBytes){
  wav_header header;
  header.riff_chunk_size = 36 + totalBytes;
  header.data_chunk_size = totalBytes;

  // Seek to the beginning of the file to overwrite the header
  audioFile.seek(0);
  audioFile.write((const uint8_t*)&header, sizeof(header));
  audioFile.close(); // Close the file

  println("Recording stopped and file saved.");
  println("WAV Header Updated");
}