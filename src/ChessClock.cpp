#include <ChessClock.h>

void ChessClock::setLeftPlayerTime(uint32_t time){
    leftPlayerTime = time;
};
void ChessClock::setRightPlayerTime(uint32_t time){
    rightPlayerTime = time;
};
void ChessClock::setClockMode(byte mode){
    clockMode = mode;
};
void ChessClock::setIncrementValue(uint16_t value){
    incrementValue = value;
};
void ChessClock::setDelayValue(uint16_t value){
    delayValue = value;
};
void ChessClock::setBronsteinValue(uint16_t value){
    bronsteinValue = value;
};
void ChessClock::setTournamentMoveCap(uint16_t value){
    tournamentMoveCap = value;
};
void ChessClock::setTournamentPreCapIncrement(uint16_t value){
    torunamentPreCapIncrement = value;
};
void ChessClock::setTournamentAdditionalTimeAfterCap(uint16_t value){
    tournamentAdditionalTimeAfterCap = value;
};
void ChessClock::setTournamentPostCapIncrement(uint16_t value){
    torunamentPostCapIncrement = value;
};
void ChessClock::switchLedIndicator(boolean mode){
    ledIndicatorEnabled = mode;
};
void ChessClock::switchSoundIndicator(boolean mode){
    soundIndicatorEnabled = mode;
};
void ChessClock::switchMoveCounter(boolean mode){
    moveCounterEnabled = mode;
};

uint16_t ChessClock::getLeftPlayerTimeAsSeconds(){
    return (uint16_t)(leftPlayerTime/1000);
};
uint16_t ChessClock::getRightPlayerTimeAsSeconds(){
    return (uint16_t)(rightPlayerTime/1000);
};
uint32_t ChessClock::getLeftPlayerTime(){
    return leftPlayerTime;
};
uint32_t ChessClock::getRightPlayerTime(){
    return rightPlayerTime;
};
uint16_t ChessClock::getLeftPlayerMoves(){
    return leftPlayerMoves;
};
uint16_t ChessClock::getRightPlayerMoves(){
    return rightPlayerMoves;
};
byte ChessClock::getCurrentPlayer(){
    return currentPlayer;
};
byte ChessClock::getGameResult(){
    return gameResult;
};
byte ChessClock::getClockMode(){
    return clockMode;
};
uint16_t ChessClock::getIncrementValue(){
    return incrementValue;
};
uint16_t ChessClock::getDelayValue(){
    return delayValue;
};
uint16_t ChessClock::getBronsteinValue(){
    return bronsteinValue;
};
uint16_t ChessClock::getTournamentMoveCap(){
    return tournamentMoveCap;
};
uint16_t ChessClock::getTorunamentPreCapIncrement(){
    return torunamentPreCapIncrement;
};
uint16_t ChessClock::getTournamentAdditionalTimeAfterCap(){
    return tournamentAdditionalTimeAfterCap;
};
uint16_t ChessClock::getTorunamentPostCapIncrement(){
    return torunamentPostCapIncrement;
};
boolean ChessClock::getLedIndicatorEnabled(){
    return ledIndicatorEnabled;
};
boolean ChessClock::getSoundIndicatorEnabled(){
    return soundIndicatorEnabled;
};
boolean ChessClock::getMoveCounterEnabled(){
    return moveCounterEnabled;
};
void ChessClock::timeUpdate(uint32_t time){
    switch(clockMode){
        case MODE_SIMPLE:{
        
            break;
        }
        case MODE_INCREMENT:{
        
            break;
        }
        case MODE_DELAY:{
        
            break;
        }
        case MODE_HOURGLASS:{
        
            break;
        }
        case MODE_TORUNAMENT:{
        
            break;
        }
    }
};
void ChessClock::playerMove(byte player){
    switch (clockMode){
        case MODE_SIMPLE:{
    
            break;
        }
        case MODE_INCREMENT:{

            break;
        }
        case MODE_DELAY:{

            break;
        }
        case MODE_HOURGLASS:{

            break;
        }
        case MODE_TORUNAMENT:{

            break;
        }
    }
};
void ChessClock::loadPreset(byte preset){
    
};
