#include <LedControl.h>

#include <MD_Parola.h>     
#include <MD_MAX72xx.h>    
#include <SPI.h>  

// Pin assignments for the MAX7219
const int dataIn = 7;  // Data In
const int clk = 6;     // Clock
const int cs = 5;      // Chip Select
const int cs2 = 4;

int initialDelay = 50;
int _delay = initialDelay;
int speedUpFactor = 5;

// int current32 = 1;  // 1 is the one with yellow cs pin

int P1Score = 0;
int P2Score = 0;

#define JH1 A0
#define JV1 A1
#define JH2 A2
#define JV2 A3

// Create a new LedControl object
LedControl lc1 = LedControl(dataIn, clk, cs, 4);  // (DataIn, CLK, CS, number of devices)
LedControl lc2 = LedControl(dataIn, clk, cs2, 4);

//                                          BALL INITIAL COORDINATES
int x = 7;  // ROW
int y = 0;  // COLUMN
byte matNum = 1;      //  CURRENT 8X8 DOT MATRIX
String hDirection = "right";
String vDirection = "up";
String ballPath = "horizontal";

//                                          PADDLE INITIAL VALUES
const int paddleSize = 5;        
int leftPaddleStartX = 5;     //  LEFT PADDLE'S INITIAL X (ROW) VALUE      
int rightPaddleStartX = 5;   //  RIGHT PADDLE'S INITIAL X (ROW) VALUE
void setup() {
  turnOnLeds();
  Serial.begin(9600);
  Serial.println("Start");
  initialAnimation(50 , 3);
}

void loop() {
  
  while (true) {
    int H1 = analogRead(JH1);
    int V1 = analogRead(JV1);
    int H2 = analogRead(JH2);
    int V2 = analogRead(JV2);

    // makeBothPaddles();
    moveLeftBat(H1);
    moveRightBat(H2);
    checkPaddleHitPoint();
    
    //  CHECK IF POINT SCORED
    bool pointUpdated = checkIfPointScored();
    if (pointUpdated) {
      _delay = initialDelay;
      clearDisplay();
      lightLED(matNum, x, y);
      delay(2000);
      clearDisplay();
      x = 7;
      y = 0;
      matNum = 1;
      ballPath = "horizontal";
    }
    if (hDirection == "right") {
      if (y < 0) {
        matNum = matNum + 1;
        y = 7;
      }
      // if ((y == 1 && matNum == 3)) {
      //   hDirection = "left";
      // }

      if(x == 1) {
        vDirection = "up";
      }
      if(x == 14) {
        vDirection = "down";
      }
      if (ballPath == "diagonal") {
        if (vDirection == "up") {
          moveUpRight(x, y);
        }
        else {
          moveDownRight(x, y);
        }
      }
      else {
        moveRight(x,y);
      } 
    }

    else if (hDirection == "left") {
      if (y > 7) {
        matNum = matNum - 1;
        y = 0;
      }
      // if (y == 6 && matNum == 0) {
      //   hDirection = "right";
      // }

      if(x == 1) {
        vDirection = "up";
      }
      if(x == 14) {
        vDirection = "down";
      }
      
      if (ballPath == "diagonal") {
        if (vDirection == "up"){
          moveUpLeft(x, y);
        } 
        else {
          moveDownLeft(x, y);
        } 
      } 
      else {
        moveLeft(x,y);
      }
      
    }
  }
}

void moveRight(int& x, int& y)
{
  lightLED(matNum, x, y);
  delay(_delay);
  clearLED(matNum, x, y);
  --y;
}

void moveLeft(int& x, int& y)
{
  lightLED(matNum, x, y);
  delay(_delay);
  clearLED(matNum, x, y);
  ++y;
}

void moveUpRight(int& x, int& y) {
  lightLED(matNum, x, y);
  delay(_delay);
  clearLED(matNum, x, y);
  --y;
  ++x;
}

void moveDownRight(int& x, int& y) {
  lightLED(matNum, x, y);
  delay(_delay);
  clearLED(matNum, x, y);
  --y;
  --x;
}

