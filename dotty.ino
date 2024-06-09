#include "MaxMatrix.h"


int DIN = 7;
int CLK = 6;
int CS = 5;
byte matNum = 1;

String hDirection = "right";
String vDirection = "up";

byte buffer[20];
char text[] = "a";

MaxMatrix m(DIN, CS, CLK, matNum);

void setup() {
  m.init();
  m.setIntensity(5);
}

void loop() {
  int x=0;
  int y=0;
  while(true)
  {
    if(hDirection == "right") {
      if(x > 7) {
        matNum = matNum+1;
        m.setMatNum(matNum);
        x=0;
      }
      if(x == 6 && matNum == 4) {
           hDirection = "left";
      }

      if(y == 7) {
        vDirection = "down";
      }
      if(y == 0) {
        vDirection = "up";
      }
      if(vDirection == "up") moveUpRight(x,y);
      else moveDownRight(x,y);
    }

    else if(hDirection == "left") {
      if(x < 0) {
        matNum = matNum-1;
        m.setMatNum(matNum);
        x=7;
      }
      if(x == 1 && matNum == 1) {
          hDirection = "right";
      }

      if(y == 7) {
        vDirection = "down";
      }
      if(y == 0) {
        vDirection = "up";
      }
      if(vDirection == "up") moveUpLeft(x,y);
      else moveDownLeft(x,y);
    }
  }
  m.clear();
}

void moveRight(int& x, int y)
{
  m.setDot(y, x, true);  
    delay(200);
    m.clear();
    ++x;
}

void moveLeft(int& x, int y)
{
  m.setDot(y, x, true);
    delay(200);
    m.clear();
    --x;
}

void moveUpRight(int& x, int& y)
{
  m.setDot(y, x, true);
    delay(200);
    m.clear();
    ++y;
    ++x;
}
void moveDownRight(int& x, int& y)
{
  m.setDot(y, x, true);
    delay(200);
    m.clear();
    --y;
    ++x;
}

void moveUpLeft(int& x, int& y)
{
  m.setDot(y, x, true);
    delay(200);
    m.clear();
    ++y;
    --x;
}
void moveDownLeft(int& x, int& y)
{
  m.setDot(y, x, true);
    delay(200);
    m.clear();
    --y;
    --x;
}
