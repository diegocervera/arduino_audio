#include <PDM.h>
#include <SdFat.h>
#include "pdm_functions.h"
#include "init_functions.h"
#include "constants.h"
#include "sd_functions.h"


// // Define pins for SD card communication (adjust if needed)
// #define SD_CS_PIN 10  // Chip Select pin for the SD card module

// // SD card object
// SdFat SD;

// File object for the audio file
// File audioFile;

// const char* filename = "audio.wav"; // Save as WAV file
// struct wav_header {
//   char riff_header[4] = {'R', 'I', 'F', 'F'};
//   uint32_t riff_chunk_size;
//   char wave_header[4] = {'W', 'A', 'V', 'E'};
//   char fmt_header[4] = {'f', 'm', 't', ' '};
//   uint32_t fmt_chunk_size = 16;
//   uint16_t audio_format = 1; // PCM = 1
//   uint16_t num_channels = channels;
//   uint32_t sample_rate = frequency;
//   uint32_t byte_rate = frequency * channels * 2;        // SampleRate * NumChannels * BitsPerSample/8
//   uint16_t block_align = channels * 2;       // NumChannels * BitsPerSample/8
//   uint16_t bits_per_sample = 16;
//   char data_header[4] = {'d', 'a', 't', 'a'};
//   uint32_t data_chunk_size;
// };

unsigned long totalBytes = 0;
unsigned int counter = 0;

void setup() {

  initSerialConnection();
  pinMode(LEDB, OUTPUT);

  initPDM();
  initSD();

  const char* filename = "audio.wav";
  audioFile = openAudioFile(filename);

  writeWAVHeader(audioFile);
  println("Initialisation sucessful.");
  digitalWrite(LEDB, LOW);

  // Open audio file for writing
  // audioFile = SD.open(filename, FILE_WRITE);
  // if (!audioFile) {
  //   println("Error opening audio file!");
  //   while (1); // Stop if file open fails
  // }
  // print("Recording to: ");
  // println(filename);


  // wav_header header;
  // header.num_channels = channels;
  // header.sample_rate = frequency;
  // header.byte_rate = frequency * channels * 2;
  // header.block_align = channels * 2;
  // audioFile.write((const uint8_t*)&header, sizeof(header));
  // println("WAV Header Written");


}

void loop() {
  // Wait for samples to be read
  if (samplesRead) {

    // Write the audio data to the SD card
    audioFile.write((const uint8_t*)sampleBuffer, samplesRead * 2); // *2 because short is 2 bytes
    totalBytes += samplesRead * 2;

    // Clear the read count
    samplesRead = 0;
  }

  if (counter > 5000000) {
    println("Stopping recording...");

    // Update WAV header with correct sizes
    wav_header header;
    header.riff_chunk_size = 36 + totalBytes;
    header.data_chunk_size = totalBytes;

    // Seek to the beginning of the file to overwrite the header
    audioFile.seek(0);
    audioFile.write((const uint8_t*)&header, sizeof(header));
    audioFile.close(); // Close the file

    println("Recording stopped and file saved.");
    println("WAV Header Updated");

    PDM.end();  // Stop PDM
    while(1);
  }
  counter++;
}
