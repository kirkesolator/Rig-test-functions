#include <Arduino.h>
//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:PARAMETERS:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
// -----Important, make sure pin is correct!!!-----
const unsigned int pinReward = 11;

// -----Reward delivery times-----
unsigned int tIRI = 1000;
unsigned int tReward = 150;

// -----Initialise-----
bool doRun = false;
bool doBreak = false;

int state = 0;
int nextstate = 0;

int counter = 0;
int incomingByte = 0;

// -----Timer parameters-----
unsigned long pTimer;
unsigned long tTimer;
bool doTime = false;


//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:FUNCTIONS:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
// f(background timer)-----
void bgtimer(int tDur){
  doTime = true;
  pTimer = millis();
  tTimer = tDur;
}

// f(state transition)-----
void transition(int n){
  state = -1;     // Set initial state to default
  nextstate = n;  // Advance state index
}


//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:SETUP:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
void setup() {
  Serial.begin(9600);     // Open serial port, set data rate to 9600 bps
  pinMode(pinReward,OUTPUT); // Set pins to output
}

//:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:LOOP:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:-:
void loop() {
  // ........Serial communication..........
  if (Serial.available() > 0) {
    // FIX BUG TOOOR
    digitalWrite(pinReward,LOW); // @megha, does this do it?

    // Read the incoming byte
    incomingByte = Serial.read();

    // Start/stop reward delivery via SPACE BAR
    if (incomingByte == 32){
      doRun = !doRun;
      if (doRun){
        Serial.println("Reward delivery started");
        doRun = true;
      }
      else{
        Serial.println("Reward delivery stopped");
        doRun = false;
      }
    }
    
    // Show key press (uncomment to see in serial monitor):
    // Serial.println(incomingByte, DEC);
    
    // Increase or decrease reward amount via "m" (for MORE) and "l" (for LESS) keys respectively
    if (incomingByte == 109){
      tReward += 10;
      Serial.print("Reward delivery now: ");
      Serial.print(tReward);
      Serial.println("ms");
    }
    if (incomingByte == 108){
      tReward -= 10;
      Serial.print("Reward delivery now: ");
      Serial.print(tReward);
      Serial.println("ms");
    }
    // Increase or decrease inter-reward interval via "s" (for SLOWER) and "f" (for FASTER) keys respectively
    if (incomingByte == 102){
      tIRI -= 50;
      Serial.print("Inter-reward interval now: ");
      Serial.print(tIRI);
      Serial.println("ms");
    }
    if (incomingByte == 115){
      tIRI += 50;
      Serial.print("Inter-reward interval now: ");
      Serial.print(tIRI);
      Serial.println("ms");
    }
    // Manual reward delivery via "r" (for REWARD) key
    if (incomingByte == 114){
      doRun = true;
      doBreak = true;
      state = 0;
      doTime = false;
      Serial.println("Reward delivered manually");
    }
    // Reset reward counter via "c" (for crab juice) key
    if (incomingByte == 99){
      counter = 0;
      Serial.println("Reward counter reset to zero");
    }
  }
  
  // ........State progression........
  if (doRun){
    //..........Timer..........
      if (doTime){
        if (millis() - pTimer >= tTimer)
        {
          state = nextstate; // Trigger state transition via switch cases below
          doTime = false; // Switch off timer
        }
      }
    //..........Switch cases to control reward delivery stages..........
    switch (state){
      case 0: // Start reward
        bgtimer(tReward); // Start timer
        digitalWrite(pinReward,HIGH);
        transition(1); // Advance state index
       break;
      case 1: // Stop reward
        digitalWrite(pinReward,LOW);
        counter += 1;
        Serial.print("Rewards so far: ");
        Serial.println(counter);
        if (doBreak){
          doRun = false;
          doBreak = false;
          break;
        }
        transition(2); // Advance state index
      case 2: // Inter reward interval
        bgtimer(tIRI); // Start timer
        transition(0); // Advance state index
        break;
      default:
        break;
    }
  }
}