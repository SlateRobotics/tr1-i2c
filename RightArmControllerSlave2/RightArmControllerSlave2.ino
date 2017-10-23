#include <Servo.h>
#include <Wire.h>
#include "Motor.h"

// front-left, front-right, etc -- from robot's perspective
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorForearmRoll(1, 2, 3, 4);
Motor motorWristTilt(2, 5, 6, 7);
Servo servoGripper;

void setup() {
  Wire.begin(0x73);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorForearmRoll.setUp();
  motorWristTilt.setUp();
  servoGripper.attach(8);
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorForearmRoll.executePreparedCommand();
  motorWristTilt.executePreparedCommand();
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
      motorForearmRoll.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 2) {
      motorWristTilt.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 3) {
      servoGripper.write(motorStep);
    }
  }
}

