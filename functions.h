#include <LiquidCrystal.h>

extern const uint8_t switchPin;
extern const uint8_t motorStartButton;
extern const uint8_t motorPin;
extern const uint8_t potPin;         // Potentiometer input pin
extern const uint8_t fuelPin;        // Potentiometer input pin
extern const uint8_t yellowPin;      // Yellow LED pin
extern const uint8_t bluePin;        // Blue LED pin
extern const uint8_t LDRPin;         // LDR input pin
extern const uint8_t temperaturePin; // Potentiometer input pin
extern const uint8_t airconPin;      // Air conditioning pin
extern const uint8_t seatBeltButton; // Seat belt pin

extern LiquidCrystal lcd;

extern const uint8_t RED_PIN;
extern const uint8_t GREEN_PIN;
extern const uint8_t BLUE_PIN;

extern unsigned long lastButtonPressTime;
extern bool permission;
extern bool doorClosed;
extern bool headlightsOn;
extern bool airconOn;
extern bool seatBeltOn;

void toggle(uint8_t pin);
void off(uint8_t pin);
void on(uint8_t pin);
 
bool checkdoor(bool firstTime = false);
bool checkSeatBelt(bool firstTime = false);
bool checkLDRLevel(bool firstTime = false);
bool checkTemperature(bool firstTime = false);
void checkMotorButton();

int checkFuelLevel();

void pink(bool on);

void WELCOME();
void typingEffect(String text, int col, int row, int speed);
void scrollMessage(String message, int speed, int row);

void toggle(uint8_t pin)
{
  digitalWrite(pin, !digitalRead(pin));
}

void off(uint8_t pin)
{
  digitalWrite(pin, LOW);
}

void on(uint8_t pin)
{
  if (!permission)
    return; // Check permission before turning on
  digitalWrite(pin, HIGH);
}

void checkMotorButton()
{
  bool buttonPressed = (digitalRead(motorStartButton) == LOW);
  if (buttonPressed && (millis() - lastButtonPressTime) > 200)
  {
    lastButtonPressTime = millis(); // Record the time of button press

    if (digitalRead(motorPin))
      off(motorPin);
    else if (doorClosed)
      on(motorPin);
  }
}

int checkFuelLevel()
{
  int fuelPercentage = analogRead(fuelPin);              // Read the fuel level from the potentiometer
  fuelPercentage = map(fuelPercentage, 0, 1023, 0, 100); // Map to percentage (0-100%)

  if (fuelPercentage < 10 && fuelPercentage >= 5)
  {
    digitalWrite(yellowPin, HIGH); // Turn on yellow LED if fuel is low
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
    for (int i = 0; i < 5; i++)
    {
      digitalWrite(yellowPin, LOW); // Turn off yellow LED if fuel is low
      delay(50);
      digitalWrite(yellowPin, HIGH); // Turn on yellow LED if fuel is low
      delay(50);
    }
  }
  else if (fuelPercentage < 1)
  {

    digitalWrite(yellowPin, LOW);
    permission = false; // Turn off yellow LED if fuel is empty
    // TO-DO: turn off all leds!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    off(motorPin);

    lcd.clear();
    lcd.print("Yakit Bitti");
    lcd.setCursor(0, 1);
    lcd.print("Motor Kapandi");
  }
  else if (fuelPercentage > 10)
  {
    permission = true;
  }
}

bool checkdoor(bool firstTime = false)
{
  bool currentdoorState = (digitalRead(switchPin) == LOW);

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
bool checkSeatBelt(bool firstTime = false)
{

  bool currentSeatBeltState = (digitalRead(seatBeltButton) == LOW); // Adjusted for button logic
  if (currentSeatBeltState && (millis() - lastButtonPressTime) > 200)
  {
    lastButtonPressTime = millis(); // Record the time of button press
    if (digitalRead(redPin))
    {
      off(redPin);
      off(buzzerPin);
    }
    else{
      on(redPin);
      on(buzzerPin);
    }
  }

  return !digitalRead(redPin);
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
        lcd.print("Farlar Kapali");
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

// RGB LED control function for pink color
void pink(bool on)
{
  if (on)
  {
    analogWrite(RED_PIN, 255);  // Red full
    analogWrite(GREEN_PIN, 0);  // Green off
    analogWrite(BLUE_PIN, 100); // Blue low → pink color
  }
  else
  {
    analogWrite(RED_PIN, 0);   // Red off
    analogWrite(GREEN_PIN, 0); // Green off
    analogWrite(BLUE_PIN, 0);  // Blue off
  }
}

// LCD EFFECTS
// Optional welcome screen
void WELCOME()
{
  scrollMessage("->  HOS GELDINIZ :)  <-", 20, 0);
  lcd.clear();
  for (int i = 0; i < 5; i++)
  {
    lcd.print(".");
    delay(50);
  }
  lcd.clear();
  for (int i = 0; i < 3; i++)
  {
    lcd.print(".....");
    delay(25);
    lcd.clear();
    delay(25);
  }
}

// Typing effect
void typingEffect(String text, int col, int row, int speed)
{
  lcd.setCursor(col, row);
  for (int i = 0; i < text.length(); i++)
  {
    lcd.print(text[i]);
    delay(speed);
  }
}

// Scroll message
void scrollMessage(String message, int speed, int row)
{
  int messageLength = message.length();
  int colStart = (row == 0) ? 16 : -messageLength;

  for (int i = 0; i < messageLength + 16; i++)
  {
    lcd.clear();
    lcd.setCursor(colStart - i, row);
    lcd.print(message);
    delay(speed);
  }
}
