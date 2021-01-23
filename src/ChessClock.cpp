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
    incrementValue = value*1000;
};
void ChessClock::setDelayValue(uint16_t value){
    delayValue = value*1000;
    delayLeft = delayValue;
};
void ChessClock::setBronsteinValue(uint16_t value){
    bronsteinValue = value*1000;
    bronsteinLeft = bronsteinValue;
};
void ChessClock::setTournamentMoveCap(uint16_t value){
    tournamentMoveCap = value;
};
void ChessClock::setTournamentPreCapIncrement(uint16_t value){
    tournamentPreCapIncrement = value*1000;
};
void ChessClock::setTournamentAdditionalTimeAfterCap(uint16_t value){
    tournamentAdditionalTimeAfterCap = value;
};
void ChessClock::setTournamentPostCapIncrement(uint16_t value){
    tournamentPostCapIncrement = value*1000;
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
int8_t ChessClock::getCurrentPlayer(){
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
uint16_t ChessClock::getTournamentPreCapIncrement(){
    return tournamentPreCapIncrement;
};
uint16_t ChessClock::getTournamentAdditionalTimeAfterCap(){
    return tournamentAdditionalTimeAfterCap;
};
uint16_t ChessClock::getTournamentPostCapIncrement(){
    return tournamentPostCapIncrement;
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
        case MODE_SUDDEN_DEATH:{
            if(currentPlayer == PLAYER_LEFT){
                if(time>leftPlayerTime){
                    leftPlayerTime = 0;
                    gameResult = PLAYER_RIGHT;
                }else{
                    leftPlayerTime -= time;
                }
            }else if(currentPlayer == PLAYER_RIGHT){
                if (time > rightPlayerTime){
                    rightPlayerTime = 0;
                    gameResult = PLAYER_LEFT;
                }
                else{
                    rightPlayerTime -= time;
                }
            }
            break;
        }
        case MODE_INCREMENT:{
            if (currentPlayer == PLAYER_LEFT){
                if (time > leftPlayerTime){
                    leftPlayerTime = 0;
                    gameResult = PLAYER_RIGHT;
                }
                else{
                    leftPlayerTime -= time;
                }
            }
            else if (currentPlayer == PLAYER_RIGHT){
                if (time > rightPlayerTime){
                    rightPlayerTime = 0;
                    gameResult = PLAYER_LEFT;
                }
                else{
                    rightPlayerTime -= time;
                }
            }
            break;
        }
        case MODE_DELAY:{
            if(currentPlayer == PLAYER_LEFT){
                if(time>leftPlayerTime){
                    leftPlayerTime = 0;
                    gameResult = PLAYER_RIGHT;
                }else if(delayLeft == 0){
                    leftPlayerTime -= time;
                }else{
                    if(time>delayLeft){
                        leftPlayerTime-= time-delayLeft;
                        delayLeft = 0;
                    }else{
                        delayLeft-= time;
                    }
                }
            }else if(currentPlayer == PLAYER_RIGHT){
                if (time > rightPlayerTime){
                    rightPlayerTime = 0;
                    gameResult = PLAYER_LEFT;
                }
                else if(delayLeft == 0){
                    rightPlayerTime -= time;
                }else{
                    if(time>delayLeft){
                        rightPlayerTime-= time-delayLeft;
                        delayLeft = 0;
                    }else{
                        delayLeft-= time;
                    }
                }
            }
            break;
        }
        case MODE_HOURGLASS:{
            if(currentPlayer == PLAYER_LEFT){
                if(time>leftPlayerTime){
                    rightPlayerTime += leftPlayerTime;
                    leftPlayerTime = 0;
                    gameResult = PLAYER_RIGHT;
                }else{
                    leftPlayerTime -= time;
                    rightPlayerTime += time;
                }
            }else if(currentPlayer == PLAYER_RIGHT){
                if (time > rightPlayerTime){
                    leftPlayerTime += rightPlayerTime; 
                    rightPlayerTime = 0;
                    gameResult = PLAYER_LEFT;
                }
                else{
                    rightPlayerTime -= time;
                    leftPlayerTime += time;
                }
            }
            break;
        }
        case MODE_BRONSTEIN:{
            if(currentPlayer == PLAYER_LEFT){
                if(time>leftPlayerTime){
                    leftPlayerTime = 0;
                    gameResult = PLAYER_RIGHT;
                }else{
                    leftPlayerTime -= time;
                    if(bronsteinLeft<time){
                        bronsteinLeft = 0;
                    }else{
                        bronsteinLeft -= time;
                    }
                }
            }else if(currentPlayer == PLAYER_RIGHT){
                if (time > rightPlayerTime){
                    rightPlayerTime = 0;
                    gameResult = PLAYER_LEFT;
                }
                else{
                    rightPlayerTime -= time;
                    if(bronsteinLeft<time){
                        bronsteinLeft = 0;
                    }else{
                        bronsteinLeft -= time;
                    }
                }
            }
            break;
        }
        case MODE_TOURNAMENT:{
            if (currentPlayer == PLAYER_LEFT){
                if (time > leftPlayerTime){
                    leftPlayerTime = 0;
                    gameResult = PLAYER_RIGHT;
                }
                else{
                    leftPlayerTime -= time;
                }
            }
            else if (currentPlayer == PLAYER_RIGHT){
                if (time > rightPlayerTime){
                    rightPlayerTime = 0;
                    gameResult = PLAYER_LEFT;
                }
                else{
                    rightPlayerTime -= time;
                }
            }
            break;
        }
    }
};
void ChessClock::playerMove(int8_t player){
    switch (clockMode){
        case MODE_SUDDEN_DEATH:{
            if(player == PLAYER_RIGHT && rightPlayerTime != 0){
                rightPlayerMoves++;
                currentPlayer = PLAYER_LEFT;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                leftPlayerMoves++;
                currentPlayer = PLAYER_RIGHT;
            }else if(player == PLAYER_NONE){
                currentPlayer = PLAYER_NONE;
            }
            break;
        }
        case MODE_INCREMENT:{
            if(player == PLAYER_RIGHT && rightPlayerTime != 0){
                rightPlayerMoves++;
                currentPlayer = PLAYER_LEFT;
                rightPlayerTime += incrementValue;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                leftPlayerMoves++;
                currentPlayer = PLAYER_RIGHT;
                leftPlayerTime += incrementValue;
            }else if(player == PLAYER_NONE){
                currentPlayer = PLAYER_NONE;
            }
            break;
        }
        case MODE_DELAY:{
            if(player == PLAYER_RIGHT && rightPlayerTime != 0){
                rightPlayerMoves++;
                currentPlayer = PLAYER_LEFT;
                delayLeft = delayValue;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                leftPlayerMoves++;
                currentPlayer = PLAYER_RIGHT;
                delayLeft = delayValue;
            }else if(player == PLAYER_NONE){
                currentPlayer = PLAYER_NONE;
            }
            break;
        }
        case MODE_HOURGLASS:{
            if(player == PLAYER_RIGHT && rightPlayerTime != 0){
                rightPlayerMoves++;
                currentPlayer = PLAYER_LEFT;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                leftPlayerMoves++;
                currentPlayer = PLAYER_RIGHT;
            }else if(player == PLAYER_NONE){
                currentPlayer = PLAYER_NONE;
            }
            break;
        }
        case MODE_BRONSTEIN:{
            if(player == PLAYER_RIGHT && rightPlayerTime != 0){
                rightPlayerMoves++;
                currentPlayer = PLAYER_LEFT;
                rightPlayerTime += bronsteinValue - bronsteinLeft;
                bronsteinLeft = bronsteinValue;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                leftPlayerMoves++;
                currentPlayer = PLAYER_RIGHT;
                leftPlayerTime += bronsteinValue - bronsteinLeft;
                bronsteinLeft = bronsteinValue;
            }else if(player == PLAYER_NONE){
                currentPlayer = PLAYER_NONE;            
            }
            break;
        }
        case MODE_TOURNAMENT:{
            if(player == PLAYER_RIGHT && rightPlayerTime != 0){
                if(rightPlayerMoves<tournamentMoveCap){
                    rightPlayerTime += tournamentPreCapIncrement;
                }else{
                    rightPlayerTime += tournamentPostCapIncrement;
                }
                rightPlayerMoves++;
                if(rightPlayerMoves == tournamentMoveCap){
                    rightPlayerTime += tournamentAdditionalTimeAfterCap;
                }
                currentPlayer = PLAYER_LEFT;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                if(rightPlayerMoves<tournamentMoveCap){
                    leftPlayerTime += tournamentPreCapIncrement;
                }else{
                    leftPlayerTime += tournamentPostCapIncrement;
                }
                leftPlayerMoves++;
                if(leftPlayerMoves == tournamentMoveCap){
                    leftPlayerTime += tournamentAdditionalTimeAfterCap;
                }
                currentPlayer = PLAYER_RIGHT;
            }else if(player == PLAYER_NONE){
                currentPlayer = PLAYER_NONE;
            }
            break;
        }
    }
};
void ChessClock::loadPreset(byte preset){
    
};

ChessClock::ChessClock(){
    currentPlayer = PLAYER_NONE;
    gameResult = PLAYER_NONE;
    leftPlayerMoves = 0;
    rightPlayerMoves = 0;
}
