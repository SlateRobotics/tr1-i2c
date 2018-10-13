#ifndef INTERRUPT_H
#define INTERRUPT_H

volatile int interruptLA_pulses;

static void interruptLA_RISING() {
  interruptLA_pulses++;
}

#endif
