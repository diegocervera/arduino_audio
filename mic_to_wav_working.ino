/*
  This example reads audio data from the on-board PDM microphones, and prints
  out the samples to the Serial console. The Serial Plotter built into the
  Arduino IDE can be used to plot the audio data (Tools -> Serial Plotter)

  Circuit:
  - Arduino Nano 33 BLE board, or
  - Arduino Nano RP2040 Connect, or
  - Arduino Portenta H7 board plus Portenta Vision Shield, or
  - Arduino Nicla Vision

  This example code is in the public domain.
*/

#include <PDM.h>
#include <SdFat.h>

// Define pins for SD card communication (adjust if needed)
#define SD_CS_PIN 10  // Chip Select pin for the SD card module

// SD card object
SdFat SD;

// File object for the audio file
File audioFile;

// default number of output channels
static const char channels = 1;

// default PCM output frequency
static const int frequency = 16000;

// Buffer to read samples into, each sample is 16-bits
short sampleBuffer[512];
const char* filename = "audio.wav"; // Save as WAV file
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

unsigned long totalBytes = 0;

// Number of audio samples read
volatile int samplesRead;

unsigned int counter = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Configure the data receive callback
  PDM.onReceive(onPDMdata);

  // Optionally set the gain
  // Defaults to 20 on the BLE Sense and 24 on the Portenta Vision Shield
  // PDM.setGain(30);

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
  // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shield
  if (!PDM.begin(channels, frequency)) {
    Serial.println("Failed to start PDM!");
    while (1);
  }

    // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("SD card initialization failed!");
    while (1); // Stop execution if SD card fails
  }
  Serial.println("SD card initialized.");
    // Delete existing file (optional)
  if (SD.exists(filename)) {
    SD.remove(filename);
  }

  // Open audio file for writing
  audioFile = SD.open(filename, FILE_WRITE);
  if (!audioFile) {
    Serial.println("Error opening audio file!");
    while (1); // Stop if file open fails
  }
  Serial.print("Recording to: ");
  Serial.println(filename);

  wav_header header;
  header.num_channels = channels;
  header.sample_rate = frequency;
  header.byte_rate = frequency * channels * 2;
  header.block_align = channels * 2;
  audioFile.write((const uint8_t*)&header, sizeof(header));
  Serial.println("WAV Header Written");
}

void loop() {
  // Wait for samples to be read
  if (samplesRead) {

    // Write the audio data to the SD card
    audioFile.write((const uint8_t*)sampleBuffer, samplesRead * 2); // *2 because short is 2 bytes
    totalBytes += samplesRead * 2;

    // Clear the read count
    // Print samples to the serial monitor or plotter
    //for (int i = 0; i < samplesRead; i++) {
      
    //  if(channels == 2) {
    //    Serial.print("L:");
    //    Serial.print(sampleBuffer[i]);
    //    Serial.print(" R:");
    //    i++;
    //  }
    //  Serial.println(sampleBuffer[i]);
    //}

    // Clear the read count
    samplesRead = 0;
  }

  if (counter > 10000000) {
    Serial.println("Stopping recording...");

    // Update WAV header with correct sizes
    wav_header header;
    header.riff_chunk_size = 36 + totalBytes;
    header.data_chunk_size = totalBytes;

    // Seek to the beginning of the file to overwrite the header
    audioFile.seek(0);
    audioFile.write((const uint8_t*)&header, sizeof(header));
    Serial.println("WAV Header Updated");

    audioFile.close(); // Close the file
    Serial.println("Recording stopped and file saved.");
    PDM.end();  // Stop PDM
    while(1);
  }
  counter++;
}

/**
 * Callback function to process the data from the PDM microphone.
 * NOTE: This callback is executed as part of an ISR.
 * Therefore using `Serial` to print messages inside this function isn't supported.
 * */
void onPDMdata() {
  // Query the number of available bytes
  int bytesAvailable = PDM.available();

  // Read into the sample buffer
  PDM.read(sampleBuffer, bytesAvailable);

  // 16-bit, 2 bytes per sample
  samplesRead = bytesAvailable / 2;
}