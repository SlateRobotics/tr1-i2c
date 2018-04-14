#ifndef EMS22A_H
#define EMS22A_H

class Ems22a {
  private:
    int PIN_INPUT;
    int PIN_CLOCK;
    int PIN_GND;
    int PIN_DATA;
    int PIN_VCC;
    int PIN_CS;
  
  public:
    void setUp();
    float readDegree(bool reverseAngle = false);
    int readPosition(bool reverseAngle = false);
    Ems22a();
    Ems22a(int, int, int, int, int, int);
    ~Ems22a();
};

#endif
