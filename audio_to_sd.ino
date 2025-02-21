#include <PDM.h>
// #include <SdFat.h>
#include "pdm_functions.h"
#include "init_functions.h"
#include "constants.h"
#include "sd_functions.h"
#include "button_functions.h"
#include <string>
#include <vector>


unsigned long totalBytes = 0;
File audioFile;

ButtonProperties buttonProperties;

static int file_counter = 0;
static std::vector<int> user_files(3, 0);


void onSwitchUser(){
  print("Switched to user: ");
  println(buttonProperties.ColourIndex);
}
void onStartRecording(){
  print("Current user: ");
  println(buttonProperties.lastColourIndex);
  std::string filename_str = std::to_string(buttonProperties.lastColourIndex) + "_" + std::to_string(user_files[buttonProperties.lastColourIndex - 1]) + ".wav";
  const char* filename = filename_str.c_str();
  audioFile = openAudioFile(filename);
  writeWAVHeader(audioFile);
  user_files[buttonProperties.lastColourIndex - 1] += 1;
  println("Started recording.");
}

void onStopRecording(){
  closeWAVFile(audioFile, totalBytes);
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
}
