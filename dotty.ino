#include <LedControl.h>
// #include "send.h"
#include <stdio.h>
#include <stdlib.h>

// Pin assignments for the MAX7219
const int dataIn = 7;  // Data In
const int clk = 6;     // Clock
const int cs1 = 5;      // Chip Select
const int cs2 = 4;

const int playButton = 11;
int playButtonState = 0;
bool gamePaused = true;

const int resetButton = 12;
int resetButtonState = 0;

int initialDelay = 75;
int _delay = initialDelay;
int speedUpFactor = 5;

int P1Score = 0;
int P2Score = 0;
int winningScore = 3;

#define JH1 A0
#define JV1 A1
#define JH2 A2
#define JV2 A3

// Create a new LedControl object
LedControl lc1 = LedControl(dataIn, clk, cs1, 4);  // (DataIn, CLK, CS, number of devices)
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

int H1;
int V1;
int H2;
int V2;

void setup() {
  turnOnLeds();
  Serial.begin(9600);
  Serial.println("Start");
  initialAnimation(25 , 3);
}

void loop() {

  while (true) {
    isGameWon();
    playButtonState = digitalRead(playButton);
    resetButtonState = digitalRead(resetButton);
    if(playButtonState == HIGH) {
      delay(200);
      gamePaused = !gamePaused;
    }
    if(resetButtonState == HIGH) {
      delay(200);
      resetGame();
    }
    if(gamePaused == false) {
      H1 = analogRead(JH1);
      V1 = analogRead(JV1);
      H2 = analogRead(JH2);
      V2 = analogRead(JV2);

      // makeBothPaddles();
      moveLeftBat(H1);
      moveRightBat(H2);
      checkPaddleHitPoint();
      //  CHECK IF POINT SCORED
      
      String playerScored = checkIfPointScored();
      if (playerScored != "") {
        pointScoredAnimation(playerScored);
        clearDisplay();
        _delay = initialDelay;
        x = 7;
        y = 0;
        matNum = 1;
        ballPath = "horizontal";
        lightLED(matNum,x,y);
        delay(250);
        makeBothPaddles();
        delay(500);
      }

      if (hDirection == "right") {
        if (y < 0) {
          matNum = matNum + 1;
          y = 7;
        }
        if(x == 0) {
          vDirection = "up";
        }
        if(x == 15) {
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
        if(x == 0) {
          vDirection = "up";
        }
        if(x == 15) {
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
    else {                              //  GAME PAUSED
        lightLED(matNum, x , y);
        delay(_delay);
        H1 = analogRead(JH1);
        V1 = analogRead(JV1);
        H2 = analogRead(JH2);
        V2 = analogRead(JV2);
        moveLeftBat(H1);
        moveRightBat(H2);
        if(resetButtonState == HIGH) {
          delay(200);
          resetGame();
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

void sendPostRequest(char *player1, char *player2)
{
   FILE *file;
   file = fopen("score.txt", "w");
   fprintf(file, "%s %s\n", player1, player2);
   fclose(file);
}

String checkIfPointScored() {
  //P1 Score Update
  int temp1 = P1Score, temp2 = P2Score;
  if (matNum == 3 && y == 0 && (x < rightPaddleStartX || x >= rightPaddleStartX + paddleSize)) {
    P1Score++;
  }
  //P2 Score Update
  if (matNum == 0 && y == 7 && (x < leftPaddleStartX || x >= leftPaddleStartX + paddleSize)) {
    P2Score++;
  }

  char p1[10], p2[10];
  itoa(P1Score, p1, 10);
  itoa(P2Score, p2, 10);

  // sendPostRequest(p1,p2);

  if (temp1 != P1Score) {
    Serial.print("Player 1 Score: ");
    Serial.println(P1Score);
    return "Player 1";
  }
  if (temp2 != P2Score) {
    Serial.print("Player 2 Score: ");
    Serial.println(P2Score);
    return "Player 2";
  }

  return "";
}

void checkPaddleHitPoint() {
  if ((matNum == 3 && y == 0 && (x >= rightPaddleStartX || x < rightPaddleStartX + paddleSize))) {
    hDirection = "left";
    if ( (x == rightPaddleStartX+2)) {
      ballPath = "horizontal";
    }
    else {
      ballPath = "diagonal";
      if((x == rightPaddleStartX + (paddleSize-1)) || (x == rightPaddleStartX + (paddleSize-2))) {
        vDirection = "up";
      }
      else if (x == rightPaddleStartX || x == (rightPaddleStartX + 1)) {
        vDirection = "down";
      }
    }
    if (_delay - speedUpFactor > 0) {
      _delay -= speedUpFactor;
    }
  }
  if ((matNum == 0 && y == 7 && (x >= leftPaddleStartX || x < leftPaddleStartX + paddleSize))) {
    hDirection = "right";
    if ((x == leftPaddleStartX + 2)) {
      ballPath = "horizontal";
    }
    else {
      ballPath = "diagonal";
      if((x == leftPaddleStartX + (paddleSize-1)) || (x == leftPaddleStartX + (paddleSize-2))) {
        vDirection = "up";
      }
      else if (x == leftPaddleStartX || x == (leftPaddleStartX + 1)) {
        vDirection = "down";
      }
    }
    if (_delay - speedUpFactor > 0) {
      _delay -= speedUpFactor;
    }
  }
}

void pointScoredAnimation (String player) {
  clearDisplay();
  lightLED(matNum, x, y);
  delay(1000);
  clearDisplay();
  
  byte G1[8] = {B00111100 , B01000010 , B01000010 , B00000010,
                B00110010 , B01000010 , B01000010 , B00111100};
  byte O1[8] = {B00111100 , B01000010 , B01000010 , B01000010,
                B01000010 , B01000010 , B01000010 , B00111100};
  byte A1[8] = {B00011000 , B00100100 , B01000010 , B01000010,
                B01111110 , B01000010 , B01000010 , B01000010};
  byte L1[8] = {B00000010 , B00000010 , B00000010 , B00000010,
                B00000010 , B00000010 , B00000010 , B01111110};
  
  byte G2[8] = {B00111100, B01000010, B01000010, B01001100,
                B01000000, B01000010, B01000010, B00111100};
  byte O2[8] = {B00111100, B01000010, B01000010, B01000010,
                B01000010, B01000010, B01000010, B00111100};
  byte A2[8] = {B01000010, B01000010, B01000010, B01111110,
                B01000010, B01000010, B00100100, B00011000};
  byte L2[8] = {B01111100, B01000000, B01000000, B01000000,
                B01000000, B01000000, B01000000, B01000000};

  for(int j = 1 ; j<=4 ; j++){
    for (int i=0 ; i<8 ; i++) {
      if (j == 1){
        lc1.setColumn(3,i,G1[i]);
        lc2.setColumn(0,i,G2[i]);
      } 
      if (j == 2){
        lc1.setColumn(2,i,O1[i]);
        lc2.setColumn(1,i,O2[i]);
      }  
      if (j == 3){
        lc1.setColumn(1,i,A1[i]);
        lc2.setColumn(2,i,A2[i]);
      }
      if (j == 4){
        lc1.setColumn(0,i,L1[i]);
        lc2.setColumn(3,i,L2[i]);
      }
    }
    delay(250);
  }
}

void isGameWon() {
  if(P1Score == winningScore || P2Score == winningScore) {
    resetGame();
  }
} 

void resetGame () {
  clearDisplay();
  _delay = initialDelay;
  initialAnimation(25 , 1);
  P1Score = 0;
  P2Score = 0;
  x = 7;  
  y = 0;  
  matNum = 1; 
  gamePaused = true;
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

  // DOT MATRIX 1
  if (x >= 0 && x <= 7) {
    lc1.setLed(matNum, x, y, true);
  }
  // DOT MATRIX 2
  else if (x >=8 && x <=15) {
    x -= 8;
    lc2.setLed(matNum, x, y, true);
  }
}

void clearLED(int matNum , int x , int y) {
  
  // DOT MATRIX 1
  if (x >= 0 && x <= 7) {
    lc1.setLed(matNum, x, y, false);
  }
  // DOT MATRIX 2
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