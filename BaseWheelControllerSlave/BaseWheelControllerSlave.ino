#include <Wire.h>
#include "Motor.h"
#include "Interrupt.h"
#include "Encoder.h"

// front-left, front-right, etc -- from robot's perspective
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorFL(9, 4, 5, 6);
Motor motorFR(10, 10, 11, 12);
Motor motorBL(11, 7, 8, 9);
Motor motorBR(12, 13, 22, 23);
Motor motorLA(13, 26, 25, 24);
Encoder encoderFL(9, 2, 29);
Encoder encoderFR(10, 18, 28);
Encoder encoderBL(11, 3, 30);
Encoder encoderBR(12, 19, 27);

int flpulses = 0;
int frpulses = 0;
int blpulses = 0;
int brpulses = 0;

void requestEvent() {
  int motorId = Wire.read();
  
  int32_t state = 0;
  if (motorId == 9) {
    state = flpulses;
  } else if (motorId == 10) {
    state = frpulses;
  } else if (motorId == 11) {
    state = blpulses;
  } else if (motorId == 12) {
    state = brpulses;
  }

  // handle signed ints via offset binary method
  if (state > 2147483647) state = 2147483647;
  if (state < -2147483648) state = -2147483648;
  int bufferSize = 4;
  int32_t excessK = pow(256, bufferSize)/2;
  uint32_t data = state + excessK;
  
  byte dataBuffer[4] = {0, 0, 0, 0};
  dataBuffer[0] = data >> 24;
  dataBuffer[1] = data >> 16;
  dataBuffer[2] = data >> 8;
  dataBuffer[3] = data;

  Wire.write(dataBuffer, 4);
}

void receiveEvent(int howMany) {
  if (howMany == 1) return; // it's a request event
  int mode = Wire.read();
  while (Wire.available()) {
    int id = Wire.read();
    int value = Wire.read();
    int dir = Wire.read();
    int duration = Wire.read();
    duration = duration * 10;

    int motorId = id;
    int motorStep = value;
    int motorStepDuration = duration;
    if (dir == 1) {
      motorStep = motorStep * -1;
    }

    if (id == 9) {
      motorFL.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 10) {
      motorFR.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 11) {
      motorBL.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 12) {
      motorBR.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 13) {
      motorLA.prepareCommand(motorStep, motorStepDuration);
    }
  }
}

void setup() {
  Wire.begin(0x70);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorFL.setUp();
  motorFR.setUp();
  motorBL.setUp();
  motorBR.setUp();
  motorLA.setUp();
  encoderBR.setUp(interruptBR);
  encoderFR.setUp(interruptFR);
  encoderBL.setUp(interruptBL);
  encoderFL.setUp(interruptFL);
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorFL.executePreparedCommand();
  motorFR.executePreparedCommand();
  motorBL.executePreparedCommand();
  motorBR.executePreparedCommand();
  motorLA.executePreparedCommand();

  flpulses = interruptFL_pulses;
  frpulses = interruptFR_pulses;
  blpulses = interruptBL_pulses;
  brpulses = interruptBR_pulses;

  //Serial.println(flpulses);
}
