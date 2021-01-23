#include <Arduino.h>
#include <ChessClock.h>

#define DIGIT_EMPTY 20

const byte digit[21] = {
    B11111101, //0
    B01100001, //1
    B11011011, //2
    B11110011, //3
    B01100111, //4
    B10110111, //5
    B10111111, //6
    B11100101, //7
    B11111111, //8
    B11110111, //9
    B11111100, //0.
    B01100000, //1.
    B11011010, //2.
    B11110010, //3.
    B01100110, //4.
    B10110110, //5.
    B10111110, //6.
    B11100100, //7.
    B11111110, //8.
    B11110110, //9.
    B00000001  //empty
};

const PROGMEM int clock = 13;
const PROGMEM int latch = 12;
const PROGMEM int data = 8;

const PROGMEM int digit1 = 10;
const PROGMEM int digit2 = 2;
const PROGMEM int digit3 = 3;
const PROGMEM int digit4 = 4;
const PROGMEM int digit5 = 5;
const PROGMEM int digit6 = 6;
const PROGMEM int digit7 = 7;
const PROGMEM int digit8 = 9;
const PROGMEM int leftLED = A0;
const PROGMEM int rightLED = A1;
const PROGMEM int rightButton = A3;
const PROGMEM int leftButton = A2;
const PROGMEM int buzzerPin = A5;
const PROGMEM int menuButton = A6;
const PROGMEM int backButton = A7;
const PROGMEM int resetPin = 11;

int digitDelay = 1;

int leftButtonState = 0;
int rightButtonState = 0;
int menuButtonState = 0;
int backButtonState = 0;

bool leftPressed = false;
bool rightPressed = false;
bool backPressed = false;
bool menuPressed = false;
bool isGameFinished = false;

long lastMilis = 0;

uint32_t milisToHours(uint32_t time)
{
  return time / 3600000;
}
uint32_t milisToMinutes(uint32_t time)
{
  uint32_t value = time - milisToHours(time) * 3600000;
  return value / 60000;
}
uint32_t milisToSeconds(uint32_t time)
{
  uint32_t value = time - milisToHours(time) * 3600000 - milisToMinutes(time) * 60000;
  return value / 1000;
}

