#include <Servo.h>
#include <Wire.h>
#include "Motor.h"

// front-left, front-right, etc -- from robot's perspective
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorForearmRoll(5, 2, 3, 4);
Motor motorWristTilt(6, 5, 6, 7);
Servo servoWrist;
Servo servoGripper;

int servoWristValue = 50;
int servoGripperValue = 40;

void setup() {
  Wire.begin(0x73);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorForearmRoll.setUp();
  motorWristTilt.setUp();
  servoWrist.attach(8);
  servoGripper.attach(9);
}

void loop() {
  // these execute if a command has been flagged/prepared
  motorForearmRoll.executePreparedCommand();
  motorWristTilt.executePreparedCommand();

  servoWrist.write(servoWristValue);
  servoGripper.write(servoGripperValue);
  
  //servoWristValue++;
  //servoGripperValue++;
  //if (servoWristValue > 100) servoWristValue = 50;
  //if (servoGripperValue > 65) servoGripperValue = 0;

  delay(50);
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

    if (id == 5) {
      motorForearmRoll.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 6) {
      motorWristTilt.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 7) {
      servoWristValue = motorStep;
    } else if (id == 8) {
      servoGripperValue = motorStep;
    }
  }
}