void moveUpLeft(int& x, int& y) {
  lightLED(matNum, x, y);
  delay(_delay);
  clearLED(matNum, x, y);
  ++y;
  ++x;
}

void moveDownLeft(int& x, int& y) {
  lightLED(matNum, x, y);
  delay(_delay);
  clearLED(matNum, x, y);
  ++y;
  --x;
}


void makeBothPaddles() {

  for (int i = leftPaddleStartX; i < leftPaddleStartX + paddleSize; i++) {
    int temp = 7;
    lightLED(0, i, temp);
  }
  for (int i = rightPaddleStartX; i < rightPaddleStartX + paddleSize; i++) {
    int temp = 0;
    lightLED(3, i, temp);
  }
}

void moveLeftBat(int H1) {
  if (H1 > 0 && H1 < 450) {
    if (leftPaddleStartX > 0) {
      clearLED(0 , leftPaddleStartX + paddleSize - 1 , 7);
      leftPaddleStartX = leftPaddleStartX - 1;
    }
  }
  if (H1 > 650 && H1 < 1032) {
    if (leftPaddleStartX < 15 - (paddleSize - 1)) {
      clearLED(0 , leftPaddleStartX , 7);
      leftPaddleStartX = leftPaddleStartX + 1;
    }
  }
  //  MAKE LEFT PADDLE
  for (int i = leftPaddleStartX; i < leftPaddleStartX + paddleSize; i++) {
    int temp = 7;
    lightLED(0, i, temp);
  }
}

void moveRightBat(int H2) {
  if (H2 > 0 && H2 < 450) {
    if (rightPaddleStartX > 0) {
      clearLED(3 , rightPaddleStartX + paddleSize - 1 , 0);
      rightPaddleStartX = rightPaddleStartX - 1;
    }
  }
  if (H2 > 650 && H2 < 1032) {
    if (rightPaddleStartX < 15 - (paddleSize - 1)) {
      clearLED(3 , rightPaddleStartX , 0);
      rightPaddleStartX = rightPaddleStartX + 1;
    }
  }
  //  MAKE RIGHT PADDLE
  for (int i = rightPaddleStartX; i < rightPaddleStartX + paddleSize; i++) {
    int temp = 0;
    lightLED(3, i, temp);
  }
}

bool checkIfPointScored() {
  //P1 Score Update
  int temp1 = P1Score, temp2 = P2Score;
  if (matNum == 3 && y == 0 && (x < rightPaddleStartX || x >= rightPaddleStartX + paddleSize)) {
    P1Score++;
  }
  //P2 Score Update
  if (matNum == 0 && y == 7 && (x < leftPaddleStartX || x >= leftPaddleStartX + paddleSize)) {
    P2Score++;
  }
  if (temp1 != P1Score) {
    Serial.println("Player 1 Score: ");
    Serial.println(P1Score);
    return true;
  }

  if (temp2 != P2Score) {
    Serial.println("Player 2 Score: ");
    Serial.println(P2Score);
    return true;
  }
  return false;
}

void checkPaddleHitPoint() {
  if ((matNum == 3 && y == 0 && (x >= rightPaddleStartX || x < rightPaddleStartX + paddleSize))) {
    hDirection = "left";
    if ( (x >= rightPaddleStartX + 1) &&  (x <= rightPaddleStartX + (paddleSize-2))) {
      ballPath = "horizontal";
    }
    else {
      ballPath = "diagonal";
      if(x == rightPaddleStartX + (paddleSize-1)) {
        vDirection = "up";
      }
      else if (x == rightPaddleStartX) {
        vDirection = "down";
      }
    }
    if (_delay - speedUpFactor > 0) {
      _delay -= speedUpFactor;
    }
  }
  if ((matNum == 0 && y == 7 && (x >= leftPaddleStartX || x < leftPaddleStartX + paddleSize))) {
    hDirection = "right";
    if ((x >= leftPaddleStartX + 1) && (x <= leftPaddleStartX + (paddleSize-2))) {
      ballPath = "horizontal";
    }
    else {
      ballPath = "diagonal";
      if(x == leftPaddleStartX + (paddleSize-1)) {
        vDirection = "up";
      }
      else if (x == leftPaddleStartX) {
        vDirection = "down";
      }
    }
    if (_delay - speedUpFactor > 0) {
      _delay -= speedUpFactor;
    }
  }
}

