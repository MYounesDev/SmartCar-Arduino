#include <LiquidCrystal.h>


extern const uint8_t doorPin;
extern const uint8_t buttonPin;
extern const uint8_t motorPin;

extern LiquidCrystal lcd;

extern const uint8_t RED_PIN;
extern const uint8_t GREEN_PIN;
extern const uint8_t BLUE_PIN;

extern unsigned long lastButtonPressTime;
extern bool switchClosed;


void toggle(uint8_t pin);
void off(uint8_t pin);
void on(uint8_t pin);
void WELCOME();
void typingEffect(String text, int col, int row, int speed);
void scrollMessage(String message, int speed, int row);
bool checkSwitch();
void checkButtonPressed();
void pink(bool on);

void toggle(uint8_t pin) {
    digitalWrite(pin, !digitalRead(pin));
  }
  
void off(uint8_t pin) {
    digitalWrite(pin, LOW);
}
  
void on(uint8_t pin) {
    digitalWrite(pin, HIGH);
}

  

void checkButtonPressed(){
    bool buttonPressed = (digitalRead(buttonPin) == LOW);
    if (buttonPressed && (millis() - lastButtonPressTime) > 200) {
    lastButtonPressTime = millis();  // Record the time of button press


    if (digitalRead(motorPin))
      off(motorPin);
    else if(switchClosed)
      on(motorPin);
    
  }
}



bool checkSwitch(){
  bool switchState = (digitalRead(doorPin) == LOW);


  if (switchState == switchClosed)
    return switchState;

  if (switchState) {
    // If switch is on
    pink(false);
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


  return switchState;
}

// RGB LED control function for pink color
void pink(bool on) {
  if (on) {
    analogWrite(RED_PIN, 255);   // Red full
    analogWrite(GREEN_PIN, 0);   // Green off
    analogWrite(BLUE_PIN, 100);  // Blue low → pink color
  } else {
    analogWrite(RED_PIN, 0);    // Red off
    analogWrite(GREEN_PIN, 0);  // Green off
    analogWrite(BLUE_PIN, 0);   // Blue off
  }
}















// LCD EFFECTS
// Optional welcome screen
void WELCOME() {
    scrollMessage("->  HOS GELDINIZ :)  <-", 20, 0);
    lcd.clear();
    for (int i = 0; i < 5; i++) {
      lcd.print(".");
      delay(50);
    }
    lcd.clear();
    for (int i = 0; i < 3; i++) {
      lcd.print(".....");
      delay(25);
      lcd.clear();
      delay(25);
    }
}
  
  // Typing effect
void typingEffect(String text, int col, int row, int speed) {
    lcd.setCursor(col, row);
    for (int i = 0; i < text.length(); i++) {
      lcd.print(text[i]);
      delay(speed);
    }
}
  
  // Scroll message
void scrollMessage(String message, int speed, int row) {
    int messageLength = message.length();
    int colStart = (row == 0) ? 16 : -messageLength;
  
    for (int i = 0; i < messageLength + 16; i++) {
      lcd.clear();
      lcd.setCursor(colStart - i, row);
      lcd.print(message);
      delay(speed);
    }
}