#include <Wire.h>
#include "ace128.h"
#include "ems22a.h"

int jointState[] = {0,0,0,0,0,0};
const int jointEncoderCount = 6;
int jointEncoders[jointEncoderCount][10] = {
  // encoderType, reverseAngle, p1, p2, p3, p4 ...
  { 0,  0,  0, A5, A4, A3, A2, A1,  0,  0}, // ShoulderPan
  { 0,  1,  0, 33, 34, 35, 36, 37,  0,  0}, // ShoulderTilt
  { 0,  0,  0, 42, 44, 46, 41, 43,  0,  0}, // UpperArmRoll
  { 0,  1,  0, 11, 12, 13, 14, 15,  0,  0}, // ElbowFlex
  { 0,  0,  0,  3,  4,  5,  6,  7,  0,  0}, // ForarmRoll
  { 1,  0, 22, 23, 24, 25, 26, 27, 28, 29}  // WristFlex
};

void requestEvent() {
  int motorId = Wire.read();
  int encoderIndex = motorId - 1;
  uint32_t state = jointState[encoderIndex];

  if (motorId == 4) Serial.println(state);
  
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
  //Serial.print("receiveEvent ");
  //Serial.println(howMany);
}

void setup() {
  Wire.begin(0x74);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Serial.begin(115200);
  Serial.println("Ready");
  
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
    
    //if (i == 4) Serial.println(Ems22a(p1, p2, p3, p4, p5, p6).readDegree(reversePosition));
    
    //Serial.print(jointState[i]);
    //Serial.print(", ");
  }
  
  //Serial.println("");
  delay(50);
}

