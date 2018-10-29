#include <Arduino.h>
// ::::::: PIN DEF :::::::::::::: PIN DEF :::::::::::::: PIN DEF :::::::::::::: PIN DEF :::::::::::::: PIN DEF :::::::

// -----LED pins on board that controls the 9px TV. Make sure it's correct!-----
const int pinsLED[9] = {11,12,13,14,15,16,17,18,19};

// -----Initiate counter-----
int i;
int j;
unsigned int counter = 0;
const int nStim = 9;

// -----ITI-----
const int tITI = 500;
const int blockSize = 8;

// -----stimulus step time-----
const int tS1 = 150;

// -----Stimulus vectors (modulus snake patterns)-----
const int vecStim[8][9] = 
{
  {1,2,3,4,5,6,7,8,9},
  {1,4,7,2,5,8,3,6,9},
  {3,2,1,6,5,4,9,8,7},
  {3,6,9,2,5,8,1,4,7},
  {9,8,7,6,5,4,3,2,1},
  {9,6,3,8,5,2,7,4,1},
  {7,4,1,8,5,2,9,6,3},
  {7,8,9,4,5,6,1,2,3}
};
int vecPins[8][9];

int S1pins[nStim];

// -----Timer parameters-----
unsigned long pTimer;
unsigned long tTimer;
bool doTime = false;

int state = 2; // Start with defining S1pins at beginning of block
int nextstate = 0;
int trials = 0;

//::::::: FUNCTIONS :::::::::::::: FUNCTIONS :::::::::::::: FUNCTIONS :::::::::::::: FUNCTIONS :::::::
// f(state transition)-----
void transition(int n)
{
  state = -1;     // Reset state to default
  nextstate = n;  // Advance state index
}

// f(output pin toggle)-----
void output(int ppins[], int n, char direction)
{
  for(i = 0; i < n; i++)
  {
    digitalWrite(ppins[i], direction);
  }
}

// f(background timer)-----
void bgtimer(int tDur)
{
  doTime = true;
  pTimer = millis();
  tTimer = tDur;
}

// ::::::: SETUP :::::::::::::: SETUP :::::::::::::: SETUP :::::::::::::: SETUP :::::::::::::: SETUP :::::::
void setup() {
  // Open serial port
  Serial.begin(9600);
  
  // Get pin n
  int nPins = sizeof(pinsLED)/sizeof(pinsLED[0]);
  
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 9; j++){
      vecPins[i][j] = pinsLED[vecStim[i][j]-1];
    }
  }
  // Set pinmodes to output
  for (i = 0; i <= nPins; i++)
  {
    pinMode(pinsLED[i],OUTPUT);
  }
  // Wait with next stage until serial monitor is running
  while (! Serial);
}


// ::::::: Loop :::::::::::::: Loop :::::::::::::: Loop :::::::::::::: Loop :::::::::::::: Loop :::::::
void loop() {
  // Run block
  // ..........Timer..........
  if (millis() - pTimer >= tTimer)
  {
    state = nextstate; // Trigger state transition via switch cases below
    doTime = false; // Reset timer
  }

  // ******************** State transitions ********************
  switch (state)
  {
    case 0: // ITI timer start
      bgtimer(tITI); // Start timer
      transition(1); // Advance state index
      Serial.print("0");
      break;
    case 1: // S1 stimulus ON
      if (counter == nStim)
      {
        counter = 0;
        transition(2); // Advance state index
        state = nextstate;
        output(S1pins,9,LOW);
        break;
      }
      bgtimer(tS1); // Start timer
      digitalWrite(S1pins[counter], HIGH); // activate(pins);
      transition(1); // Advance state index
      counter++;
      Serial.print("1");
      break;
    case 2: //New block, define S1pins
      for (i = 0; i < nStim; i++)
      {
        S1pins[i] = vecPins[trials][i];
      }
      transition(0); // Advance state index
      Serial.println("2");
      Serial.print(trials);
      Serial.print(":");
      trials++;
      if (trials == blockSize)
      {
        trials = 0;
      }
    default: // Default stay out of switches
      break;
  }
}
