#include <PDM.h>
// #include <SdFat.h>
#include "pdm_functions.h"
#include "init_functions.h"
#include "constants.h"
#include "sd_functions.h"
#include "button_functions.h"

// Define push button pin
// #define BUTTON_PIN 2  // Change if using a different pin

unsigned long totalBytes = 0;
unsigned int counter = 0;
File audioFile;

ButtonProperties buttonProperties;

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

  pinMode(buttonProperties.buttonPin, INPUT_PULLUP);

}

void loop() {

  buttonListener(buttonProperties);

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
