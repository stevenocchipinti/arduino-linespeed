// Based on this example:
// http://www.arduino.cc/en/Tutorial/LiquidCrystal


// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd (8, 9, 4, 5, 6, 7);


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Hello");
  delay(1000);
}


void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Up:   964  Kbps");
  lcd.setCursor(0, 1);
  lcd.print("Down: 5035 Kbps");
  
  delay(10*1000);

  lcd.setCursor(0, 0);
  lcd.print("Refreshing...   ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  
  delay(1000);
}

