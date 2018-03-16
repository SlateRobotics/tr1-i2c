#include <Wire.h>
#include "Ace128.h"

float ShoulderPan;
float SholderTilt;
float UpperArmRoll;
float ElbowFlex;
float ForarmRoll;
float WristFlex;

const int RightArmAce128Count = 6;

const int ShoulderPanIndex = 5;
const int SholderTiltIndex = 3;
const int UpperArmRollIndex = 4;
const int ElbowFlexIndex = 1;
const int ForarmRollIndex = 0;
const int WristFlexIndex = 2;

int jointState[] = {0,0,0,0,0,0};

Ace128 RightArmAce128[RightArmAce128Count] = {
  Ace128( 2,  3,  4,  5,  6,  7,  8,  9),
  Ace128(10, 11, 12, 13, 14, 15, 16, 17),
  Ace128(22, 23, 24, 25, 26, 27, 28, 29),
  Ace128(32, 33, 34, 35, 36, 37, 38, 39),
  Ace128(40, 42, 44, 46, 41, 43, 45, 47),
  Ace128(A0, A1, A2, A3, A4, A5, A6, A7)
};

void requestEvent() {
  int motorId = Wire.read();
  int encoderIndex = motorId - 1;

  uint8_t buf[1];
  buf[0] = jointState[encoderIndex];

  Serial.print("motor: ");
  Serial.print(motorId);
  Serial.print("; pos: ");
  Serial.println(buf[encoderIndex]);

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
  
  for (int i = 0; i < RightArmAce128Count; i++) {
    RightArmAce128[i].setUp();
  }
}

void loop() {
  jointState[0] = RightArmAce128[ShoulderPanIndex].readPosition(true);
  jointState[1] = RightArmAce128[SholderTiltIndex].readPosition(false);
  jointState[2] = RightArmAce128[UpperArmRollIndex].readPosition(true);
  jointState[3] = RightArmAce128[ElbowFlexIndex].readPosition(false);
  jointState[4] = RightArmAce128[ForarmRollIndex].readPosition(true);
  jointState[5] = RightArmAce128[WristFlexIndex].readPosition(true);

//  for (int i = 0; i < 6; i++) {
//    Serial.print(jointState[i]);
//    Serial.print(", ");
//  }
//  Serial.println("");
  
  delay(50);
}

