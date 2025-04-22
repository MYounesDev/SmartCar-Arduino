#include <LiquidCrystal.h>
#include "functions.h"

const uint8_t bluePin = 6;      // Blue LED pin
const uint8_t yellowPin = 11;       // Yellow LED pin
const uint8_t redPin = 13;        // Red LED pin
const uint8_t motorStartButton = 14;
const uint8_t seatBeltButton = 17; // Seat belt pin
const uint8_t motorPin = 18;
const uint8_t airconPin = 19; // Air conditioning pin.
const uint8_t buzzerPin = 20; // Buzzer pin
const uint8_t switchPin = 21;
const uint8_t fuelPin = A0;          // Potentiometer input pin
const uint8_t LDRPin = A1;          // LDR input pin
const uint8_t temperaturePin = A2;      // Potentiometer input pin


// Initialize LCD
LiquidCrystal lcd(12, 7, 5, 4, 3, 2);

// RGB LED Pins
const uint8_t RED_PIN = 10;
const uint8_t GREEN_PIN = 9;
const uint8_t BLUE_PIN = 8;

unsigned long lastButtonPressTime = 0;

bool permission = true;
bool doorClosed;
bool headlightsOn;
bool airconOn;
bool seatBeltOn;



void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(motorStartButton, INPUT_PULLUP);
  pinMode(seatBeltButton, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  pinMode(airconPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  

  lcd.begin(16, 2);

  // RGB LED Pins setup
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  checkdoor(true);
  checkLDRLevel(true);
  checkTemperature(true);

  delay(100);
  // Uncomment to show welcome message
  //WELCOME();
}



void loop() {


  doorClosed = checkdoor();
  headlightsOn = checkLDRLevel();
  
  
  airconOn = checkTemperature();
  seatBeltOn = checkSeatBelt();

  
  checkFuelLevel(); 
  
  checkMotorButton();

}






