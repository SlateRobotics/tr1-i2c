#ifndef INTERRUPT_H
#define INTERRUPT_H

volatile int interruptBR_pulses, interruptFR_pulses, interruptBL_pulses, interruptFL_pulses;

static void interruptBR() {
  int interruptBR_A = 18;
  int interruptBR_B = 27;
  if (digitalRead(interruptBR_B) == 0) {
    if (digitalRead(interruptBR_A) == 0) {
      interruptBR_pulses--;
    } else {
      interruptBR_pulses++;
    }
  } else {
    if (digitalRead(interruptBR_A) == 0) {
      interruptBR_pulses++;
    } else {
      interruptBR_pulses--;
    }
  }
}

static void interruptFR() {
  int interruptFR_A = 3;
  int interruptFR_B = 23;
  if (digitalRead(interruptFR_B) == 0) {
    if (digitalRead(interruptFR_A) == 0) {
      interruptFR_pulses--;
    } else {
      interruptFR_pulses++;
    }
  } else {
    if (digitalRead(interruptFR_A) == 0) {
      interruptFR_pulses++;
    } else {
      interruptFR_pulses--;
    }
  }
}

void interruptBL() {
  int interruptBL_A = 19;
  int interruptBL_B = 28;
  if (digitalRead(interruptBL_B) == 0) {
    if (digitalRead(interruptBL_A) == 0) {
      interruptBL_pulses--;
    } else {
      interruptBL_pulses++;
    }
  } else {
    if (digitalRead(interruptBL_A) == 0) {
      interruptBL_pulses++;
    } else {
      interruptBL_pulses--;
    }
  }
}

void interruptFL() {
  int interruptFL_A = 2;
  int interruptFL_B = 22;
  if (digitalRead(interruptFL_B) == 0) {
    if (digitalRead(interruptFL_A) == 0) {
      interruptFL_pulses--;
    } else {
      interruptFL_pulses++;
    }
  } else {
    if (digitalRead(interruptFL_A) == 0) {
      interruptFL_pulses++;
    } else {
      interruptFL_pulses--;
    }
  }
}

#endif
