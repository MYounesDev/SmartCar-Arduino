#include <LiquidCrystal.h>
#include "others.h"

extern const uint8_t switchPin;
extern const uint8_t motorStartButton;
extern const uint8_t motorPin;
extern const uint8_t fuelPin;        // Potentiometer input pin
extern const uint8_t yellowPin;      // Yellow LED pin
extern const uint8_t bluePin;        // Blue LED pin
extern const uint8_t redPin;         // Red LED pin
extern const uint8_t LDRPin;         // LDR input pin
extern const uint8_t temperaturePin; // Potentiometer input pin
extern const uint8_t airconPin;      // Air conditioning pin
extern const uint8_t seatBeltButton; // Seat belt pin
extern const uint8_t buzzerPin;      // Buzzer pin

extern LiquidCrystal lcd;

extern bool permission;
extern bool doorClosed;
extern bool headlightsOn;
extern bool airconOn;
extern bool seatBeltOn;

bool checkdoor(bool firstTime = false);
bool checkSeatBelt();
bool checkLDRLevel(bool firstTime = false);
bool checkTemperature(bool firstTime = false);
void checkMotorButton();
int checkFuelLevel();

void checkMotorButton()
{
  bool buttonPressed = (get(motorStartButton) == LOW);
  if (buttonPressed && doorClosed && seatBeltOn && permission)
    on(motorPin);
  else
    off(motorPin); // Turn off motor if button is not pressed or conditions are not met
}

int checkFuelLevel()
{
  int fuelPercentage = analogRead(fuelPin);              // Read the fuel level from the potentiometer
  fuelPercentage = map(fuelPercentage, 0, 1023, 0, 100); // Map to percentage (0-100%)

  if (fuelPercentage < 10 && fuelPercentage >= 5)
  {
    on(yellowPin); // Turn on yellow LED if fuel is low
    lcd.clear();
    lcd.print("Yakit Seviyesi");
    lcd.setCursor(0, 1);
    lcd.print("Dusuk - %");
    lcd.print(fuelPercentage);
    delay(300);
    return 1;
  }
  else if (fuelPercentage < 5 && fuelPercentage > 0)
  {
    lcd.clear();
    lcd.print("Yakit Seviyesi");
    lcd.setCursor(0, 1);
    lcd.print("Cok Dusuk - %");
    lcd.print(fuelPercentage);
    for (int time = 0; time < 2; time++)
    {
      off(yellowPin); // Turn off yellow LED if fuel is low
      delay(49);
      on(yellowPin); // Turn on yellow LED if fuel is low
      delay(49);
    }
  }
  else if (fuelPercentage < 1)
  {

    permission = false; // Turn off yellow LED if fuel is empty

    off(yellowPin);
    off(bluePin);
    off(redPin);
    off(airconPin);
    noTone(buzzerPin);
    off(motorPin);
    pink(false);

    lcd.clear();
    lcd.print("Yakit Bitti");
    lcd.setCursor(0, 1);
    lcd.print("Motor Kapandi");
  }
  else if (fuelPercentage > 10)
  {
    if (!permission || get(yellowPin))
      lcd.clear();
    permission = true;
    off(yellowPin);
  }
}

bool checkdoor(bool firstTime = false)
{
  bool currentdoorState = (get(switchPin) == LOW);

  if (currentdoorState == doorClosed && !firstTime)
    return currentdoorState;

  if (currentdoorState)
  {
    // If door is on
    pink(false);
    lcd.clear();
    delay(40);
  }
  else
  {
    // If door is off, turn off output and show warning message
    off(motorPin);
    pink(true);
    lcd.clear();
    lcd.print("Uyari: Kapi Acik");
    lcd.setCursor(0, 1);
    lcd.print("Motor Calismaz");
  }

  return currentdoorState;
}

bool checkSeatBelt()
{

  bool currentSeatBeltState = (get(seatBeltButton) == LOW); // Adjusted for button logic
  if (currentSeatBeltState)
  {
    if (get(redPin))
      lcd.clear();
    off(redPin);
    noTone(buzzerPin);
  }
  else
  {
    if (!get(redPin))
    {
      lcd.clear();
      lcd.print("Emniyet Kemeri");
      lcd.setCursor(0, 1);
      lcd.print("Takili Degil!");
    }
    on(redPin);
    off(motorPin);
    tone(buzzerPin, 1000);
  }

  return !get(redPin);
}

bool checkLDRLevel(bool firstTime = false)
{
  {
    int ldrValue = analogRead(LDRPin);               // Read the LDR value
    bool currentHeadlightsState = (ldrValue <= 255); // Adjust threshold as needed

    if (currentHeadlightsState == headlightsOn && !firstTime)
      return currentHeadlightsState;

    if (currentHeadlightsState)
    {
      on(bluePin); // Turn on blue LED if headlights are on
      lcd.clear();
      lcd.print("Farlar Acik");
      lcd.setCursor(0, 1);
    }
    else
    {
      off(bluePin); // Turn off blue LED if headlights are off
      lcd.clear();
      if (!firstTime)
      {
        lcd.print("Farlar Kapandi");
        lcd.setCursor(0, 1);
        delay(1000);
        lcd.clear();
      }
    }

    return currentHeadlightsState;
  }
}

bool checkTemperature(bool firstTime = false)
{
  int temperatureValue = analogRead(temperaturePin); // Read the temperature value

  float temperatureC = (temperatureValue * (5.0 / 1023.0)) * 100.0; // Convert voltage to temperature in Celsius

  bool currentAirconState = (temperatureC > 25); // Adjust threshold as needed

  if (currentAirconState)
  {
    on(airconPin); // Turn on air conditioning if temperature is high
    lcd.clear();
    lcd.print("Sicaklik: ");
    lcd.print(temperatureC, 1); // Display temperature with 1 decimal place
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Klima Acik");
    delay(300);
  }
  else
  {
    off(airconPin); // Turn off air conditioning if temperature is low
    if (airconOn)
      lcd.clear();
  }

  return currentAirconState;
}
