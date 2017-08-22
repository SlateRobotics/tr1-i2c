#include <Wire.h>
#include "Motor.h"

// Motor::Motor(int pinEnable, int pinDrive1, int pinDrive2);
Motor motor(2, 3, 4);

void setup() {
  Wire.begin(0x71);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motor.setUp();
}

void loop() {
  // this executes if a command has been flagged/prepared
  motor.executePreparedCommand();
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

    motor.prepareCommand(motorStep, motorStepDuration);
  }
}

