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
void ChessClock::setIncrementValue(uint32_t value){
    incrementValue = value*1000;
};
void ChessClock::setDelayValue(uint32_t value){
    delayValue = value*1000;
    delayLeft = delayValue;
};
void ChessClock::setBronsteinValue(uint32_t value){
    bronsteinValue = value*1000;
    bronsteinLeft = bronsteinValue;
};
void ChessClock::setTournamentMoveCap(uint32_t value){
    tournamentMoveCap = value;
};
void ChessClock::setTournamentPreCapIncrement(uint32_t value){
    tournamentPreCapIncrement = value*1000;
};
void ChessClock::setTournamentAdditionalTimeAfterCap(uint32_t value){
    tournamentAdditionalTimeAfterCap = value;
};
void ChessClock::setTournamentPostCapIncrement(uint32_t value){
    tournamentPostCapIncrement = value*1000;
};
void ChessClock::switchSoundIndicator(boolean mode){
    soundIndicatorEnabled = mode;
};
void ChessClock::switchMoveCounter(boolean mode){
    moveCounterEnabled = mode;
};
void ChessClock::setCurrentPlayer(int8_t player){
    currentPlayer = player;
}
void ChessClock::setTournamentMoveCap2(uint32_t value){
    tournamentMoveCap2 = value;
};
void ChessClock::setTournamentPostCap2Increment(uint32_t value){
    tournamentPostCap2Increment = 1000*value;
};
void ChessClock::setTournamentAdditionalTimeAfterCap2(uint32_t value){
    tournamentAdditionalTimeAfterCap2 = value;
};
uint32_t ChessClock::getLeftPlayerTime(){
    return leftPlayerTime;
};
uint32_t ChessClock::getRightPlayerTime(){
    return rightPlayerTime;
};
uint32_t ChessClock::getLeftPlayerMoves(){
    return leftPlayerMoves;
};
uint32_t ChessClock::getRightPlayerMoves(){
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
uint32_t ChessClock::getIncrementValue(){
    return incrementValue;
};
uint32_t ChessClock::getDelayValue(){
    return delayValue;
};
uint32_t ChessClock::getBronsteinValue(){
    return bronsteinValue;
};
uint32_t ChessClock::getTournamentMoveCap(){
    return tournamentMoveCap;
};
uint32_t ChessClock::getTournamentPreCapIncrement(){
    return tournamentPreCapIncrement;
};
uint32_t ChessClock::getTournamentAdditionalTimeAfterCap(){
    return tournamentAdditionalTimeAfterCap;
};
uint32_t ChessClock::getTournamentPostCapIncrement(){
    return tournamentPostCapIncrement;
};
boolean ChessClock::getSoundIndicatorEnabled(){
    return soundIndicatorEnabled;
};
boolean ChessClock::getMoveCounterEnabled(){
    return moveCounterEnabled;
};
uint32_t ChessClock::getTournamentMoveCap2(){
    return tournamentMoveCap2;
};
uint32_t ChessClock::getTournamentPostCap2Increment(){
    return tournamentPostCap2Increment;
};
uint32_t ChessClock::getTournamentAdditionalTimeAfterCap2(){
    return tournamentAdditionalTimeAfterCap2;
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
                }else if(rightPlayerMoves < tournamentMoveCap2 && rightPlayerMoves >= tournamentMoveCap){
                    rightPlayerTime += tournamentPostCapIncrement;
                }else{
                    rightPlayerTime += tournamentPostCap2Increment;
                }
                rightPlayerMoves++;
                if(rightPlayerMoves == tournamentMoveCap){
                    rightPlayerTime += tournamentAdditionalTimeAfterCap;
                }
                if (rightPlayerMoves == tournamentMoveCap2)
                {
                    rightPlayerTime += tournamentAdditionalTimeAfterCap2;
                }
                currentPlayer = PLAYER_LEFT;
            }else if(player == PLAYER_LEFT && leftPlayerTime != 0){
                if(leftPlayerMoves<tournamentMoveCap){
                    leftPlayerTime += tournamentPreCapIncrement;
                }else if(leftPlayerMoves < tournamentMoveCap2 && leftPlayerMoves >= tournamentMoveCap){
                    leftPlayerTime += tournamentPostCapIncrement;
                }else{
                    leftPlayerTime += tournamentPostCap2Increment;
                }
                leftPlayerMoves++;
                if(leftPlayerMoves == tournamentMoveCap){
                    leftPlayerTime += tournamentAdditionalTimeAfterCap;
                }
                if (leftPlayerMoves == tournamentMoveCap2)
                {
                    leftPlayerTime += tournamentAdditionalTimeAfterCap2;
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
    incrementValue = 0;
    delayValue = 0;
    bronsteinValue = 0;
    tournamentMoveCap = 0;
    tournamentPreCapIncrement = 0;
    tournamentAdditionalTimeAfterCap = 0;
    tournamentPostCapIncrement = 0;
    tournamentMoveCap2 = 0;
    tournamentPostCap2Increment = 0;
    tournamentAdditionalTimeAfterCap2 = 0;
    soundIndicatorEnabled = false;
    moveCounterEnabled = false;
    switch(preset){
        case PRESET_1i0:{
            leftPlayerTime = 60000;
            rightPlayerTime = 60000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_1i1:{
            leftPlayerTime = 60000;
            rightPlayerTime = 60000;
            clockMode = MODE_INCREMENT;
            incrementValue = 1000;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_2i0:{
            leftPlayerTime = 120000;
            rightPlayerTime = 120000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_2i1:{
            leftPlayerTime = 120000;
            rightPlayerTime = 120000;
            clockMode = MODE_INCREMENT;
            incrementValue = 1000;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_3i0:{
            leftPlayerTime = 180000;
            rightPlayerTime = 180000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_3i2:{
            leftPlayerTime = 180000;
            rightPlayerTime = 180000;
            clockMode = MODE_INCREMENT;
            incrementValue = 2000;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_5i0:{
            leftPlayerTime = 300000;
            rightPlayerTime = 300000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_5i5:{
            leftPlayerTime = 300000;
            rightPlayerTime = 300000;
            clockMode = MODE_INCREMENT;
            incrementValue = 5000;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_10i0:{
            leftPlayerTime = 600000;
            rightPlayerTime = 600000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_10i5:{
            leftPlayerTime = 600000;
            rightPlayerTime = 600000;
            clockMode = MODE_INCREMENT;
            incrementValue = 5000;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_15i0:{
            leftPlayerTime = 900000;
            rightPlayerTime = 900000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            break;
        }
        case PRESET_15i10:{
            leftPlayerTime = 900000;
            rightPlayerTime = 900000;
            clockMode = MODE_INCREMENT;
            incrementValue = 10000;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
        case PRESET_20i0:{
            leftPlayerTime = 1200000;
            rightPlayerTime = 1200000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
        case PRESET_30i0:{
            leftPlayerTime = 1800000;
            rightPlayerTime = 1800000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
        case PRESET_45i45:{
            leftPlayerTime = 2700000;
            rightPlayerTime = 2700000;
            clockMode = MODE_INCREMENT;
            incrementValue = 45000;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
        }
        case PRESET_60i0:{
            leftPlayerTime = 3600000;
            rightPlayerTime = 3600000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
        case PRESET_90i0:{
            leftPlayerTime = 5400000;
            rightPlayerTime = 5400000;
            clockMode = MODE_SUDDEN_DEATH;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
        case PRESET_90c40a30i30:{
            leftPlayerTime = 5400000;
            rightPlayerTime = 5400000;
            clockMode = MODE_TOURNAMENT;
            tournamentMoveCap = 40;
            tournamentPreCapIncrement = 0;
            tournamentAdditionalTimeAfterCap = 1800000;
            tournamentPostCapIncrement = 0;
            tournamentMoveCap2 = 40;
            tournamentPostCap2Increment = 30000;
            tournamentAdditionalTimeAfterCap2 = 0;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
        case PRESET_120c40a60c60a40i0:{
            leftPlayerTime = 7200000;
            rightPlayerTime = 7200000;
            clockMode = MODE_TOURNAMENT;
            tournamentMoveCap = 40;
            tournamentPreCapIncrement = 0;
            tournamentAdditionalTimeAfterCap = 3600000;
            tournamentPostCapIncrement = 0;
            tournamentMoveCap2 = 60;
            tournamentPostCap2Increment = 30000;
            tournamentAdditionalTimeAfterCap2 = 2400000;
            soundIndicatorEnabled = true;
            moveCounterEnabled = true;
            break;
        }
    }
};

ChessClock::ChessClock(){
    currentPlayer = PLAYER_NONE;
    gameResult = PLAYER_NONE;
    leftPlayerMoves = 0;
    rightPlayerMoves = 0;
}
