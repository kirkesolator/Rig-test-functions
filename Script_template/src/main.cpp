#include <Arduino.h>
//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: VARIABLES :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..

// -----Pin parameters-----
int outPins[] = {};

// -----Timer parameters-----
unsigned long pTimer;
unsigned long tTimer;
bool doTime = false;

int state = 2; // Starting state
int nextstate;

//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: FUNCTIONS :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..
// f(state transition)-----
void transition(int n){
  state = -1;     // Reset state to default
  nextstate = n;  // Advance state index
}

// f(output pin toggle)-----
void output(int ppins[], int n, char direction){
  for(int i = 0; i < n; i++){
    digitalWrite(ppins[i], direction);
  }
}

// f(background timer)-----
void bgtimer(int tDur){
  doTime = true;
  pTimer = millis();
  tTimer = tDur;
}

// f(pinmodes)-----
void pinout(int outPins[], int nPins){
  for (int i = 0; i <= nPins; i++){
    pinMode(outPins[i],OUTPUT);
  }
}

//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: SETUP :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..
void setup() {
  // Start serial killing
  Serial.begin(9600);
  
  int nPins = sizeof(outPins)/sizeof(outPins[0]);

  // Set pin outputs
  pinout(outPins,nPins);

  // Wait with next stage until serial monitor is running
  while (! Serial);
}

//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: LOOP :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..
void loop() {

  //:::::: STATES ::::::
  switch(state){
    case 0:
      break;
    case 1:
      break;
    default:
      break;
  }
}