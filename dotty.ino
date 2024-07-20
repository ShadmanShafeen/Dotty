#include <LedControl.h>

#include <MD_Parola.h>     
#include <MD_MAX72xx.h>    
#include <SPI.h>  

// Pin assignments for the MAX7219
const int dataIn = 7;  // Data In
const int clk = 6;     // Clock
const int cs = 5;      // Chip Select
const int cs2 = 4;
byte matNum = 1;      //  CURRENT 8X8 DOT MATRIX
String hDirection = "right";
String vDirection = "up";
String ballPath = "horizontal";

int initialDelay = 50;
int _delay = initialDelay;
int speedUpFactor = 5;

const int paddleSize = 3;        
int leftPaddleStartX = 3;     //  LEFT PADDLE'S INITIAL X (ROW) VALUE      
int righttPaddleStartX = 3;   //  RIGHT PADDLE'S INITIAL X (ROW) VALUE
int current32 = 1;  // 1 is the one with yellow cs pin

int P1Score = 0;
int P2Score = 0;

#define JH1 A0
#define JV1 A1
#define JH2 A2
#define JV2 A3

// Create a new LedControl object
LedControl lc = LedControl(dataIn, clk, cs, 4);  // (DataIn, CLK, CS, number of devices)
LedControl lc2 = LedControl(dataIn, clk, cs2, 4);

            // BALL COORDINATES
int x = 4;  // ROW
int y = 0;  // COLUMN


void setup() {

  turnOnLeds();
  Serial.begin(9600);
  Serial.println("Start");
  lightLED(current32 , matNum , 4 , 0);
  delay(1000);
  makeBothPaddles();
  delay(1000);
}

