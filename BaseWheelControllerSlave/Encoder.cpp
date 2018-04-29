#include <Arduino.h>
#include "Encoder.h"

Encoder::Encoder(int id, int pinPhaseA, int pinPhaseB) {
  this->id = id;
  this->pinPhaseA = pinPhaseA;
  this->pinPhaseB = pinPhaseB;
}

Encoder::~Encoder() {
}

void Encoder::setUp(void (*interupt)()) {
  pinMode(pinPhaseA, INPUT_PULLUP);
  pinMode(pinPhaseB, INPUT_PULLUP);

  int interruptId = digitalPinToInterrupt(pinPhaseA);
  if (interruptId == -1) {
    Serial.print("ERROR: pinPhaseA for encoder ID #");
    Serial.print(id);
    Serial.println(" is not an interrupt.");
  } else {
    attachInterrupt(interruptId, interupt, CHANGE);
  }
}

