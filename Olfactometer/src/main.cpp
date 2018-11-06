#include <Arduino.h>
//..
//..
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-: VARIABLES :-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
//..
//..

// -----Pin parameters-----
int outPins[] = {11,12,13,14,15,16,17,18,19,20,21,22,23};

// -----Valve configurations for 12 odor channels and 1 clean channel (first is clean)-----
const int nStim = 12;
int stimPins[4][nStim]; // Empty array to map pins to laters
int odorPins[4][nStim] = {
  {0,0,0,3,3,3,6,6,6, 9, 9, 9},    // Valve 1
  {1,2,2,4,5,5,7,8,8,10,11,11}, // Valve 2
  {0,1,1,0,1,1,0,1,1, 0, 1, 1}, // Valve 1 value
  {1,0,1,1,0,1,1,0,1, 1, 0, 1}  // Valve 2 value
};
int stimVal[2] = {LOW,HIGH};

int nPins = sizeof(outPins)/sizeof(outPins[0]);
const int defaultStim = 1;

// -----Timer parameters-----
unsigned long pTimer;
unsigned long tTimer;
bool doTime = true;
 
// -----State parameters-----
int state = 0; // Starting state
int nextstate;
int counter = 1;
int countmax = 0; // Change if you want another cycle than complete run through

// -----Stimulus presentation time (ms)-----
int unsigned tStim = 200;
int unsigned tISI = 800;

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

// f(map stimulus vector to output pins)
void mapoutpins(int outPins[],int stimPins[4][nStim],int odorPins[4][nStim],int nStim){
   for(int j = 0; j < 2; j++){
    for (int i = 0; i <= nStim; i++){
      stimPins[j][i] = outPins[odorPins[j][i]];
    }
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

  // Map output pins onto simulus matrix
  mapoutpins(outPins,stimPins,odorPins,nStim);

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
  // ..........Timer..........
      if(doTime){
        if (millis() - pTimer >= tTimer){
          state = nextstate; // Trigger state transition via switch cases below
          doTime = false; // Reset timer
        }
      }

  //:::::: STATES ::::::
  switch(state){
    case 0: // Run inter-stimulus-interval
      bgtimer(tISI);
      transition(1);
      break;
    case 1: // Initiate odor delivery
      counter = counter + 1; // Advance counter
      if (counter == countmax){
        counter = 1;
        break;
      }

      // Set the next stimulus ID and pin vector
      for (int i = 0; i < 2; i++){
        digitalWrite(stimPins[i][counter],stimVal[stimPins[i+2][counter]]);
        digitalWrite(stimPins[i][defaultStim],LOW);
      }
      bgtimer(tStim);
      transition(2);
      break;
    case 2: // Initiate odor delivery
      // Reset the stimulus ID and pin vector
      for (int i = 0; i < 2; i++){
        digitalWrite(stimPins[i][counter],LOW);
        digitalWrite(stimPins[i][defaultStim],stimVal[stimPins[i+2][defaultStim]]);
      }
      transition(0);
      break;
    default:
      break;
  }
}