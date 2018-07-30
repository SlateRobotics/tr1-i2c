#include <Wire.h>
#include "Motor.h"
#include "ace128.h"

int jointState[] = {0,0};
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorHeadPan(14, 2, 3, 4);
Motor motorHeadTilt(15, 5, 6, 7);
Ace128 encoderHeadPan(22, 23, 24, 25, 26, 27, 28, 29, 30);
Ace128 encoderHeadTilt(32, 33, 34, 35, 36, 37, 38, 39, 40);

void setup() {
  Wire.begin(0x73);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorHeadPan.setUp();
  motorHeadTilt.setUp();
  encoderHeadPan.setUp();
  encoderHeadTilt.setUp();
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorHeadPan.executePreparedCommand();
  motorHeadTilt.executePreparedCommand();
  jointState[0] = encoderHeadPan.readPosition();
  jointState[1] = encoderHeadTilt.readPosition();
}

void requestEvent() {
  int motorId = Wire.read();
  
  int32_t state = 0;
  if (motorId == 14) {
    state = jointState[0];
  } else if (motorId == 15) {
    state = jointState[1];
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

    Serial.println(motorId);

    if (id == 14) {
      motorHeadPan.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 15) {
      motorHeadTilt.prepareCommand(motorStep, motorStepDuration);
    }
  }
}

