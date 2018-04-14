#include <Wire.h>
#include "ace128.h"
#include "ems22a.h"

float jointState[] = {0,0,0,0,0,0};
const int jointEncoderCount = 6;
int jointEncoders[jointEncoderCount][10] = {
  // encoderType, reverseAngle, p1, p2, p3, p4 ...
  { 0,  1,  0, A5, A4, A3, A2, A1,  0,  0}, // ShoulderPanIndex
  { 0,  0,  0, 33, 34, 35, 36, 37,  0,  0}, // SholderTiltIndex
  { 0,  1,  0, 42, 44, 46, 41, 43,  0,  0}, // UpperArmRollIndex
  { 1,  0, 10, 11, 12, 13, 14, 15, 16, 17}, // ElbowFlexIndex
  { 1,  1,  2,  3,  4,  5,  6,  7,  8,  9}, // ForarmRollIndex
  { 1,  1, 22, 23, 24, 25, 26, 27, 28, 29}  // WristFlexIndex
};

void requestEvent() {
  int motorId = Wire.read();
  int encoderIndex = motorId - 1;

  uint8_t buf[1];
  buf[0] = jointState[encoderIndex];

//  Serial.print("motor: ");
//  Serial.print(motorId);
//  Serial.print("; pos: ");
//  Serial.println(buf[encoderIndex]);

  Wire.write(buf, 1);
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
      jointState[i] = Ems22a(p1, p2, p3, p4, p5, p6).readDegree(reversePosition);
    } else if (encoderType == 1) {
      jointState[i] = Ace128(p1, p2, p3, p4, p5, p6, p7, p8).readDegree(reversePosition);
    }
    
    Serial.print(jointState[i]);
    Serial.print(", ");
  }
  
  Serial.println("");
  delay(50);
}

