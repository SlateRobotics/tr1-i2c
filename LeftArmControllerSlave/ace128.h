#ifndef ACE128_H
#define ACE128_H

class Ace128 {
  private:
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
    int p6;
    int p7;
    int p8;
  
  public:
    void setUp();
    void printByte();
    float readDegree(bool reverseAngle = false);
    int readPosition(bool reverseAngle = false);
    Ace128();
    Ace128(int, int, int, int, int, int, int, int);
    ~Ace128();
};

#endif
