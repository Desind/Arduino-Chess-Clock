#include <Arduino.h>
#include <ChessClock.h>
#include <EEPROM.h>

#define DIGIT_EMPTY 20
#define CHAR_S 21
#define CHAR_E 22
#define CHAR_t 23
#define CHAR_H 24
#define CHAR_N 25
#define CHAR_n 26
#define CHAR_c 27

#define SCREEN_GAME 0
#define SCREEN_MENU_MODE 1
#define SCREEN_MENU_MODE_CHANGE 2
#define SCREEN_MENU_TIME 3
#define SCREEN_MENU_TIME_CHANGE 4
#define SCREEN_MENU_INCREMENT 5
#define SCREEN_MENU_INCREMENT_CHANGE 6
#define SCREEN_MENU_DELAY 7
#define SCREEN_MENU_DELAY_CHANGE 8
#define SCREEN_MENU_BRONSTEIN 9
#define SCREEN_MENU_BRONSTEIN_CHANGE 10
#define SCREEN_MENU_TOURNAMENT_INCA 11
#define SCREEN_MENU_TOURNAMENT_INCA_CHANGE 12
#define SCREEN_MENU_TOURNAMENT_CAP 13
#define SCREEN_MENU_TOURNAMENT_CAP_CHANGE 14
#define SCREEN_MENU_TOURNAMENT_ADD_TIME 15
#define SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE 16
#define SCREEN_MENU_TOURNAMENT_INCB 17
#define SCREEN_MENU_TOURNAMENT_INCB_CHANGE 18
#define SCREEN_MENU_TOURNAMENT_CAP2 19
#define SCREEN_MENU_TOURNAMENT_CAP2_CHANGE 20
#define SCREEN_MENU_TOURNAMENT_ADD_TIME2 21
#define SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE 22
#define SCREEN_MENU_TOURNAMENT_INCC 23
#define SCREEN_MENU_TOURNAMENT_INCC_CHANGE 24
#define SCREEN_MENU_MOVES 25
#define SCREEN_MENU_MOVES_CHANGE 26
#define SCREEN_MENU_SOUND 27
#define SCREEN_MENU_SOUND_CHANGE 28
#define SCREEN_MENU_PRESET 29
#define SCREEN_MENU_PRESET_LOAD 30
#define SCREEN_MENU_USER_PRESET 31
#define SCREEN_MENU_USER_PRESET_LOAD 32
#define SCREEN_MENU_USER_PRESET_SAVE 33
#define SCREEN_MENU_USER_PRESET_LOAD_PICK 34
#define SCREEN_MENU_USER_PRESET_SAVE_PICK 35


#define MOVE_COUNTER_SHOW_TIME 2000

const byte digit[28] = {
    B11111101, //0.
    B01100001, //1.
    B11011011, //2.
    B11110011, //3.
    B01100111, //4.
    B10110111, //5.
    B10111111, //6.
    B11100101, //7.
    B11111111, //8.
    B11110111, //9.
    B11111100, //0
    B01100000, //1
    B11011010, //2
    B11110010, //3
    B01100110, //4
    B10110110, //5
    B10111110, //6
    B11100100, //7
    B11111110, //8
    B11110110, //9
    B00000000, //empty
    B10110110, //S
    B10011110, //E 
    B00011110, //t
    B01101111, //H.
    B11101101, //N
    B00101010, //n
    B00011010, //c
};

const byte setupAnim[28] = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000000,
    B10000010,
    B10010010,
    B10010010,
    B10010010,
    B10010010,
    B00010010,
    B00010000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
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
int8_t lastPlayer = PLAYER_NONE;
bool gameStarted = false;
uint16_t flickerTime = 1000;
int currentTimeChangeSegment = 0;
uint16_t showCounterTime = 0;
uint8_t presetLoad = 0;
int8_t curPlayer = 0;

