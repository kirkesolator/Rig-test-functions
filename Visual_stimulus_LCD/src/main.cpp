//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: LIBRARIES :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..
#include <Arduino.h>
#include <LiquidCrystal.h>

// Initialize LCD object
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: VARIABLES :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..

int sizeStim = 6; 
// Define custom character shapes
byte full[8] = { // Full
  B11111,B11111,
  B11111,B11111,
  B11111,B11111,
  B11111,B11111,
};
byte dl[8] = { // Down to left
  B11111,B11111,
  B01111,B01111,
  B00111,B00111,
  B00011,B00001,
};
byte dr[8] = { // Down to right
  B11111,B11111,
  B11110,B11110,
  B11100,B11100,
  B11000,B10000,
};
byte ul[8] = { // Up to left
  B00001,B00011,
  B00111,B00111,
  B01111,B01111,
  B11111,B11111,
};
byte ur[8] = { // Down to right
  B10000,B11000,
  B11100,B11100,
  B11110,B11110,
  B11111,B11111,
};

// Stimulus-character matrix 
int charVec[2][6] = {
  {1,0,0,0,0,2},
  {3,0,0,0,0,4}
};

// Stimulus-cursor matrix 
int cursorVec[2][6] = {
  {0,1, 0,1,0, 1},
  {0,5,10,0,5,10}
};

// Initilalize counter
int counter = 0;

//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: SETUP :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..
void setup() {
  // Create custom characters in the custom character slots 0-4
  lcd.createChar(0, full);
  lcd.createChar(1, dl);
  lcd.createChar(2, dr);
  lcd.createChar(3, ul);
  lcd.createChar(4, ur);
  
  // Start LCD
  lcd.begin(16, 2);
  
  // Initialize Serial
  Serial.begin(9600);
}

//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: LOOP :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..
void loop() {
  // Clear LCD
  lcd.clear();
  
  // Delay (UGH)
  delay(1000);
  
  counter += 1; // Update counter
  counter = counter % 6; // Keep counter within [0 5]

  lcd.setCursor(cursorVec[1][counter],cursorVec[0][counter]); 
  // Write the custom characters to LCD
  for(int i = 0; i < sizeStim; i++){
    lcd.write(byte(charVec[counter % 2][i]));
  }
  lcd.setCursor(cursorVec[1][counter]+2,!cursorVec[0][counter]);
  lcd.write(byte(charVec[counter % 2][0]));
  lcd.write(byte(charVec[counter % 2][5]));
  Serial.print("Counter: ");
  Serial.println(counter);
  delay(500);
}