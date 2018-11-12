#include <Arduino.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int sizeStim = 6;
byte full[8] = {
  B11111,B11111,
  B11111,B11111,
  B11111,B11111,
  B11111,B11111,
};
byte dl[8] = {
  B11111,B11111,
  B01111,B01111,
  B00111,B00111,
  B00011,B00001,
};
byte dr[8] = {
  B11111,B11111,
  B11110,B11110,
  B11100,B11100,
  B11000,B10000,
};
byte ul[8] = {
  B00001,B00011,
  B00111,B00111,
  B01111,B01111,
  B11111,B11111,
};
byte ur[8] = {
  B10000,B11000,
  B11100,B11100,
  B11110,B11110,
  B11111,B11111,
};

int charVec[2][6] = {
  {1,0,0,0,0,2},
  {3,0,0,0,0,4}
};

int counter = 0;
void setup() {
  lcd.createChar(0, full);
  lcd.createChar(1, dl);
  lcd.createChar(2, dr);
  lcd.createChar(3, ul);
  lcd.createChar(4, ur);
  lcd.begin(16, 2);
}

void loop() {
  // Clear LCD
  lcd.clear();
  
  // Delay (UGH)
  delay(1000);
  
  // Update counter
  counter += 1;
  lcd.setCursor((counter%3)*5,(counter%2-1)); 
  for(int i = 0; i < sizeStim; i++){
    lcd.write(byte(charVec[(counter%2)][i]));
  }
  delay(500);
}