byte currentScreen = 0;

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
    if(C1 ==0) C1 = DIGIT_EMPTY;
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
  if(curPlayer==PLAYER_LEFT && (lastMilis % 1000 < 500) && !isGameFinished){
    shiftOut(data, clock, MSBFIRST, digit[C2+10]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[C2]);
  }
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
    if(C5 ==0) C5 = DIGIT_EMPTY;
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
  if(curPlayer==PLAYER_RIGHT && (lastMilis % 1000 < 500) && !isGameFinished){
    shiftOut(data, clock, MSBFIRST, digit[C6+10]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[C6]);
  }
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
void screenMovesLeft(int moves){
  shiftOut(data, clock, MSBFIRST, digit[DIGIT_EMPTY]);
  digitalWrite(latch, LOW);
  digitalWrite(digit1, LOW);
  delay(digitDelay);
  digitalWrite(digit1, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 2
  if(moves<100){
    shiftOut(data, clock, MSBFIRST, digit[moves / 10 + 10]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[moves / 100 + 10]);
  }
  digitalWrite(latch, LOW);
  digitalWrite(digit2, LOW);
  delay(digitDelay);
  digitalWrite(digit2, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 3
  if(moves<100){
    shiftOut(data, clock, MSBFIRST, digit[moves % 10+10]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[((moves / 10)-(moves/100)*10) + 10]);
  }
  digitalWrite(latch, LOW);
  digitalWrite(digit3, LOW);
  delay(digitDelay);
  digitalWrite(digit3, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 4
  if(moves<100){
    shiftOut(data, clock, MSBFIRST, digit[DIGIT_EMPTY]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[moves % 10 + 10]);
  }
  digitalWrite(latch, LOW);
  digitalWrite(digit4, LOW);
  delay(digitDelay);
  digitalWrite(digit4, HIGH);
  digitalWrite(latch, HIGH);
}
void screenMovesRight(int moves){
  shiftOut(data, clock, MSBFIRST, digit[DIGIT_EMPTY]);
  digitalWrite(latch, LOW);
  digitalWrite(digit5, LOW);
  delay(digitDelay);
  digitalWrite(digit5, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 6
  if(moves<100){
    shiftOut(data, clock, MSBFIRST, digit[moves / 10 + 10]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[moves / 100 + 10]);
  }
  digitalWrite(latch, LOW);
  digitalWrite(digit6, LOW);
  delay(digitDelay);
  digitalWrite(digit6, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 7
  if(moves<100){
    shiftOut(data, clock, MSBFIRST, digit[moves % 10+10]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[((moves / 10)-(moves/100)*10) + 10]);
  }
  digitalWrite(latch, LOW);
  digitalWrite(digit7, LOW);
  delay(digitDelay);
  digitalWrite(digit7, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 8
  if(moves<100){
    shiftOut(data, clock, MSBFIRST, digit[DIGIT_EMPTY]);
  }else{
    shiftOut(data, clock, MSBFIRST, digit[moves % 10 + 10]);
  }
  digitalWrite(latch, LOW);
  digitalWrite(digit8, LOW);
  delay(digitDelay);
  digitalWrite(digit8, HIGH);
  digitalWrite(latch, HIGH);
}
void updateLed(int currentPlayer){
  if(currentPlayer == PLAYER_LEFT){
    digitalWrite(leftLED, 255);
    digitalWrite(rightLED, 0);
  }else if(currentPlayer == PLAYER_RIGHT){
    digitalWrite(leftLED, 0);
    digitalWrite(rightLED, 255);
  }else{
    digitalWrite(leftLED, 0);
    digitalWrite(rightLED, 0);
  }
}
void screenUpdate(byte c1, byte c2, byte c3, byte c4, byte c5, byte c6, byte c7, byte c8){
  //DITIT 1
  shiftOut(data, clock, MSBFIRST, c1);
  digitalWrite(latch, LOW);
  digitalWrite(digit1, LOW);
  delay(digitDelay);
  digitalWrite(digit1, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 2
  shiftOut(data, clock, MSBFIRST, c2);
  digitalWrite(latch, LOW);
  digitalWrite(digit2, LOW);
  delay(digitDelay);
  digitalWrite(digit2, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 3
  shiftOut(data, clock, MSBFIRST, c3);
  digitalWrite(latch, LOW);
  digitalWrite(digit3, LOW);
  delay(digitDelay);
  digitalWrite(digit3, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 4
  shiftOut(data, clock, MSBFIRST, c4);
  digitalWrite(latch, LOW);
  digitalWrite(digit4, LOW);
  delay(digitDelay);
  digitalWrite(digit4, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 5
  shiftOut(data, clock, MSBFIRST, c5);
  digitalWrite(latch, LOW);
  digitalWrite(digit5, LOW);
  delay(digitDelay);
  digitalWrite(digit5, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 6
  shiftOut(data, clock, MSBFIRST, c6);
  digitalWrite(latch, LOW);
  digitalWrite(digit6, LOW);
  delay(digitDelay);
  digitalWrite(digit6, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 7
  shiftOut(data, clock, MSBFIRST, c7);
  digitalWrite(latch, LOW);
  digitalWrite(digit7, LOW);
  delay(digitDelay);
  digitalWrite(digit7, HIGH);
  digitalWrite(latch, HIGH);
  //DIGIT 8
  shiftOut(data, clock, MSBFIRST, c8);
  digitalWrite(latch, LOW);
  digitalWrite(digit8, LOW);
  delay(digitDelay);
  digitalWrite(digit8, HIGH);
  digitalWrite(latch, HIGH);
}
void setupAnimation(){
  for(int counter=7; counter<28; counter++){
    for (int i = 0; i < 6; i++)
    {
      screenUpdate(setupAnim[counter], setupAnim[counter-1], setupAnim[counter-2], setupAnim[counter-3], setupAnim[counter-4], setupAnim[counter-5], setupAnim[counter-6], setupAnim[counter-7]);
    }
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
  if(EEPROM.read(0)==255){
    chessClock.loadFromEEPROM(0);
  }else{
    chessClock.setLeftPlayerTime(300000);
    chessClock.setRightPlayerTime(300000);
    chessClock.setClockMode(MODE_SUDDEN_DEATH);
    chessClock.setIncrementValue(0);
    chessClock.setDelayValue(0);
    chessClock.setBronsteinValue(0);
    chessClock.setTournamentMoveCap(0);
    chessClock.setTournamentPreCapIncrement(0);
    chessClock.setTournamentPostCapIncrement(0);
    chessClock.setTournamentAdditionalTimeAfterCap(0);
    chessClock.setTournamentMoveCap2(0);
    chessClock.setTournamentPostCap2Increment(0);
    chessClock.setTournamentAdditionalTimeAfterCap2(0);
    chessClock.switchMoveCounter(true);
    chessClock.switchSoundIndicator(true);
  }
  setupAnimation();
}

void loop(){
  leftButtonState = analogRead(leftButton);
  rightButtonState = analogRead(rightButton);
  menuButtonState = analogRead(menuButton);
  backButtonState = analogRead(backButton);
  //LEFT BUTTON PRESS
  if(leftButtonState > 800 && !leftPressed){
    switch(currentScreen){
      case SCREEN_GAME:{
        if(gameStarted && chessClock.getCurrentPlayer() == PLAYER_NONE){
          break;
        }
        if (chessClock.getCurrentPlayer() != PLAYER_RIGHT)
        {
          if(chessClock.getGameResult() == PLAYER_NONE){
            if (chessClock.getSoundIndicatorEnabled()) tone(buzzerPin, 330, 50);
          }
          chessClock.playerMove(PLAYER_LEFT);
        }
        if(!isGameFinished){
          showCounterTime = MOVE_COUNTER_SHOW_TIME;
        }
        gameStarted = true;
        break;
      }
      case SCREEN_MENU_MODE:{
        currentScreen = SCREEN_MENU_PRESET;
        break;
      }
      case SCREEN_MENU_MODE_CHANGE:{
        if(chessClock.getClockMode() == 0){
          chessClock.setClockMode(5);
        }else{
          chessClock.setClockMode(chessClock.getClockMode()-1);
        }
        break; 
      }
      case SCREEN_MENU_TIME:{ 
        currentScreen = SCREEN_MENU_MODE;
        break;
      }
      case SCREEN_MENU_TIME_CHANGE:{
        switch(currentTimeChangeSegment){
          case 0:{
            if(chessClock.getLeftPlayerTime()>=3600000){
              chessClock.setLeftPlayerTime(chessClock.getLeftPlayerTime()-3600000);
            }
            break;
          }
          case 1:{
            if (chessClock.getLeftPlayerTime() >= 60000){
              chessClock.setLeftPlayerTime(chessClock.getLeftPlayerTime() - 60000);
            }
            break;
          }
          case 2:{
            if(chessClock.getRightPlayerTime()>=3600000){
              chessClock.setRightPlayerTime(chessClock.getRightPlayerTime()-3600000);
            }
            break;
          }
          case 3:{
            if (chessClock.getRightPlayerTime() >= 60000){
              chessClock.setRightPlayerTime(chessClock.getRightPlayerTime() - 60000);
            }
            break;
          }
        }
        break;
      }
      case SCREEN_MENU_INCREMENT:{
        currentScreen = SCREEN_MENU_TIME;
        break;
      }
      case SCREEN_MENU_INCREMENT_CHANGE:{      
        if(chessClock.getIncrementValue()<1000){
          chessClock.setIncrementValue(0);
        }else{
          chessClock.setIncrementValue(chessClock.getIncrementValue()/1000-1);
        }
        break;
      }
      case SCREEN_MENU_DELAY:{
        currentScreen = SCREEN_MENU_TIME;
        break;
      }
      case SCREEN_MENU_DELAY_CHANGE:{
        if(chessClock.getDelayValue()<1000){
          chessClock.setDelayValue(0);
        }else{
          chessClock.setDelayValue(chessClock.getDelayValue()/1000-1);
        }
        break;
      }
      case SCREEN_MENU_BRONSTEIN:{
        currentScreen = SCREEN_MENU_TIME;
        break;
      }
      case SCREEN_MENU_BRONSTEIN_CHANGE:{
        if(chessClock.getBronsteinValue()<1000){
          chessClock.setBronsteinValue(0);
        }else{
          chessClock.setBronsteinValue(chessClock.getBronsteinValue()/1000-1);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA:{
        currentScreen = SCREEN_MENU_TIME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA_CHANGE:{
        if (chessClock.getTournamentPreCapIncrement() < 1000){
          chessClock.setTournamentPreCapIncrement(0);
        }else{
          chessClock.setTournamentPreCapIncrement(chessClock.getTournamentPreCapIncrement() / 1000 - 1);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCA;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP_CHANGE:{
        if (chessClock.getTournamentMoveCap() < 1){
          chessClock.setTournamentMoveCap(0);
        }else{
          chessClock.setTournamentMoveCap(chessClock.getTournamentMoveCap() - 1);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE:{
        if (chessClock.getTournamentAdditionalTimeAfterCap() < 60000){
          chessClock.setTournamentAdditionalTimeAfterCap(0);
        }else{
          chessClock.setTournamentAdditionalTimeAfterCap(chessClock.getTournamentAdditionalTimeAfterCap() - 60000);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB_CHANGE:{
        if (chessClock.getTournamentPostCapIncrement() < 1000){
          chessClock.setTournamentPostCapIncrement(0);
        }else{
          chessClock.setTournamentPostCapIncrement(chessClock.getTournamentPostCapIncrement() / 1000 - 1);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCB;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2_CHANGE:{
        if (chessClock.getTournamentMoveCap2() > chessClock.getTournamentMoveCap()){
          chessClock.setTournamentMoveCap2(chessClock.getTournamentMoveCap2()-1);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP2;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE:{
        if (chessClock.getTournamentAdditionalTimeAfterCap2() < 60000){
          chessClock.setTournamentAdditionalTimeAfterCap2(0);
        }else{
          chessClock.setTournamentAdditionalTimeAfterCap2(chessClock.getTournamentAdditionalTimeAfterCap2() - 60000);
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME2;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC_CHANGE:{
        if (chessClock.getTournamentPostCap2Increment() < 1000){
          chessClock.setTournamentPostCap2Increment(0);
        }else{
          chessClock.setTournamentPostCap2Increment(chessClock.getTournamentPostCap2Increment() / 1000 - 1);
        }
        break;
      }
      case SCREEN_MENU_MOVES:{
        switch(chessClock.getClockMode()){
          case MODE_SUDDEN_DEATH:{
            currentScreen = SCREEN_MENU_TIME;
            break;
          }
          case MODE_INCREMENT:{
            currentScreen = SCREEN_MENU_INCREMENT;
            break;
          }
          case MODE_DELAY:{
            currentScreen = SCREEN_MENU_DELAY;
            break;
          }
          case MODE_HOURGLASS:{
            currentScreen = SCREEN_MENU_TIME;
            break;
          }
          case MODE_BRONSTEIN:{
            currentScreen = SCREEN_MENU_BRONSTEIN;
            break;
          }
          case MODE_TOURNAMENT:{
            currentScreen = SCREEN_MENU_TOURNAMENT_INCC;
            break;
          }
        }
        break;
      }
      case SCREEN_MENU_MOVES_CHANGE:{
        chessClock.switchMoveCounter(false);
        break;
      }
      case SCREEN_MENU_SOUND:{
        currentScreen = SCREEN_MENU_MOVES;
        break;
      }
      case SCREEN_MENU_SOUND_CHANGE:{
        chessClock.switchSoundIndicator(false);
        break;
      }
      case SCREEN_MENU_PRESET:{
        currentScreen = SCREEN_MENU_SOUND;
        break;
      }
      case SCREEN_MENU_PRESET_LOAD:{
        if(presetLoad>0){
          presetLoad--;
        }else{
          presetLoad = 18;
        }
        break;
      }
      /*case SCREEN_MENU_USER_PRESET:{
        currentScreen = SCREEN_MENU_PRESET;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD:{
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE:{
        currentScreen = SCREEN_MENU_USER_PRESET_LOAD;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD_PICK:{
      
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE_PICK:{

        break;
      }*/
    }
      leftPressed = true;
  }else if(leftButtonState<100){
    leftPressed = false;
  }
  //RIGHT BUTTON PRESS
  if(rightButtonState > 800 && !rightPressed){
    switch(currentScreen){
      case SCREEN_GAME:{
        if(gameStarted && chessClock.getCurrentPlayer() == PLAYER_NONE){
          break;
        }
        if (chessClock.getCurrentPlayer() != PLAYER_LEFT){
          if(chessClock.getGameResult() == PLAYER_NONE){
            if(chessClock.getSoundIndicatorEnabled()) tone(buzzerPin, 330, 50);
          }
          chessClock.playerMove(PLAYER_RIGHT);
        }
        if (!isGameFinished){
          showCounterTime = MOVE_COUNTER_SHOW_TIME;
        }
        gameStarted = true;
        break;
      }
      case SCREEN_MENU_MODE:{
        currentScreen = SCREEN_MENU_TIME;
        break;
      }
      case SCREEN_MENU_MODE_CHANGE:{
        if(chessClock.getClockMode() == 5){
          chessClock.setClockMode(0);
        }else{
          chessClock.setClockMode(chessClock.getClockMode()+1);
        }
        break;
      }
      case SCREEN_MENU_TIME:{
        switch(chessClock.getClockMode()){
          case MODE_SUDDEN_DEATH:{
            currentScreen = SCREEN_MENU_MOVES;
            break;
          }
          case MODE_INCREMENT:{
            currentScreen = SCREEN_MENU_INCREMENT;
            break;
          }
          case MODE_DELAY:{
            currentScreen = SCREEN_MENU_DELAY;
            break;
          }
          case MODE_HOURGLASS:{
            currentScreen = SCREEN_MENU_MOVES;
            break;
          }
          case MODE_BRONSTEIN:{
            currentScreen = SCREEN_MENU_BRONSTEIN;
            break;
          }
          case MODE_TOURNAMENT: {
            currentScreen = SCREEN_MENU_TOURNAMENT_INCA;
            break;
          }
        }
        break;
      }
      case SCREEN_MENU_TIME_CHANGE:{
        switch(currentTimeChangeSegment){
          case 0:{
            chessClock.setLeftPlayerTime(chessClock.getLeftPlayerTime() + 3600000);
            if (chessClock.getLeftPlayerTime() > 86400000){
              chessClock.setLeftPlayerTime(86400000);
            }
            break;
          }
          case 1:{
            chessClock.setLeftPlayerTime(chessClock.getLeftPlayerTime() + 60000);
            if (chessClock.getLeftPlayerTime() > 86400000){
              chessClock.setLeftPlayerTime(86400000);
            }
            break;
          }
          case 2:{
            chessClock.setRightPlayerTime(chessClock.getRightPlayerTime() + 3600000);
            if (chessClock.getRightPlayerTime() > 86400000){
              chessClock.setRightPlayerTime(86400000);
            }
            break;
          }
          case 3:{
            chessClock.setRightPlayerTime(chessClock.getRightPlayerTime() + 60000);
            if (chessClock.getRightPlayerTime() > 86400000){
              chessClock.setRightPlayerTime(86400000);
            }
            break;
          }
        }
        break;
      }
      case SCREEN_MENU_INCREMENT:{
        currentScreen = SCREEN_MENU_MOVES;
        break;
      }
      case SCREEN_MENU_INCREMENT_CHANGE:{
        if (chessClock.getIncrementValue()<180000) 
          chessClock.setIncrementValue(chessClock.getIncrementValue() / 1000 + 1);
        break;
      }
      case SCREEN_MENU_DELAY:{
        currentScreen = SCREEN_MENU_MOVES;
        break;
      }
      case SCREEN_MENU_DELAY_CHANGE:{
        if (chessClock.getDelayValue() < 180000)
          chessClock.setDelayValue(chessClock.getDelayValue() / 1000 + 1);
        break;
      }
      case SCREEN_MENU_BRONSTEIN:{
        currentScreen = SCREEN_MENU_MOVES;
        break;
      }
      case SCREEN_MENU_BRONSTEIN_CHANGE:{
        if (chessClock.getBronsteinValue() < 180000)
          chessClock.setBronsteinValue(chessClock.getBronsteinValue() / 1000 + 1);
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA_CHANGE:{
        if (chessClock.getTournamentPreCapIncrement()<180000)
          chessClock.setTournamentPreCapIncrement(chessClock.getTournamentPreCapIncrement() / 1000 + 1);
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP_CHANGE:{
        if(chessClock.getTournamentMoveCap()<255){
          chessClock.setTournamentMoveCap(chessClock.getTournamentMoveCap() + 1);
          if (chessClock.getTournamentMoveCap() > chessClock.getTournamentMoveCap2())
          {
            chessClock.setTournamentMoveCap2(chessClock.getTournamentMoveCap());
          }
        }
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCB;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE:{
        chessClock.setTournamentAdditionalTimeAfterCap(chessClock.getTournamentAdditionalTimeAfterCap() + 60000);
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP2;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB_CHANGE:{
        if (chessClock.getTournamentPostCapIncrement() < 180000)
          chessClock.setTournamentPostCapIncrement(chessClock.getTournamentPostCapIncrement() / 1000 + 1);
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME2;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2_CHANGE:{
        if(chessClock.getTournamentMoveCap2()<255)
          chessClock.setTournamentMoveCap2(chessClock.getTournamentMoveCap2() + 1);
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCC;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE:{
        chessClock.setTournamentAdditionalTimeAfterCap2(chessClock.getTournamentAdditionalTimeAfterCap2() + 60000);
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC:{
        currentScreen = SCREEN_MENU_MOVES;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC_CHANGE:{
        if (chessClock.getTournamentPostCap2Increment() < 180000)
          chessClock.setTournamentPostCap2Increment(chessClock.getTournamentPostCap2Increment() / 1000 + 1);
        break;
      }
      case SCREEN_MENU_MOVES:{
        currentScreen = SCREEN_MENU_SOUND;
        break;
      }
      case SCREEN_MENU_MOVES_CHANGE:{
        chessClock.switchMoveCounter(true);
        break;
      }
      case SCREEN_MENU_SOUND:{
        currentScreen = SCREEN_MENU_PRESET;
        break;
      }
      case SCREEN_MENU_SOUND_CHANGE:{
        chessClock.switchSoundIndicator(true);
        break;
      }
      case SCREEN_MENU_PRESET:{
        currentScreen = SCREEN_MENU_MODE;
        break;
      }
      case SCREEN_MENU_PRESET_LOAD:{
        if(presetLoad<18){
          presetLoad++;
        }else{
          presetLoad = 0;
        }
        break;
      }
      /*case SCREEN_MENU_USER_PRESET:{
        currentScreen = SCREEN_MENU_MODE;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD:{
        currentScreen = SCREEN_MENU_USER_PRESET_SAVE;
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE:{
      
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD_PICK:{
      
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE_PICK:{

        break;
      }*/
    }


    rightPressed = true;
  }else if(rightButtonState<100){
    rightPressed = false;
  }
  //BACK BUTTON PRESS
  if(backButtonState > 800 && !backPressed){
    if(chessClock.getSoundIndicatorEnabled()) tone(buzzerPin, 330, 50);
    switch(currentScreen){
      case SCREEN_GAME:{
        if(chessClock.getGameResult() != PLAYER_NONE){
          digitalWrite(resetPin,LOW);
        }
        if(chessClock.getCurrentPlayer() != PLAYER_NONE){
          lastPlayer = chessClock.getCurrentPlayer();
          chessClock.setCurrentPlayer(PLAYER_NONE);
          showCounterTime=0;
        }else{
          chessClock.setCurrentPlayer(lastPlayer);
        }
        break;
      }
      case SCREEN_MENU_MODE:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_MODE_CHANGE:{
        currentScreen = SCREEN_MENU_MODE;
        break;
      }
      case SCREEN_MENU_TIME:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TIME_CHANGE:{
        currentScreen = SCREEN_MENU_TIME;
        currentTimeChangeSegment = 0;
        break;
      }
      case SCREEN_MENU_INCREMENT:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_INCREMENT_CHANGE:{
        currentScreen = SCREEN_MENU_INCREMENT;
        break;
      }
      case SCREEN_MENU_DELAY:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_DELAY_CHANGE:{
        currentScreen = SCREEN_MENU_DELAY;
        break;
      }
      case SCREEN_MENU_BRONSTEIN:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_BRONSTEIN_CHANGE:{
        currentScreen = SCREEN_MENU_BRONSTEIN;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCA;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCB;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP2;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME2;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC_CHANGE:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCC;
        break;
      }
      case SCREEN_MENU_MOVES:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_MOVES_CHANGE:{
        currentScreen = SCREEN_MENU_MOVES;
        break;
      }
      case SCREEN_MENU_SOUND:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_SOUND_CHANGE:{
        currentScreen = SCREEN_MENU_SOUND;
        break;
      }
      case SCREEN_MENU_PRESET:{
        chessClock.saveToEEPROM(0);
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_PRESET_LOAD:{
        currentScreen = SCREEN_MENU_PRESET;
        break;
      }
      /*case SCREEN_MENU_USER_PRESET:{
        currentScreen = SCREEN_GAME;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD:{
        currentScreen = SCREEN_MENU_USER_PRESET;
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE:{
        currentScreen = SCREEN_MENU_USER_PRESET;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD_PICK:{
        currentScreen = SCREEN_MENU_USER_PRESET_LOAD;
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE_PICK:{
        currentScreen = SCREEN_MENU_USER_PRESET_SAVE;
        break;
      }*/
    }

    backPressed = true;
  }else if(backButtonState<100){
    backPressed = false;
  }
  //MENU BUTTON PRESS
  if(menuButtonState > 800 && !menuPressed){
    if(chessClock.getSoundIndicatorEnabled()) tone(buzzerPin, 330, 50);
    switch(currentScreen){
      case SCREEN_GAME:{
        if(!gameStarted){
          currentScreen = SCREEN_MENU_MODE;
        }else{
          digitalWrite(resetPin,LOW);
        }
        break;
      }
      case SCREEN_MENU_MODE:{
        currentScreen = SCREEN_MENU_MODE_CHANGE;
        break;
      }
      case SCREEN_MENU_MODE_CHANGE:{
        currentScreen = SCREEN_MENU_MODE_CHANGE;
        break;
      }
      case SCREEN_MENU_TIME:{
        currentScreen = SCREEN_MENU_TIME_CHANGE;
        break;
      }
      case SCREEN_MENU_TIME_CHANGE:{
        currentTimeChangeSegment++;
        if(currentTimeChangeSegment>3){
          currentTimeChangeSegment = 0;
        }
        break;
      }
      case SCREEN_MENU_INCREMENT:{
        currentScreen = SCREEN_MENU_INCREMENT_CHANGE;
        break;
      }
      case SCREEN_MENU_INCREMENT_CHANGE:{
    
      break;
    }
      case SCREEN_MENU_DELAY:{
        currentScreen = SCREEN_MENU_DELAY_CHANGE;
        break;  
      }
      case SCREEN_MENU_DELAY_CHANGE:{

        break;
      }
      case SCREEN_MENU_BRONSTEIN:{
        currentScreen = SCREEN_MENU_BRONSTEIN_CHANGE;
        break;
      }
      case SCREEN_MENU_BRONSTEIN_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCA_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCA_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE:{

        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCB_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCB_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2:{
        currentScreen = SCREEN_MENU_TOURNAMENT_CAP2_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_CAP2_CHANGE:{

        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2:{
        currentScreen = SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC:{
        currentScreen = SCREEN_MENU_TOURNAMENT_INCC_CHANGE;
        break;
      }
      case SCREEN_MENU_TOURNAMENT_INCC_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_MOVES:{
        currentScreen = SCREEN_MENU_MOVES_CHANGE;
        break;
      }
      case SCREEN_MENU_MOVES_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_SOUND:{
        currentScreen = SCREEN_MENU_SOUND_CHANGE;
        break;
      }
      case SCREEN_MENU_SOUND_CHANGE:{
      
        break;
      }
      case SCREEN_MENU_PRESET:{
        currentScreen = SCREEN_MENU_PRESET_LOAD;
        break;
      }
      case SCREEN_MENU_PRESET_LOAD:{
        chessClock.loadPreset(presetLoad);
        currentScreen = SCREEN_GAME;
        break;
      }
      /*case SCREEN_MENU_USER_PRESET:{
        currentScreen = SCREEN_MENU_USER_PRESET_LOAD;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD:{
        currentScreen = SCREEN_MENU_USER_PRESET_LOAD_PICK;
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE:{
        currentScreen = SCREEN_MENU_USER_PRESET_SAVE_PICK;
        break;
      }
      case SCREEN_MENU_USER_PRESET_LOAD_PICK:{
      
        break;
      }
      case SCREEN_MENU_USER_PRESET_SAVE_PICK:{

        break;
      }*/
    }
    menuPressed = true;
  }else if(menuButtonState<100){
    menuPressed = false;
  }

  //SCREEN DISPLAY
  switch(currentScreen){
    case SCREEN_GAME:{
      updateLed(chessClock.getCurrentPlayer());
      if(chessClock.getMoveCounterEnabled() && showCounterTime>0){
        if(chessClock.getCurrentPlayer() == PLAYER_LEFT){
          screenUpdateLeft(chessClock.getLeftPlayerTime());
          screenMovesRight(chessClock.getRightPlayerMoves());
        }else if(chessClock.getCurrentPlayer() == PLAYER_RIGHT){
          screenMovesLeft(chessClock.getLeftPlayerMoves());
          screenUpdateRight(chessClock.getRightPlayerTime());
        }else if(chessClock.getCurrentPlayer() == PLAYER_NONE){
          if(lastMilis%flickerTime>flickerTime/3){
            screenUpdateLeft(chessClock.getLeftPlayerTime());
            screenUpdateRight(chessClock.getRightPlayerTime());
          }else{
            screenUpdate(B0000000, B00000001, B0000000, B00000000, B0000000, B00000001, B0000000, B00000000);
          }
          screenUpdateLeft(chessClock.getLeftPlayerTime());
          screenUpdateRight(chessClock.getRightPlayerTime());
        }
        if(millis()-lastMilis>showCounterTime){
          showCounterTime = 0;
        }else{
          showCounterTime -= millis() - lastMilis;
        }      
      }else{
        if (chessClock.getCurrentPlayer() == PLAYER_NONE && gameStarted){
          if (lastMilis % flickerTime > flickerTime / 3){
            screenUpdateLeft(chessClock.getLeftPlayerTime());
            screenUpdateRight(chessClock.getRightPlayerTime());
          }
          else{
            screenUpdate(B0000000, B00000001, B0000000, B00000000, B0000000, B00000001, B0000000, B00000000);
          }
        }else{
          screenUpdateLeft(chessClock.getLeftPlayerTime());
          screenUpdateRight(chessClock.getRightPlayerTime());
        }

        }
      if (chessClock.getGameResult() != PLAYER_NONE && isGameFinished == false){
        if(chessClock.getSoundIndicatorEnabled()) gameEnded();
        isGameFinished = true;
      }
      chessClock.timeUpdate(millis() - lastMilis);
      break;
    }

    case SCREEN_MENU_MODE:{
      switch (chessClock.getClockMode()){
        case MODE_SUDDEN_DEATH:{
          screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B10110110, B00011110, B01111010);
          break;
        }
        case MODE_INCREMENT:{
          screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B01100000, B00101010, B00011010);
          break;
        }
        case MODE_DELAY:{
          screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B01111010, B10011110, B00011100);
          break;
        }
        case MODE_HOURGLASS:{
          screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B01101110, B00001010, B10111100);
          break;
        }
        case MODE_BRONSTEIN:{
          screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00111110, B00001010, B00101010, B01111010);
          break;
        }
        case MODE_TOURNAMENT:{
          screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00011110, B00001010, B00101010, B00011110);
          break;
        }
      }
      break;
    }

    case SCREEN_MENU_MODE_CHANGE:{
      if(lastMilis%flickerTime>flickerTime/4){
        switch (chessClock.getClockMode()){
          case MODE_SUDDEN_DEATH:{
            screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B10110110, B00011110, B01111010);
            break;
          }
          case MODE_INCREMENT:{
            screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B01100000, B00101010, B00011010);
            break;
          }
          case MODE_DELAY:{
            screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B01111010, B10011110, B00011100);
            break;
          }
          case MODE_HOURGLASS:{
            screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00000000, B01101110, B00001010, B10111100);
            break;
          }
          case MODE_BRONSTEIN:{
            screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00111110, B00001010, B00101010, B01111010);
            break;
          }
          case MODE_TOURNAMENT:{
            screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, B00011110, B00001010, B00101010, B00011110);
            break;
          }
        }
      }else{
        screenUpdate(digit[CHAR_S], B10011110, B00011110, B00011110, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    case SCREEN_MENU_TIME:{
      screenUpdate(digit[CHAR_H], digit[CHAR_H], digit[CHAR_N], digit[CHAR_N], digit[CHAR_H], digit[CHAR_H], digit[CHAR_N], digit[CHAR_N]);
      break;
    }

    case SCREEN_MENU_TIME_CHANGE:{
      int hl = milisToHours(chessClock.getLeftPlayerTime());
      int ml = milisToMinutes(chessClock.getLeftPlayerTime());
      int hr = milisToHours(chessClock.getRightPlayerTime());
      int mr = milisToMinutes(chessClock.getRightPlayerTime());

      switch (currentTimeChangeSegment){
        case 0:{
          if(lastMilis%flickerTime>flickerTime/4){
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[ml / 10], digit[ml % 10], digit[hr / 10], digit[hr % 10], digit[mr / 10], digit[mr % 10]);
          }else{
            screenUpdate(digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[ml / 10], digit[ml % 10], digit[hr / 10], digit[hr % 10], digit[mr / 10], digit[mr % 10]);
          }
          break;
        }
        case 1:{
          if(lastMilis%flickerTime>flickerTime/4){
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[ml / 10], digit[ml % 10], digit[hr / 10], digit[hr % 10], digit[mr / 10], digit[mr % 10]);
          }else{
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[hr / 10], digit[hr % 10], digit[mr / 10], digit[mr % 10]);
          }
          break;
        }
        case 2:{
          if(lastMilis%flickerTime>flickerTime/4){
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[ml / 10], digit[ml % 10], digit[hr / 10], digit[hr % 10], digit[mr / 10], digit[mr % 10]);
          }else{
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[ml / 10], digit[ml % 10], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[mr / 10], digit[mr % 10]);
          }
          break;
        }
        case 3:{
          if(lastMilis%flickerTime>flickerTime/4){
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[ml / 10], digit[ml % 10], digit[hr / 10], digit[hr % 10], digit[mr / 10], digit[mr % 10]);
          }else{
            screenUpdate(digit[hl / 10], digit[hl % 10], digit[ml / 10], digit[ml % 10], digit[hr / 10], digit[hr % 10], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
          }
          break;
        }
      }
      break;
    }
  
    case SCREEN_MENU_INCREMENT:{
      screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], digit[DIGIT_EMPTY], digit[CHAR_N], digit[CHAR_N], digit[CHAR_S], digit[CHAR_S]);
      break;
    }
    
    case SCREEN_MENU_INCREMENT_CHANGE:{
      int m = milisToMinutes(chessClock.getIncrementValue());
      int s = milisToSeconds(chessClock.getIncrementValue());
      if(lastMilis%flickerTime>flickerTime/4){
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], digit[DIGIT_EMPTY], digit[m/10], digit[m%10], digit[s/10], digit[s%10]);
      }else{
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    case SCREEN_MENU_DELAY:{
      screenUpdate(B01111010, B10011110, B00011100, digit[DIGIT_EMPTY], digit[CHAR_N], digit[CHAR_N], digit[CHAR_S], digit[CHAR_S]);
      break;
    }

    case SCREEN_MENU_DELAY_CHANGE:{
      int m = milisToMinutes(chessClock.getDelayValue());
      int s = milisToSeconds(chessClock.getDelayValue());
      if(lastMilis%flickerTime>flickerTime/4){
        screenUpdate(B01111010, B10011110, B00011100, digit[DIGIT_EMPTY], digit[m / 10], digit[m % 10], digit[s / 10], digit[s % 10]);
      }else{
        screenUpdate(B01111010, B10011110, B00011100, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    case SCREEN_MENU_BRONSTEIN:{
      screenUpdate(B00111110, B00001010, B00101010, B01111010, digit[CHAR_N], digit[CHAR_N], digit[CHAR_S], digit[CHAR_S]); 
      break;
    }

    case SCREEN_MENU_BRONSTEIN_CHANGE:{
      int m = milisToMinutes(chessClock.getBronsteinValue());
      int s = milisToSeconds(chessClock.getBronsteinValue());
      if(lastMilis%flickerTime>flickerTime/4){
        screenUpdate(B00111110, B00001010, B00101010, B01111010, digit[m / 10], digit[m % 10], digit[s / 10], digit[s % 10]);
      }else{
        screenUpdate(B00111110, B00001010, B00101010, B01111010, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }
  
    case SCREEN_MENU_TOURNAMENT_INCA:{
      screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B11101110, digit[CHAR_N], digit[CHAR_N], digit[CHAR_S], digit[CHAR_S]);
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_INCA_CHANGE:{
      int m = milisToMinutes(chessClock.getTournamentPreCapIncrement());
      int s = milisToSeconds(chessClock.getTournamentPreCapIncrement());
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B11101110, digit[m / 10], digit[m % 10], digit[s / 10], digit[s % 10]);
      }
      else{
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B11101110, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_CAP:{
      screenUpdate(B00011110,B10011100,B11101110,B11001110,B00000000,digit[chessClock.getTournamentMoveCap()/10+10],digit[chessClock.getTournamentMoveCap()%10+10], digit[DIGIT_EMPTY]);
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_CAP_CHANGE:{
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(B00011110, B10011100, B11101110, B11001110, B00000000, digit[chessClock.getTournamentMoveCap() / 10 + 10], digit[chessClock.getTournamentMoveCap() % 10 + 10], digit[DIGIT_EMPTY]);
      }
      else{
        screenUpdate(B00011110, B10011100, B11101110, B11001110, B00000000, B00000000, B00000000, B00000000);
      }
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_ADD_TIME:{
      screenUpdate(B00011110, B11101110, B01111010, B01111010, digit[CHAR_H], digit[CHAR_H], digit[CHAR_N], digit[CHAR_N]);
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_ADD_TIME_CHANGE:{
      int h = milisToHours(chessClock.getTournamentAdditionalTimeAfterCap());
      int m = milisToMinutes(chessClock.getTournamentAdditionalTimeAfterCap());
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(B00011110, B11101110, B01111010, B01111010, digit[h / 10], digit[h % 10], digit[m / 10], digit[m % 10]);
      }
      else{
        screenUpdate(B00011110, B11101110, B01111010, B01111010, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_INCB:{
      screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B00111110, digit[CHAR_N], digit[CHAR_N], digit[CHAR_S], digit[CHAR_S]);
      break;
    }
    
    case SCREEN_MENU_TOURNAMENT_INCB_CHANGE:{
      int m = milisToMinutes(chessClock.getTournamentPostCapIncrement());
      int s = milisToSeconds(chessClock.getTournamentPostCapIncrement());
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B00111110, digit[m / 10], digit[m % 10], digit[s / 10], digit[s % 10]);
      }
      else{
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B00111110, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }
  
    case SCREEN_MENU_TOURNAMENT_CAP2:{
      screenUpdate(B10011100, B11101110, B11001110, B00111110, B00000000, digit[chessClock.getTournamentMoveCap2() / 10 + 10], digit[chessClock.getTournamentMoveCap2() % 10 + 10], digit[DIGIT_EMPTY]);
      break;
    }

    case SCREEN_MENU_TOURNAMENT_CAP2_CHANGE:{
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(B10011100, B11101110, B11001110, B00111110, B00000000, digit[chessClock.getTournamentMoveCap2() / 10 + 10], digit[chessClock.getTournamentMoveCap2() % 10 + 10], digit[DIGIT_EMPTY]);
      }
      else{
        screenUpdate(B10011100, B11101110, B11001110, B00111110, B00000000, B00000000, B00000000, B00000000);
      }
      break;
    }

    case SCREEN_MENU_TOURNAMENT_ADD_TIME2:{
      screenUpdate(B11101110, B01111010, B01111010, B00111110, digit[CHAR_H], digit[CHAR_H], digit[CHAR_N], digit[CHAR_N]);
      break;
    }

    case SCREEN_MENU_TOURNAMENT_ADD_TIME2_CHANGE:{
      int h = milisToHours(chessClock.getTournamentAdditionalTimeAfterCap2());
      int m = milisToMinutes(chessClock.getTournamentAdditionalTimeAfterCap2());
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(B11101110, B01111010, B01111010, B00111110, digit[h / 10], digit[h % 10], digit[m / 10], digit[m % 10]);
      }
      else{
        screenUpdate(B11101110, B01111010, B01111010, B00111110, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    case SCREEN_MENU_TOURNAMENT_INCC:{
      screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B10011100, digit[CHAR_N], digit[CHAR_N], digit[CHAR_S], digit[CHAR_S]);
      break;
    }

    case SCREEN_MENU_TOURNAMENT_INCC_CHANGE:{
      int m = milisToMinutes(chessClock.getTournamentPostCap2Increment());
      int s = milisToSeconds(chessClock.getTournamentPostCap2Increment());
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B10011100, digit[m / 10], digit[m % 10], digit[s / 10], digit[s % 10]);
      }
      else{
        screenUpdate(digit[11], digit[CHAR_n], digit[CHAR_c], B10011100, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }
    
    case SCREEN_MENU_MOVES:{
      if(chessClock.getMoveCounterEnabled()){
        screenUpdate(B10011100, B00101010, B00011110, B00000000, B00000000, B11111100, B00101010, B00000000);
      }else{
        screenUpdate(B10011100, B00101010, B00011110, B00000000, B00000000, B11111100, B10001110, B10001110);
      }
      break;
    }

    case SCREEN_MENU_MOVES_CHANGE:{
      if (lastMilis % flickerTime > flickerTime / 4){
        if(chessClock.getMoveCounterEnabled()){
          screenUpdate(B10011100, B00101010, B00011110, B00000000, B00000000, B11111100, B00101010, B00000000);
        }else{
          screenUpdate(B10011100, B00101010, B00011110, B00000000, B00000000, B11111100, B10001110, B10001110);
        }
      }
      else{
        screenUpdate(B10011100, B00101010, B00011110, B00000000, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    case SCREEN_MENU_SOUND:{
      if(chessClock.getSoundIndicatorEnabled()){
        screenUpdate(B10110110, B00101010, B01111010, B00000000, B00000000, B11111100, B00101010, B00000000);
      }else{
        screenUpdate(B10110110, B00101010, B01111010, B00000000, B00000000, B11111100, B10001110, B10001110);
      }
      break;
    }

    case SCREEN_MENU_SOUND_CHANGE:{
      if (lastMilis % flickerTime > flickerTime / 4){
        if(chessClock.getSoundIndicatorEnabled()){
          screenUpdate(B10110110, B00101010, B01111010, B00000000, B00000000, B11111100, B00101010, B00000000);
        }else{
          screenUpdate(B10110110, B00101010, B01111010, B00000000, B00000000, B11111100, B10001110, B10001110);
        }
      }
      else{
        screenUpdate(B10110110, B00101010, B01111010, B00000000, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    case SCREEN_MENU_PRESET:{
      screenUpdate(B11001110, B00001010, B00011110, B00000000, B00000000, B00000000, B00000000, B00000000);
      break;
    }

    case SCREEN_MENU_PRESET_LOAD:{
      if (lastMilis % flickerTime > flickerTime / 4){
        switch(presetLoad){
          case PRESET_1i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, B01100001, B00000000, B11111100);
            break;
          }
          case PRESET_1i1:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, B01100001, B00000000, B01100001);
            break;
          }
          case PRESET_2i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, B11011011, B00000000, B11111100);
            break;
          }
          case PRESET_2i1:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, digit[2], B00000000, digit[1]);
            break;
          }
          case PRESET_3i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, digit[3], B00000000, digit[0]);
            break;
          }
          case PRESET_3i2:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, digit[3], B00000000, digit[2]);
            break;
          }
          case PRESET_5i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, digit[5], B00000000, digit[0]);
            break;
          }
          case PRESET_5i5:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B0000000, digit[5], B00000000, digit[5]);
            break;  
          }
          case PRESET_10i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[1], digit[0], B00000000, digit[0]);
            break;
          }
          case PRESET_10i5:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[1], digit[0], B00000000, digit[5]);
            break;
          }
          case PRESET_15i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[1], digit[5], B00000000, digit[0]);
            break;
          }
          case PRESET_15i10:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[1], digit[5], digit[1], digit[0]);
            break;
          }
          case PRESET_20i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[2], digit[0], B00000000, digit[0]);
            break;
          }
          case PRESET_30i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[3], digit[0], B00000000, digit[0]);
            break;
          }
          case PRESET_45i45:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[4], digit[5], digit[4], digit[5]);
            break;
          }
          case PRESET_60i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[6], digit[0], B00000000, digit[0]);
            break;
          }
          case PRESET_90i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[9], digit[0], B00000000, digit[0]);
            break;
          }
          case PRESET_90c40a30i30:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B10001110, B00011110, B00001010, B00101010);
            break;
          }
          case PRESET_120c40a60c60a40i0:{
            screenUpdate(B00011100, B00111010, B11101110, B01111010, B10001110, B00001100, B01111010, B10011110);
            break;
          }
        }
      }
      else{
        screenUpdate(B00011100, B00111010, B11101110, B01111010, digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY], digit[DIGIT_EMPTY]);
      }
      break;
    }

    /*case SCREEN_MENU_USER_PRESET:{
      screenUpdate(B01111100, B10110110, B00001010, B00000000, B11001110, B00001010, B00011110, B00000000);
      break;
    }

    case SCREEN_MENU_USER_PRESET_LOAD:{
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(B00011100, B00111010, B11101110, B01111010, B10110110, B11101110, B01111100, B10011110);
      }
      else{
        screenUpdate(B00000000, B00000000, B00000000, B0000000, B10110110, B11101110, B01111100, B10011110);
      }
      break;
    }

    case SCREEN_MENU_USER_PRESET_SAVE:{
      if (lastMilis % flickerTime > flickerTime / 4){
        screenUpdate(B00011100, B00111010, B11101110, B01111010, B10110110, B11101110, B01111100, B10011110);
      }
      else{
        screenUpdate(B00011100, B00111010, B11101110, B01111010, B00000000, B00000000, B00000000, B0000000);
      }
      break;
    }

    case SCREEN_MENU_USER_PRESET_LOAD_PICK:{
      screenUpdate(B00011100, B00111010, B11101110, B01111010, B00000000, B00000000, B00000000, B0000000);
      break;
    }

    case SCREEN_MENU_USER_PRESET_SAVE_PICK:{
      screenUpdate(B00000000, B00000000, B00000000, B0000000, B10110110, B11101110, B01111100, B10011110);
      break;
    }*/
  }
  curPlayer = chessClock.getCurrentPlayer();
  lastMilis = millis();
  digitalWrite(rightLED, LOW);
  digitalWrite(leftLED, LOW);
}
