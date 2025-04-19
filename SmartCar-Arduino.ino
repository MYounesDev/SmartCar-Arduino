#include <LiquidCrystal.h>
#include "functions.h"

const uint8_t doorPin = 15;
const uint8_t buttonPin = 16;
const uint8_t motorPin = 14;

// Initialize LCD
LiquidCrystal lcd(12, 7, 5, 4, 3, 2);

// RGB LED Pins
const uint8_t RED_PIN = 10;
const uint8_t GREEN_PIN = 9;
const uint8_t BLUE_PIN = 8;

unsigned long lastButtonPressTime = 0;

bool switchClosed;

void setup() {
  pinMode(doorPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);

  lcd.begin(16, 2);

  // RGB LED Pins setup
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  delay(100);
  // Uncomment to show welcome message
  //WELCOME();
}


void loop() {

  switchClosed = checkSwitch();

  // Button debounce logic
  checkButtonPressed();
}






