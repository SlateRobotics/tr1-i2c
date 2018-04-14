#include <Arduino.h>
#include "Ems22a.h"

Ems22a::Ems22a() {
}

Ems22a::Ems22a(int p1, int p2, int p3, int p4, int p5, int p6) {
  this->PIN_INPUT = p1;
  this->PIN_CLOCK = p2;
  this->PIN_GND = p3;
  this->PIN_DATA = p4;
  this->PIN_VCC = p5;
  this->PIN_CS = p6;
}

Ems22a::~Ems22a() {
}


void Ems22a::setUp() {
  pinMode(PIN_INPUT, INPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_DATA, INPUT_PULLUP);
  pinMode(PIN_VCC, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  
  digitalWrite(PIN_GND, LOW);
  digitalWrite(PIN_VCC, HIGH);

  digitalWrite(PIN_CLOCK, HIGH);
  digitalWrite(PIN_CS, LOW);
}

float Ems22a::readDegree(bool reverseAngle = false) {
  int pos = readPosition(reverseAngle);
  return ((pos + 1.0) / 1024.0) * 360.0;
}

int Ems22a::readPosition(bool reverseAngle = false) {
  digitalWrite(PIN_GND, LOW);
  digitalWrite(PIN_VCC, HIGH);

  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CS, LOW);
  int pos = 0;
  for (int i=0; i<10; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
   
    byte b = digitalRead(PIN_DATA) == HIGH ? 1 : 0;
    pos += b * pow(2, 10-(i+1));
  }
  for (int i=0; i<6; i++) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_CLOCK, HIGH);
  }
  digitalWrite(PIN_CLOCK, LOW);
  digitalWrite(PIN_CLOCK, HIGH);

  if (reverseAngle == false) {
    pos = abs(1024 - pos);
  }

  return pos;
}

