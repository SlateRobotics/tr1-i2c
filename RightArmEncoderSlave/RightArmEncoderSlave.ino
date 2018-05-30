#include <Servo.h>
#include <Wire.h>
#include "Motor.h"
#include "ace128.h"
#include "ems22a.h"

// Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2);
Motor motorShoulderPan(1, 2, 3, 45);
Motor motorShoulderTilt(2, 5, 6, 7);
Motor motorBicepRoll(3, 8, 9, 10);
Motor motorElbowTilt(4, 11, 12, 13);
Motor motorForearmRoll(5, 22, 23, 24);
Motor motorWristTilt(6, 25, 26, 27);

Servo servoWrist;
Servo servoGripper;

int servoWristValue = 50;
int servoGripperValue = 40;

int jointState[] = {0,0,0,0,0,0};
const int jointEncoderCount = 6;
int jointEncoders[jointEncoderCount][10] = {
  // encoderType, reverseAngle, p1, p2, p3, p4 ...
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0}, // ShoulderPan
  { 0,  0,  0, 31,  0, 32,  0, 33,  0,  0}, // ShoulderTilt
  { 0,  0,  0, 34,  0, 35,  0, 36,  0,  0}, // UpperArmRoll
  { 0,  0,  0, 37,  0, 38,  0, 39,  0,  0}, // ElbowFlex
  { 0,  0,  0, 40,  0, 41,  0, 42,  0,  0}, // ForarmRoll
  { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0}  // WristFlex
};

void requestEvent() {
  int motorId = Wire.read();
  int encoderIndex = motorId - 1;
  uint32_t state = jointState[encoderIndex];

  //if (motorId == 4) Serial.println(state);
  
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

    if (id == 1) {
      motorShoulderPan.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 2) {
      motorShoulderTilt.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 3) {
      motorBicepRoll.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 4) {
      motorElbowTilt.prepareCommand(motorStep, motorStepDuration);
    } else if (id == 5) {
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

void setup() {
  Wire.begin(0x71);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");

  motorShoulderPan.setUp();
  motorShoulderTilt.setUp();
  motorBicepRoll.setUp();
  motorElbowTilt.setUp();
  motorForearmRoll.setUp();
  motorWristTilt.setUp();
  servoWrist.attach(43);
  servoGripper.attach(44);
  
  for (int i = 0; i < jointEncoderCount; i++) {
    int encoderType = jointEncoders[i][0];
    bool reversePosition = jointEncoders[1][1];
    int p1 = jointEncoders[i][2];
    int p2 = jointEncoders[i][3];
    int p3 = jointEncoders[i][4];
    int p4 = jointEncoders[i][5];
    int p5 = jointEncoders[i][6];
    int p6 = jointEncoders[i][7];
    int p7 = jointEncoders[i][8];
    int p8 = jointEncoders[i][9];
    
    if (encoderType == 0) {
      Ems22a(p1, p2, p3, p4, p5, p6).setUp();
    } else if (encoderType == 1) {
      Ace128(p1, p2, p3, p4, p5, p6, p7, p8).setUp();
    }
  }
}

void loop() {
  motorShoulderPan.executePreparedCommand();
  motorShoulderTilt.executePreparedCommand();
  motorBicepRoll.executePreparedCommand();
  motorElbowTilt.executePreparedCommand();
  motorForearmRoll.executePreparedCommand();
  motorWristTilt.executePreparedCommand();

  /*motorWristTilt.forward(100);
  delay(500);
  motorWristTilt.backward(100);
  delay(500);*/

  servoWrist.write(servoWristValue);
  servoGripper.write(servoGripperValue);
  
  for (int i = 0; i < jointEncoderCount; i++) {
    int encoderType = jointEncoders[i][0];
    bool reversePosition = jointEncoders[i][1];
    int p1 = jointEncoders[i][2];
    int p2 = jointEncoders[i][3];
    int p3 = jointEncoders[i][4];
    int p4 = jointEncoders[i][5];
    int p5 = jointEncoders[i][6];
    int p6 = jointEncoders[i][7];
    int p7 = jointEncoders[i][8];
    int p8 = jointEncoders[i][9];
    
    if (encoderType == 0) {
      jointState[i] = Ems22a(p1, p2, p3, p4, p5, p6).readPosition(reversePosition);
    } else if (encoderType == 1) {
      jointState[i] = Ace128(p1, p2, p3, p4, p5, p6, p7, p8).readPosition(reversePosition);
    }
    
    if (i == 4) Serial.println(Ems22a(p1, p2, p3, p4, p5, p6).readDegree(reversePosition));
    
    //Serial.print(jointState[i]);
    //Serial.print(", ");
  }
  
  //Serial.println("");
  delay(50);
}

