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

// Function to be called on long press
void LongPressFunction() {
  if (!buttonProperties.isInWhiteMode) {
    buttonProperties.lastColourIndex = buttonProperties.ColourIndex;
    setColour(0); // White
    buttonProperties.isInWhiteMode = true;
    // println("Setting to White");
    Serial.println("Started recording.");
  }
}

void ShortPressFunction() {
  if (buttonProperties.isInWhiteMode) {
    setColour(buttonProperties.lastColourIndex);
    buttonProperties.isInWhiteMode = false;
    println("Stopped recording.");
    print("Current user: ");
    println(buttonProperties.lastColourIndex);
  } else {
    buttonProperties.ColourIndex = (buttonProperties.ColourIndex % 3) + 1;
    setColour(buttonProperties.ColourIndex);
    print("Switched to user: ");
    println(buttonProperties.ColourIndex);
  }
}

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
  buttonProperties.onLongPress = LongPressFunction;
  buttonProperties.onShortPress = ShortPressFunction;


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
