#include <Arduino.h>
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

int incomingByte = 0;   // for incoming serial data

// -----Timer parameters-----
unsigned long pTimer;
unsigned long tTimer;
bool doTime = false;


//::::::::::::::::::FUNCTIONS::::::::::::::::::::::
// f(background timer)-----
void bgtimer(int tDur)
{
  doTime = true;
  pTimer = millis();
  tTimer = tDur;
}

// f(state transition)-----
void transition(int n)
{
  state = -1;     // Reset state to default
  nextstate = n;  // Advance state index
}

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
}

void loop() {
  pinMode(pinReward,OUTPUT);

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
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
    
    // Show key press (uncomment to see in serial monitor)
    // Serial.println(incomingByte, DEC);
    
    // Increase or decrease reward amount via "m" and "l" keys respectively
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

    // Increase or decrease inter-reward interval via "s" and "f" keys respectively
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

    // Manual reward delivery via "r" key
    if (incomingByte == 114){
      doRun = true;
      doBreak = true;
      state = 0;
      doTime = false;
      Serial.println("Reward delivered manually");
    }
  }
  
  if (doRun){
    // ..........Timer..........
      if (doTime){
        if (millis() - pTimer >= tTimer)
        {
          state = nextstate; // Trigger state transition via switch cases below
          doTime = false; // Reset timer
        }
      }
    switch (state){
      case 0: // Start reward
        bgtimer(tReward); // Start timer
        digitalWrite(pinReward,HIGH);
        transition(1); // Advance state index
       break;
      case 1: // Stop reward
        digitalWrite(pinReward,LOW);
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