void screenUpdateLeft(uint32_t time){
  int C1,C2,C3,C4;
  int h = milisToHours(time);
  int m = milisToMinutes(time);
  int s = milisToSeconds(time);
  if(h==0 && m<30){
    C1 = m / 10;
    C2 = m % 10;
    C3 = s / 10;
    C4 = s % 10;
  }else{
    C1 = h / 10;
    C2 = h % 10;
    C3 = m / 10;
    C4 = m % 10;
  }
  //DITIT 1
  shiftOut(data, clock, MSBFIRST, digit[C1]);
  digitalWrite(latch, LOW);
  digitalWrite(digit1, LOW);
  delay(digitDelay);
  digitalWrite(digit1, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 2
  shiftOut(data, clock, MSBFIRST, digit[C2]);
  digitalWrite(latch, LOW);
  digitalWrite(digit2, LOW);
  delay(digitDelay);
  digitalWrite(digit2, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 3
  shiftOut(data, clock, MSBFIRST, digit[C3]);
  digitalWrite(latch, LOW);
  digitalWrite(digit3, LOW);
  delay(digitDelay);
  digitalWrite(digit3, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 4
  shiftOut(data, clock, MSBFIRST, digit[C4]);
  digitalWrite(latch, LOW);
  digitalWrite(digit4, LOW);
  delay(digitDelay);
  digitalWrite(digit4, HIGH);
  digitalWrite(latch, HIGH);
}
void screenUpdateRight(uint32_t time){
  int C5, C6, C7, C8;
  int h = milisToHours(time);
  int m = milisToMinutes(time);
  int s = milisToSeconds(time);
  if (h == 0 && m < 30)
  {
    C5 = m / 10;
    C6 = m % 10;
    C7 = s / 10;
    C8 = s % 10;
  }else{
    C5 = h / 10;
    C6 = h % 10;
    C7 = m / 10;
    C8 = m % 10;
  }
  //DIGIT 5
  shiftOut(data, clock, MSBFIRST, digit[C5]);
  digitalWrite(latch, LOW);
  digitalWrite(digit5, LOW);
  delay(digitDelay);
  digitalWrite(digit5, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 6
  shiftOut(data, clock, MSBFIRST, digit[C6]);
  digitalWrite(latch, LOW);
  digitalWrite(digit6, LOW);
  delay(digitDelay);
  digitalWrite(digit6, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 7
  shiftOut(data, clock, MSBFIRST, digit[C7]);
  digitalWrite(latch, LOW);
  digitalWrite(digit7, LOW);
  delay(digitDelay);
  digitalWrite(digit7, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 8
  shiftOut(data, clock, MSBFIRST, digit[C8]);
  digitalWrite(latch, LOW);
  digitalWrite(digit8, LOW);
  delay(digitDelay);
  digitalWrite(digit8, HIGH);
  digitalWrite(latch, HIGH);
}
void updateLed(int currentPlayer){
  if(currentPlayer == PLAYER_LEFT){
    digitalWrite(leftLED, HIGH);
    digitalWrite(rightLED, LOW);
  }else if(currentPlayer == PLAYER_RIGHT){
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, HIGH);
  }else{
    digitalWrite(leftLED, LOW);
    digitalWrite(rightLED, LOW);
  }
}
void gameEnded(){
  tone(buzzerPin, 330, 50);
  delay(100);
  tone(buzzerPin, 330, 50);
  delay(100);
  tone(buzzerPin, 330, 50);
}
ChessClock chessClock;

void setup(){
    pinMode(latch, OUTPUT);
    pinMode(clock, OUTPUT);
    pinMode(data, OUTPUT);
    pinMode(digit1, OUTPUT);
    pinMode(digit2, OUTPUT);
    pinMode(digit3, OUTPUT);
    pinMode(digit4, OUTPUT);
    pinMode(digit5, OUTPUT);
    pinMode(digit6, OUTPUT);
    pinMode(digit7, OUTPUT);
    pinMode(digit8, OUTPUT);
    pinMode(leftButton, INPUT);
    pinMode(rightButton, INPUT);
    pinMode(backButton, INPUT);
    pinMode(menuButton, INPUT);
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
    digitalWrite(digit5, HIGH);
    digitalWrite(digit6, HIGH);
    digitalWrite(digit7, HIGH);
    digitalWrite(digit8, HIGH);
    digitalWrite(resetPin, HIGH);
    pinMode(resetPin, OUTPUT);
    lastMilis = millis();

    chessClock.setLeftPlayerTime(10000);
    chessClock.setRightPlayerTime(10000);
    chessClock.setClockMode(MODE_TOURNAMENT);
    chessClock.ledIndicatorEnabled = true;
    chessClock.setIncrementValue(5);
    chessClock.setDelayValue(3); 
    chessClock.setBronsteinValue(5);
    chessClock.setTournamentMoveCap(10);
    chessClock.setTournamentPreCapIncrement(5);
    chessClock.setTournamentPostCapIncrement(10);
    chessClock.setTournamentAdditionalTimeAfterCap(30000);
}

void loop(){
  leftButtonState = analogRead(leftButton);
  rightButtonState = analogRead(rightButton);
  menuButtonState = analogRead(menuButton);
  backButtonState = analogRead(backButton);
  if(leftButtonState > 800 && !leftPressed){
    //LEFT BUTTON PRESS
    if(chessClock.getCurrentPlayer() != PLAYER_RIGHT){
      tone(buzzerPin, 330, 50);
      chessClock.playerMove(PLAYER_LEFT);
    }
    leftPressed = true;
  }else if(leftButtonState<100){
    leftPressed = false;
  }

  if(rightButtonState > 800 && !rightPressed){
    //RIGHT BUTTON PRESS
    if(chessClock.currentPlayer != PLAYER_LEFT){
      tone(buzzerPin, 330, 50);
      chessClock.playerMove(PLAYER_RIGHT);
    }
    rightPressed = true;
  }else if(rightButtonState<100){
    rightPressed = false;
  }

  if(backButtonState > 800 && !backPressed){
    //BACK BUTTON PRESS
    tone(buzzerPin, 330, 50);
    chessClock.playerMove(PLAYER_NONE);
    backPressed = true;
  }else if(backButtonState<100){
    backPressed = false;
  }

  if(menuButtonState > 800 && !menuPressed){
    //MENU BUTTON PRESS
    tone(buzzerPin, 330, 50);
    digitalWrite(resetPin, LOW);
    menuPressed = true;
  }else if(menuButtonState<100){
    menuPressed = false;
  }
  updateLed(chessClock.getCurrentPlayer());
  if(chessClock.getGameResult() != PLAYER_NONE && isGameFinished == false){
    gameEnded();
    isGameFinished = true;
  }

  screenUpdateLeft(chessClock.getLeftPlayerTime());
  screenUpdateRight(chessClock.getRightPlayerTime());
  chessClock.timeUpdate(millis()-lastMilis);
  lastMilis = millis();
}
