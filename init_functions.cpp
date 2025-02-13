#include <PDM.h>
#include "Arduino.h"
#include "pdm_functions.h"
#include "constants.h"



bool serialInitialised = false;


void println(const char* message) {
  if (serialInitialised) {
    Serial.println(message);
  }
}

void println(int number) {
  if (serialInitialised) {
    Serial.println(number);
  }
}

void print(const char* message) {
  if (serialInitialised) {
    Serial.print(message);
  }
}

void print(int number) {
  if (serialInitialised) {
    Serial.print(number);
  }
}



void initSerialConnection() {
  delay(2000);
  if (Serial) {
    Serial.begin(9600);
    serialInitialised = true;
  } else {
    println("Serial not initialised. No Serial Monitor Detected.");
  }
}

void initPDM() {
   PDM.onReceive(onPDMdata);

  // Optionally set the gain
  // Defaults to 20 on the BLE Sense and 24 on the Portenta Vision Shield
  // PDM.setGain(30);

  // Initialize PDM with:
  // - one channel (mono mode)
  // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
  // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shield
  if (!PDM.begin(channels, frequency)) {
    println("Failed to start PDM!");
    while (1);
  }
}
