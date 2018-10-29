#include <Arduino.h>
// ::::::: Pin definitions::::::

// LED pins on board that controls the 9px TV. Make sure it's correct!
const int pinsLED[9] = {1,2,3,4,5,6,7,8,9};

// Initiate counter
int i;

// Stimulus vectors
const int vecStim[8][9] = 
{
  {1,2,3,4,5,6,7,8,9},
  {1,4,7,2,5,8,3,6,9},
  {},
  {},
  {},
  {},
  {},
  {}
};

// ::::::: Setup :::::::
void setup() {
  // Open serial port
  Serial.begin(9600);
  
  // Get pin n
  int nPins = sizeof(pinsLED)/sizeof(pinsLED[0]);
  
  // Set pinmodes to output
  for (i = 0; i <= nPins; i++)
  {
    pinMode(pinsLED[i],OUTPUT);
  }
  // Wait with next stage until serial monitor is running
  while (! Serial);
}


// ::::::: Loop :::::::
void loop() {
  
}