void loop() {
  
  while (true) {
    // clear8x8DotMatrix(2, 0);
    // delay(_delay);
    // lightLED(2, 0, x, y);
    int H1 = analogRead(JH1);
    int V1 = analogRead(JV1);
    int H2 = analogRead(JH2);
    int V2 = analogRead(JV2);

    // makeBothPaddles();
    moveLeftBat(H1);
    moveRightBat(H2);
    checkPaddleHitPoint();
    bool pointUpdated = checkIfPointScored();
    if (pointUpdated) {
      _delay = initialDelay;
      for (int i = 0; i <= 3; i++) {
        clear8x8DotMatrix(current32, i);
      }
      lightLED(current32, matNum, x, y);
      delay(2000);
      for (int i = 0; i <= 3; i++) {
        clear8x8DotMatrix(current32, i);
      }
      x = 4;
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

      if (x == 7) {
        vDirection = "down";
      }
      if (x == 0) {
        vDirection = "up";
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

      if (x == 7) {
        vDirection = "down";
      }
      if (x == 0) {
        vDirection = "up";
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

    // clear8x8DotMatrix(current32, matNum);
  }
}

void moveRight(int& x, int& y)
{
  lightLED(current32, matNum, x, y);
  delay(_delay);
  clear8x8DotMatrix(current32, matNum);
  --y;
}

void moveLeft(int& x, int& y)
{
  lightLED(current32, matNum, x, y);
  delay(_delay);
  clear8x8DotMatrix(current32, matNum);
  ++y;
}

void moveUpRight(int& x, int& y) {
  lightLED(current32, matNum, x, y);
  delay(_delay);
  clear8x8DotMatrix(current32, matNum);
  --y;
  ++x;
}

void moveDownRight(int& x, int& y) {
  lightLED(current32, matNum, x, y);
  delay(_delay);
  clear8x8DotMatrix(current32, matNum);
  --y;
  --x;
}

void moveUpLeft(int& x, int& y) {
  lightLED(current32, matNum, x, y);
  delay(_delay);
  clear8x8DotMatrix(current32, matNum);
  ++y;
  ++x;
}

void moveDownLeft(int& x, int& y) {
  lightLED(current32, matNum, x, y);
  delay(_delay);
  clear8x8DotMatrix(current32, matNum);
  ++y;
  --x;
}


void makeBothPaddles() {

  for (int i = leftPaddleStartX; i < leftPaddleStartX + paddleSize; i++) {
    int temp = 7;
    lightLED(current32, 0, i, temp);
  }
  for (int i = righttPaddleStartX; i < righttPaddleStartX + paddleSize; i++) {
    int temp = 0;
    lightLED(current32, 3, i, temp);
  }
}

void moveLeftBat(int H1) {
  if (H1 > 0 && H1 < 450) {
    if (leftPaddleStartX > 0) {
      clearLED(current32 , 0 , leftPaddleStartX + 2 , 7);
      leftPaddleStartX = leftPaddleStartX - 1;
    }
  }
  if (H1 > 650 && H1 < 1032) {
    if (leftPaddleStartX < 5) {
      clearLED(current32 , 0 , leftPaddleStartX , 7);
      leftPaddleStartX = leftPaddleStartX + 1;
    }
  }
  //  MAKE LEFT PADDLE
  for (int i = leftPaddleStartX; i < leftPaddleStartX + paddleSize; i++) {
    int temp = 7;
    lightLED(current32, 0, i, temp);
  }
}

void moveRightBat(int H2) {
  if (H2 > 0 && H2 < 450) {
    if (righttPaddleStartX > 0) {
      clearLED(current32 , 3 , righttPaddleStartX + 2 , 0);
      righttPaddleStartX = righttPaddleStartX - 1;
    }
  }
  if (H2 > 650 && H2 < 1032) {
    if (righttPaddleStartX < 5) {
      clearLED(current32 , 3 , righttPaddleStartX , 0);
      righttPaddleStartX = righttPaddleStartX + 1;
    }
  }
  //  MAKE RIGHT PADDLE
  for (int i = righttPaddleStartX; i < righttPaddleStartX + paddleSize; i++) {
    int temp = 0;
    lightLED(current32, 3, i, temp);
  }
}

bool checkIfPointScored() {
  //P1 Score Update
  int temp1 = P1Score, temp2 = P2Score;
  if (matNum == 3 && y == 0 && (x < righttPaddleStartX || x >= righttPaddleStartX + paddleSize)) {
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

  if (temp1 != P1Score || temp2 != P2Score) {
    _delay = initialDelay;
  }
  return false;
}

void checkPaddleHitPoint() {
  if ((matNum == 3 && y == 0 && (x >= righttPaddleStartX || x < righttPaddleStartX + paddleSize))) {
    hDirection = "left";
    if ((x == righttPaddleStartX + 1)) {
      ballPath = "horizontal";
    }
    else {
      ballPath = "diagonal";
      if(x == righttPaddleStartX + 2) {
        vDirection = "up";
      }
      else if (x == righttPaddleStartX) {
        vDirection = "down";
      }
    }
    if (_delay - speedUpFactor > 0) {
      _delay -= speedUpFactor;
    }
  }
  if ((matNum == 0 && y == 7 && (x >= leftPaddleStartX || x < leftPaddleStartX + paddleSize))) {
    hDirection = "right";
    if ((x == leftPaddleStartX + 1)) {
      ballPath = "horizontal";
    }
    else {
      ballPath = "diagonal";
      if(x == leftPaddleStartX + 2) {
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
  for(int i = 0 ; i <= 3 ; i++) {
    clear8x8DotMatrix(current32 , i);
  }
  P1Score = 0;
  P2Score = 0;
  x = 4;
  y = 0;
  matNum = 1;
  lightLED(current32 , matNum , x , y);
  delay(1000);
}

void pauseGame () {
  
}

void lightLED(int _ledNo, int matNum, int x, int y) {
  if (_ledNo == 1) lc.setLed(matNum, x, y, true);
  else lc2.setLed(matNum, x, y, true);
}

void clearLED(int _ledNo, int matNum , int x , int y) {
  if (_ledNo == 1) lc.setLed(matNum, x, y, false);
  else lc2.setLed(matNum, x, y, false); 
}

void clear8x8DotMatrix(int _ledNo, int matNum) {
  if (_ledNo == 1) lc.clearDisplay(matNum);
  else lc2.clearDisplay(matNum);
}

void turnOnLeds() {
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.shutdown(2, false);
  lc.shutdown(3, false);

  // lc2.shutdown(0, false);
  // lc2.shutdown(1, false);
  // lc2.shutdown(2, false);
  // lc2.shutdown(3, false);

  lc.setIntensity(0, 8);
  lc.setIntensity(1, 8);
  lc.setIntensity(2, 8);
  lc.setIntensity(3, 8);
  lc.clearDisplay(0);

  // lc2.setIntensity(0, 8);
  // lc2.clearDisplay(0);
}