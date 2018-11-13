#include <Arduino.h>

const int bRate = 9600; // Baud rate for serial com
const int pinLD = 13; // Input pin
bool LDstate = false; // Lick state set to 0 first (TODO: should it?)

void setup() {
  Serial.begin(bRate); // Open serial com
  pinMode(pinLD,INPUT); // Set pinmode to input
}

void loop() {
  if (digitalRead(pinLD) != LDstate){ // Compare detector state
    LDstate = !LDstate;
    if (LDstate){
      Serial.print("Lick ON"); // Report lick on
    }
    else{
      Serial.print("Lick OFF"); // Report lick off
    }
  }
}