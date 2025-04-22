#include <LiquidCrystal.h>

extern LiquidCrystal lcd;

extern const uint8_t RED_PIN;
extern const uint8_t GREEN_PIN;
extern const uint8_t BLUE_PIN;

extern bool permission;


void toggle(uint8_t pin);
void off(uint8_t pin);
void on(uint8_t pin);


void pink(bool on);

void WELCOME();
void typingEffect(String text, int col, int row, int speed);
void scrollMessage(String message, int speed, int row);



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
