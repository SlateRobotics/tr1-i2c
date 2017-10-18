#include <Wire.h>
#include "Motor.h"

// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorLeft(1, 2, 3, 4);
Motor motorRight(2, 5, 6, 7);

void setup() {
  Wire.begin(0x74);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorLeft.setUp();
  motorRight.setUp();
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorLeft.executePreparedCommand();
  motorRight.executePreparedCommand();
}

void receiveEvent(int howMany) {
  int mode = Wire.read();
  while (Wire.available()) {
    int value = Wire.read();
    int dir = Wire.read();
    int duration = Wire.read();
    duration = duration * 10;
    
    int motorStep = value;
    int motorStepDuration = duration;
    if (dir == 1) {
      motorStep = motorStep * -1;
    }

    motorLeft.prepareCommand(motorStep, motorStepDuration);
    motorRight.prepareCommand(motorStep, motorStepDuration);
  }
}

