#include <Wire.h>
#include "Motor.h"
#include "Interrupt.h"
#include "Encoder.h"

// front-left, front-right, etc -- from robot's perspective
// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorFL(1, 4, 5, 6);
Motor motorFR(2, 10, 11, 12);
Motor motorBL(3, 7, 8, 9);
Motor motorBR(4, 13, 22, 23);
Motor motorLA(5, 26, 25, 24);
Encoder encoderBR(6, 19, 27);
Encoder encoderFR(7, 18, 28);
Encoder encoderFL(8, 2, 29);
Encoder encoderBL(9, 3, 30);

unsigned long startTime = millis();
int mode = 0;

void setup() {
  Wire.begin(0x70);
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
  
  /*Motor motor = motorLA;
  
  if (mode == 0) {
    motor.step(100);
  } else if (mode == 1) {
    motor.step(0);
  } else if (mode == 2) {
    motor.step(-100);
  } else if (mode == 3) {
    motor.step(0);
  }
  
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  if (elapsedTime < 1000) {
    mode = 0;
  } else if (elapsedTime > 1000 && elapsedTime < 3000)  {
    mode = 1;
  } else if (elapsedTime > 3000 && elapsedTime < 4000) {
    mode = 2;
  } else if (elapsedTime > 4000 && elapsedTime < 5000) {
    mode = 3;
    startTime = millis();
  }
  
  Serial.println(interruptBL_pulses);*/
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