void resetGame () {
  clearDisplay();
  P1Score = 0;
  P2Score = 0;
  x = 4;
  y = 0;
  matNum = 1;
  lightLED(matNum , x , y);
  delay(1000);
}

void pauseGame (bool gamePaused) {
  while (gamePaused){

  }
}

void initialAnimation (int iADelay , int count) {
  int initialAnimationDelay = iADelay;
  int pulseCount = count;
  for(int pc = 1 ; pc <= pulseCount ; pc++) {
    for(int i=8 ; i>=0 ; i--) {
      lc1.setIntensity(0, i);
      lc1.setIntensity(1, i);
      lc1.setIntensity(2, i);
      lc1.setIntensity(3, i);
      lc2.setIntensity(0, i);
      lc2.setIntensity(1, i);
      lc2.setIntensity(2, i);
      lc2.setIntensity(3, i);
      lightLED(matNum , 7 , 0);
      delay(initialAnimationDelay);
      makeBothPaddles();
      delay(initialAnimationDelay);
    }
    for(int i=0 ; i<=8 ; i++) {
      lc1.setIntensity(0, i);
      lc1.setIntensity(1, i);
      lc1.setIntensity(2, i);
      lc1.setIntensity(3, i);
      lc2.setIntensity(0, i);
      lc2.setIntensity(1, i);
      lc2.setIntensity(2, i);
      lc2.setIntensity(3, i);
      lightLED(matNum , 7 , 0);
      delay(initialAnimationDelay);
      makeBothPaddles();
      delay(initialAnimationDelay);
    }
  }
}

void lightLED(int matNum, int x, int y) {

  //                        DOT MATRIX 1
  if (x >= 0 && x <= 7) {
    lc1.setLed(matNum, x, y, true);
  }
  //                        DOT MATRIX 2
  else if (x >=8 && x <=15) {
    x -= 8;
    lc2.setLed(matNum, x, y, true);
  }
  // if (_ledNo == 1) lc1.setLed(matNum, x, y, true);
  // else lc2.setLed(matNum, x, y, true);
}

void clearLED(int matNum , int x , int y) {
  
  //                        DOT MATRIX 1
  if (x >= 0 && x <= 7) {
    lc1.setLed(matNum, x, y, false);
  }
  //                        DOT MATRIX 2
  else if (x >=8 && x <= 15) {
    x -= 8;
    lc2.setLed(matNum, x, y, false);
  }
  // if (_ledNo == 1) lc1.setLed(matNum, x, y, false);
  // else lc2.setLed(matNum, x, y, false); 
}

void clear8x8DotMatrix(int _ledNo, int matNum) {
  if (_ledNo == 1) lc1.clearDisplay(matNum);
  else lc2.clearDisplay(matNum);
}

void clearDisplay() {
  for (int i=0 ; i<=3 ; i++) {
    lc1.clearDisplay(i);
    lc2.clearDisplay(i);
  }
}

void turnOnLeds() {
  lc1.shutdown(0, false);
  lc1.shutdown(1, false);
  lc1.shutdown(2, false);
  lc1.shutdown(3, false);

  lc2.shutdown(0, false);
  lc2.shutdown(1, false);
  lc2.shutdown(2, false);
  lc2.shutdown(3, false);

  lc1.setIntensity(0, 8);
  lc1.setIntensity(1, 8);
  lc1.setIntensity(2, 8);
  lc1.setIntensity(3, 8);
  lc1.clearDisplay(0);
  
  lc2.setIntensity(0, 8);
  lc2.setIntensity(1, 8);
  lc2.setIntensity(2, 8);
  lc2.setIntensity(3, 8);
  lc2.clearDisplay(0);



  // lc2.setIntensity(0, 8);
  // lc2.clearDisplay(0);
}