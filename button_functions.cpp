#include "Arduino.h"
#include "button_functions.h"
#include "init_functions.h"

void setColour(int index) {
  switch (index) {
    case 0: digitalWrite(LEDR, LOW); digitalWrite(LEDG, LOW); digitalWrite(LEDB, LOW); break; // White
    case 1: digitalWrite(LEDR, LOW); digitalWrite(LEDG, HIGH); digitalWrite(LEDB, HIGH); break; // Red
    case 2: digitalWrite(LEDR, HIGH); digitalWrite(LEDG, LOW); digitalWrite(LEDB, HIGH); break; // Green
    case 3: digitalWrite(LEDR, HIGH); digitalWrite(LEDG, HIGH); digitalWrite(LEDB, LOW); break; // Blue
  }
}

void buttonListener(ButtonProperties &buttonProperties) {
  // Read current button state
  buttonProperties.buttonState = digitalRead(buttonProperties.buttonPin);

  // Debounce and detect button press
  if (buttonProperties.buttonState == LOW && buttonProperties.lastButtonState == HIGH) {
    // Button just pressed
    buttonProperties.buttonPressed = true;
    buttonProperties.buttonPressStartTime = millis();
  }

  //Detect button release
  if (buttonProperties.buttonState == HIGH && buttonProperties.lastButtonState == LOW && buttonProperties.buttonPressed) {
    // Button released after being pressed
    unsigned long pressDuration = millis() - buttonProperties.buttonPressStartTime;

    if (pressDuration > buttonProperties.debounceDelay) { // Filter out noise
      if (pressDuration < buttonProperties.longPressDuration) {
        // Short Press
        if (buttonProperties.isInWhiteMode) {
          setColour(buttonProperties.lastColourIndex);
          buttonProperties.isInWhiteMode = false;
          if (buttonProperties.onStopRecording){
            buttonProperties.onStopRecording();
          }

        } else {
          buttonProperties.ColourIndex = (buttonProperties.ColourIndex % 3) + 1;
          setColour(buttonProperties.ColourIndex);
          if (buttonProperties.onSwitchUser){
            buttonProperties.onSwitchUser();
          }
        }
      } else {
        // Long Press
        if (!buttonProperties.isInWhiteMode) {
          buttonProperties.lastColourIndex = buttonProperties.ColourIndex;
          setColour(0); // White
          buttonProperties.isInWhiteMode = true;
          if (buttonProperties.onStartRecording){
            buttonProperties.onStartRecording();
          }
        }
      }
      buttonProperties.buttonPressed = false; // Clear buttonPressed flag *after* processing
    } else {
      buttonProperties.buttonPressed = false; //Ensure button pressed is reset for debounced events.
    }
  }

  buttonProperties.lastButtonState = buttonProperties.buttonState; // Update last button state
}