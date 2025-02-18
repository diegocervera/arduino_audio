#include <PDM.h>
#include <SdFat.h>
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



