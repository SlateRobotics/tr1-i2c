#include <Wire.h>
#include "Motor.h"

// front-left, front-right, etc -- from robot's perspective
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorShoulderPan(1, 2, 3, 4);
Motor motorShoulderTilt(2, 5, 6, 7);
Motor motorBicepRoll(3, 8, 9, 10);
Motor motorElbowTilt(4, 11, 12, 13);

void setup() {
  Wire.begin(0x72);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.print(0x72);
  Serial.println("Ready");

  motorShoulderPan.setUp();
  motorShoulderTilt.setUp();
  motorBicepRoll.setUp();
  motorElbowTilt.setUp();
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorShoulderPan.executePreparedCommand();
  motorShoulderTilt.executePreparedCommand();
  motorBicepRoll.executePreparedCommand();
  motorElbowTilt.executePreparedCommand();
}

void receiveEvent(int howMany) {
  int address = Wire.read();
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
      motorShoulderPan.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 2) {
      motorShoulderTilt.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 3) {
      motorBicepRoll.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 4) {
      motorElbowTilt.prepareCommand(motorStep, motorStepDuration);
    }
  }
}

