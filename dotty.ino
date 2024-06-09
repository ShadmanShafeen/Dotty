#include "MaxMatrix.h"


int DIN = 7;
int CLK = 6;
int CS = 5;
int matNum = 1;

String direction = "ltr";

byte buffer[20];
char text[] = "a";

MaxMatrix m(DIN, CS, CLK, matNum);

void setup() {
  m.init();
  m.setIntensity(8);
}

void loop() {
  int i=0;

  while(true)
  {
    if(direction == "ltr") {
      if(i > 7) {
        matNum = matNum+1;
        m.setMatNum(matNum);
        i=0;
      }
      if(i == 6 && matNum == 4) {
           direction = "rtl";
      }
      moveRight(i);
    }

    else if(direction == "rtl") {
      if(i < 0) {
        matNum = matNum-1;
        m.setMatNum(matNum);
        i=7;
      }
      if(i == 1 && matNum == 1) {
          direction = "ltr";
      }
      moveLeft(i);
    }
  }
  m.clear();
}

void moveRight(int& i)
{
  m.setDot(4, i, true);  
    delay(200);
    m.clear();
    ++i;
}

void moveLeft(int& i)
{
  m.setDot(4, i, true);
  delay(200);
  m.clear();
  --i;
}
