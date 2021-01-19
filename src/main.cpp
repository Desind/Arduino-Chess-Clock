#include <Arduino.h>

#define DIGIT_EMPTY 10

const byte digit[11] = {
    B11111101,     //0
    B01100001,     //1
    B11011011,     //2
    B11110011,     //3
    B01100111,     //4
    B10110111,     //5
    B10111111,     //6
    B11100101,     //7
    B11111111,     //8
    B11110111,     //9
    B00000001      //empty
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
const PROGMEM int blackLED = A0;
const PROGMEM int whiteLED = A1;
const PROGMEM int blackButton = A3;
const PROGMEM int whiteButton = A2;
const PROGMEM int buzzerPin = A4;
const PROGMEM int menuButton = A6;
const PROGMEM int backButton = A7;

int digitDelay = 1;

int blackButtonState = 0;
int whiteButtonState = 0;
int optionButtonState = 0;

bool blackPressed = false;
bool whitePressed = false;
bool optionPressed = false;

long blackTime = 3600000;
long whiteTime = 3600000;

long lastMilis = 0;
//-1 right
//1 left
int currentTurn = 0;

void screenUpdateWhite(int C1, int C2, int C3, int C4){
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
void screenUpdateBlack(int C5, int C6, int C7, int C8){
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
    pinMode(blackButton, INPUT);
    pinMode(whiteButton, INPUT);
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
    digitalWrite(digit5, HIGH);
    digitalWrite(digit6, HIGH);
    digitalWrite(digit7, HIGH);
    digitalWrite(digit8, HIGH);
    lastMilis = millis();
  }

void loop(){
    blackButtonState = digitalRead(blackButton);
    whiteButtonState = digitalRead(whiteButton);
    if (blackButtonState == HIGH && !blackPressed){
      digitalWrite(blackLED, HIGH);
      digitalWrite(whiteLED, LOW);
      tone(A5, 330, 50);
      currentTurn = 1;

      blackPressed = true;
    }else if(blackButtonState == LOW){
      blackPressed = false;
    }

    if (whiteButtonState == HIGH && !whitePressed){
      digitalWrite(whiteLED, HIGH);
      digitalWrite(blackLED, LOW);
      tone(A5, 330, 50);
      currentTurn = -1;

      whitePressed = true;
    }else if(whiteButtonState == LOW){
      whitePressed = false;
    }

    if(optionButtonState == HIGH && !optionPressed){

      tone(A5, 330, 50);
      optionPressed = true;
    }else if(optionButtonState == LOW){
      whitePressed = false;
    }

    long whiteMinutes = whiteTime / 60000;
    long whiteSeconds = (whiteTime - whiteMinutes * 60000)/1000;
    long blackMinutes = blackTime / 60000;
    long blackSeconds = (blackTime - blackMinutes * 60000)/1000;
    screenUpdateWhite(whiteMinutes / 10 % 10, whiteMinutes % 10, whiteSeconds / 10 % 10, whiteSeconds % 10);
    screenUpdateBlack(blackMinutes / 10 % 10, blackMinutes % 10, blackSeconds / 10 % 10, blackSeconds % 10);
    if(currentTurn>0){
      whiteTime -= millis() - lastMilis;
    }
    if(currentTurn<0){
      blackTime -= millis() - lastMilis;
    }
    lastMilis=millis();
}
