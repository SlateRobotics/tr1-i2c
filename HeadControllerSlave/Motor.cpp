#include <Arduino.h>
#include "Motor.h"

Motor::Motor(int id, int pinEnable, int pinDrive1, int pinDrive2) {
  // motorSpeed set to a percentage of max voltage to pins
  this->motorSpeed = 0;
  this->encoderTickCount = 0;
  this->id = id;
  this->pinEnable = pinEnable;
  this->pinDrive1 = pinDrive1;
  this->pinDrive2 = pinDrive2;
}

Motor::~Motor() {
  this->stop();
}

void Motor::setPinSpeed() {
  int pinStrength = int(255.0 * (motorSpeed / 100.0));
  analogWrite(pinEnable, pinStrength);
}

void Motor::setUp() {
  pinMode(pinEnable, OUTPUT);
  pinMode(pinDrive1, OUTPUT);
  pinMode(pinDrive2, OUTPUT);

  setPinSpeed();
}

void Motor::forward(int speed = 100) {
  // given speed should be a percentage of total speed so that we can tell it to start slowly
  this->motorSpeed = speed;
  if (speed < 20) {
    analogWrite(pinEnable, LOW);
    digitalWrite(pinDrive1, LOW);
    digitalWrite(pinDrive2, LOW);
  } else {
    setPinSpeed();
    digitalWrite(pinDrive1, HIGH);
    digitalWrite(pinDrive2, LOW);
  }

  isMovingForward = true;
  isMovingBackward = false;
}

void Motor::backward(int speed = 100) {
  this->motorSpeed = speed;
  if (speed < 20) {
    analogWrite(pinEnable, LOW);
    digitalWrite(pinDrive1, LOW);
    digitalWrite(pinDrive2, LOW);
  } else {
    setPinSpeed();
    digitalWrite(pinDrive1, LOW);
    digitalWrite(pinDrive2, HIGH);
  }

  isMovingForward = false;
  isMovingBackward = true;
}

void Motor::step(int speed = 100) {
  if (speed > 0) {
    forward(abs(speed));
  } else if (speed < 0) {
    backward(abs(speed));
  } else {
    stop();
  }
}

void Motor::stop() {
  this->motorSpeed = 0;
  analogWrite(pinEnable, LOW);
  digitalWrite(pinDrive1, LOW);
  digitalWrite(pinDrive2, LOW);

  isMovingForward = false;
  isMovingBackward = false;
}

int Motor::getEncoderTickCount() {
  return encoderTickCount;
}

int Motor::getPreviousEncoderTickCount() {
  float result = previousEncoderTickCount;
  previousEncoderTickCount = encoderTickCount;
  return result;
}

int Motor::getEncoderTickCountDelta() {
  float tickCount = getEncoderTickCount();
  float tickCountPrevious = getPreviousEncoderTickCount();
  return tickCount - tickCountPrevious;
}

void Motor::resetEncoderTickCount() {
  encoderTickCount = 0;
}

void Motor::incrementEncoderTickCount() {
  encoderTickCount++;
}

void Motor::decrementEncoderTickCount() {
  encoderTickCount--;
}

bool Motor::isFlagged() {
  return flagExecute;
}

void Motor::prepareCommand(int motorSpeed, int duration) {
  flagExecute = true;
  flagExecuteSpeed = motorSpeed;
  flagExecuteDuration = duration;
  flagExecuteExpiration = millis() + duration;
}

void Motor::executePreparedCommand() {
  if (flagExecute == true && millis() < flagExecuteExpiration) {
    this->step(flagExecuteSpeed);
  } else if (millis() > flagExecuteExpiration) {
    this->step(0);
    flagExecute = false;
  } else {
    this->step(0);
  }
}

