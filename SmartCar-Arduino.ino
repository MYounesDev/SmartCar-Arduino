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

void setup() {
  pinMode(doorPin, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);

  lcd.begin(16, 2);
  
  // RGB LED Pins setup
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Uncomment to show welcome message
  //WELCOME();
}

void loop() {
  bool buttonPressed = (digitalRead(buttonPin) == LOW);

  // Button debounce logic
  if (buttonPressed && (millis() - lastButtonPressTime) > 200) {
    lastButtonPressTime = millis(); // Record the time of button press

    bool switchState = (digitalRead(doorPin) == LOW);
    if (digitalRead(motorPin))
      off(motorPin);
    else if (switchState) {
      // If switch is on, turn on output and set LED to pink
      pink(false);
      on(motorPin);
      lcd.clear();
      delay(40);
    } else {
      // If switch is off, turn off output and show warning message
      off(motorPin);
      pink(true);
      lcd.clear();
      lcd.print("Uyari: Kapi Acik");
      lcd.setCursor(0, 1);
      lcd.print("Motor Calismaz");
      
    }
  }
}

// RGB LED control function for pink color
void pink(bool on){
  if (on){
    analogWrite(RED_PIN, 255);  // Red full
    analogWrite(GREEN_PIN, 0);  // Green off
    analogWrite(BLUE_PIN, 100); // Blue low → pink color
  }
  else{
    analogWrite(RED_PIN, 0);    // Red off
    analogWrite(GREEN_PIN, 0);  // Green off
    analogWrite(BLUE_PIN, 0);   // Blue off
  }
}
