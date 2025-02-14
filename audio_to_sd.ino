#include <PDM.h>
#include <SdFat.h>
#include "pdm_functions.h"
#include "init_functions.h"
#include "constants.h"
#include "sd_functions.h"

unsigned long totalBytes = 0;
unsigned int counter = 0;
File audioFile;

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

}

void loop() {
  // Wait for samples to be read
  if (samplesRead) {
    writeSamples(audioFile, samplesRead, totalBytes);
  }

  if (counter > 5000000) {
    println("Stopping recording...");
    closeWAVFile(audioFile, totalBytes);

    PDM.end();  // Stop PDM
    while(1);
  }
  counter++;
}
