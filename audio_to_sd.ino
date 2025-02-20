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


void onSwitchUser(){
  print("Switched to user: ");
  println(buttonProperties.ColourIndex);
}
void onStartRecording(){
  print("Current user: ");
  println(buttonProperties.lastColourIndex);
  println("Started recording.");
}

void onStopRecording(){
  println("Stopped recording.");
  print("Current user: ");
  println(buttonProperties.ColourIndex);
}

void setup() {

  initSerialConnection();
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);

  initPDM();
  initSD();

  const char* filename = "audio.wav";
  audioFile = openAudioFile(filename);

  writeWAVHeader(audioFile);
  println("Initialisation sucessful.");
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);

  pinMode(buttonProperties.buttonPin, INPUT_PULLUP);
  buttonProperties.onSwitchUser = onSwitchUser;
  buttonProperties.onStartRecording = onStartRecording;
  buttonProperties.onStopRecording = onStopRecording;
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
