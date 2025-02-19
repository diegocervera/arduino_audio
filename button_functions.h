#ifndef BUTTON_FUNCTIONS_H
#define BUTTON_FUNCTIONS_H

struct ButtonProperties {
  const int buttonPin = 2;
  bool buttonState = HIGH;        // Current state of the button (starts HIGH because of pull-up resistor)
  int ColourIndex = 0;  // Tracks current Colour
  int lastColourIndex = 0; // Stores the original Colour before long press
  unsigned long lastButtonPressTime = 0;  // For debounce timing
  unsigned long buttonPressStartTime = 0; // Track when button press starts
  unsigned long debounceDelay = 50;  // Minimum time between button presses (50ms)
  unsigned long longPressDuration = 1000;  // Duration for long press detection (1 second)
  bool lastButtonState = HIGH; // Last button state
  bool buttonPressed = false;  // Flag for button press
  bool isInWhiteMode = false;  // Flag to track if LED is in white mode
};

void setColour(int index);
void buttonListener(ButtonProperties &buttonProperties);
#endif