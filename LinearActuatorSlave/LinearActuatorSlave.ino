#include <Wire.h>
#include "Interrupt.h"
#include "Encoder.h"

Encoder encoderLA(13, 2, 0);
uint16_t lapulses = 0;
int interruptLA_pulses_prev;

int laPinEnable = 3;
int laPinDrive1 = 4;
int laPinDrive2 = 5;

int pins[] = {32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47};

void setup() {
  Serial.begin(115200);
  Serial.println("Ready");
  
  pinMode(laPinEnable, INPUT);
  pinMode(laPinDrive1, INPUT);
  pinMode(laPinDrive2, INPUT);
  
  for (int i = 15; i >= 0; i--) {
    pinMode(pins[i], OUTPUT);
  }

  attachInterrupt(0, interruptLA_RISING, RISING);
}

int prevMovement = 0;
void loop() {
  int laEnable = digitalRead(laPinEnable);
  int laDrive1 = digitalRead(laPinDrive1);
  int laDrive2 = digitalRead(laPinDrive2);

  bool movingUp = (laDrive2 == 1);
  bool movingDown = (laDrive1 == 1);

  int diff = interruptLA_pulses - interruptLA_pulses_prev;
  if (movingUp == true) {
    prevMovement = 1;
  } else if (movingDown == true) {
    prevMovement = 2;
  }

  if (prevMovement == 1) {
    lapulses += diff;
  } else if (prevMovement == 2) {
    lapulses -= diff;
  }
  
  interruptLA_pulses_prev = interruptLA_pulses;

  Serial.println(interruptLA_pulses);

  uint16_t data = lapulses + 1000;
  for (int i = 15; i >= 0; i--) {
    int state = bitRead(data, i);
    if (state == 1) {
      digitalWrite(pins[i], HIGH);
    } else {
      digitalWrite(pins[i], LOW);
    }
  }
}
