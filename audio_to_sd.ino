#include <PDM.h>
// #include <SdFat.h>
#include "pdm_functions.h"
#include "init_functions.h"
#include "constants.h"
#include "sd_functions.h"

// Define push button pin
#define BUTTON_PIN 2  // Change if using a different pin

unsigned long totalBytes = 0;
unsigned int counter = 0;
File audioFile;

bool buttonState = HIGH;        // Current state of the button (starts HIGH because of pull-up resistor)
int colorIndex = 0;  // Tracks current color
int lastColorIndex = 0; // Stores the original color before long press
unsigned long lastButtonPressTime = 0;  // For debounce timing
unsigned long buttonPressStartTime = 0; // Track when button press starts
unsigned long debounceDelay = 50;  // Minimum time between button presses (50ms)
unsigned long longPressDuration = 1000;  // Duration for long press detection (1 second)
bool lastButtonState = HIGH; // Last button state
bool buttonPressed = false;  // Flag for button press
bool isInWhiteMode = false;  // Flag to track if LED is in white mode


void setColor(int index) {
  switch (index) {
    case 0: digitalWrite(LEDR, LOW); digitalWrite(LEDG, LOW); digitalWrite(LEDB, LOW); break; // White
    case 1: digitalWrite(LEDR, LOW); digitalWrite(LEDG, HIGH); digitalWrite(LEDB, HIGH); break; // Red
    case 2: digitalWrite(LEDR, HIGH); digitalWrite(LEDG, LOW); digitalWrite(LEDB, HIGH); break; // Green
    case 3: digitalWrite(LEDR, HIGH); digitalWrite(LEDG, HIGH); digitalWrite(LEDB, LOW); break; // Blue
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

  pinMode(BUTTON_PIN, INPUT_PULLUP);

}

void loop() {

  // Read current button state
  buttonState = digitalRead(BUTTON_PIN);

  // Debounce and detect button press
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Button just pressed
    buttonPressed = true;
    buttonPressStartTime = millis();
    Serial.println("Button Pressed");
  }

  //Detect button release
  if (buttonState == HIGH && lastButtonState == LOW && buttonPressed) {
    // Button released after being pressed
    unsigned long pressDuration = millis() - buttonPressStartTime;
    Serial.print("Button Released. Duration: ");
    Serial.println(pressDuration);

    if (pressDuration > debounceDelay) { // Filter out noise
      if (pressDuration < longPressDuration) {
        // Short Press
        Serial.println("Short Press Detected");
        if (isInWhiteMode) {
          setColor(lastColorIndex);
          isInWhiteMode = false;
          Serial.print("Restoring color: ");
          Serial.println(lastColorIndex);
        } else {
          colorIndex = (colorIndex % 3) + 1;
          setColor(colorIndex);
          Serial.print("Changing color to: ");
          Serial.println(colorIndex);
        }
      } else {
        // Long Press
        Serial.println("Long Press Detected");
        if (!isInWhiteMode) {
          lastColorIndex = colorIndex;
          setColor(0); // White
          isInWhiteMode = true;
          Serial.println("Setting to White");
        }
      }
      buttonPressed = false; // Clear buttonPressed flag *after* processing
    } else {
      Serial.println("Debounced Release");
      buttonPressed = false; //Ensure button pressed is reset for debounced events.
    }
  }

  lastButtonState = buttonState; // Update last button state

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
