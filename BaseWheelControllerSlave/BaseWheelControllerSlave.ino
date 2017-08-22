#include <Wire.h>
#include "Motor.h"

// front-left, front-right, etc -- from robot's perspective
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorFL(1, 2, 3, 4);
Motor motorFR(2, 5, 6, 7);
Motor motorBL(3, 8, 9, 10);
Motor motorBR(4, 11, 12, 13);

void setup() {
  Wire.begin(0x70);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorFL.setUp();
  motorFR.setUp();
  motorBL.setUp();
  motorBR.setUp();
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorFL.executePreparedCommand();
  motorFR.executePreparedCommand();
  motorBL.executePreparedCommand();
  motorBR.executePreparedCommand();
}

void receiveEvent(int howMany) {
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

    if (id == 1) {
      motorFL.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 2) {
      motorFR.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 3) {
      motorBL.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 4) {
      motorBR.prepareCommand(motorStep, motorStepDuration);
    }
  }
}

