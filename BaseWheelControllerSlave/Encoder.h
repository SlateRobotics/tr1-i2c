#ifndef ENCODER_H
#define ENCODER_H

class Encoder {
  private:
    int pinPhaseA;
    int pinPhaseB;
  
  public:
    int id;
    void setUp(void (*interupt)());
    Encoder(int, int, int);
    ~Encoder();
};

#endif
