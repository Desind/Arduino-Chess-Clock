#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

#define MODE_SUDDEN_DEATH 0
#define MODE_INCREMENT 1
#define MODE_DELAY 2
#define MODE_HOURGLASS 3
#define MODE_BRONSTEIN 4
#define MODE_TOURNAMENT 5

#define PLAYER_LEFT -1
#define PLAYER_RIGHT 1
#define PLAYER_NONE 0

class ChessClock
{
    public:
        uint32_t leftPlayerTime;                    //TIME LEFT FOR LEFT PLAYER
        uint32_t rightPlayerTime;                   //TIME LEFT FOR RIGHT PLAYER
        uint16_t leftPlayerMoves;                   //MOVE COUNT FOR LEFT PLAYER    
        uint16_t rightPlayerMoves;                  //MOVE COUNT FOR RIGHT PLAYER
        int8_t currentPlayer;                       //TURN OF WHICH PLAYER IS NOW
        byte gameResult;                            //RESULT OF THE GAME (GAME ENDED / GAME RUNNING)
        byte clockMode;                             //CURRENT MODE OF THE CLOCK
        uint16_t incrementValue;                    //VALUE OF INCREMENT TIME
        uint16_t delayValue;                        //VALUE OF DELAY TIME
        uint16_t delayLeft;                         //HOW MUCH DELAY IS LEFT FOR CURRENT PLAYER
        uint32_t bronsteinLeft;                     //START OF LEFT PLAYER TURN - NEEDED TO CALCULATE TIME ADDED AFTER TURN
        uint16_t bronsteinValue;                    //VALUE OF BRONSTEIN INCREMENT TIME
        uint16_t tournamentMoveCap;                 //HOW MANY MOVES UNTIL ADDITIONAL TIME
        uint16_t tournamentPreCapIncrement;         //INCREMENT BEFORE MOVE CAP
        uint16_t tournamentAdditionalTimeAfterCap;  //ADDITIONAL TIME ADDED TO TIMER AFTER MOVE CAP 
        uint16_t tournamentPostCapIncrement;        //INCREMENT AFTER MOVE CAP
        boolean ledIndicatorEnabled;                //LED MOVE INDICATOR TOGGLE
        boolean soundIndicatorEnabled;              //SOUND MOVE INDICATOR TOGGLE
        boolean moveCounterEnabled;                 //VALUE TO DETERMINE IF THERE IS NEED TO DRAW MOVE COUNTER

    public:

        ChessClock();

        void setLeftPlayerTime(uint32_t time);
        void setRightPlayerTime(uint32_t time);
        void setClockMode(byte clockMode);
        void setIncrementValue(uint16_t value);
        void setDelayValue(uint16_t value);
        void setBronsteinValue(uint16_t value);
        void setTournamentMoveCap(uint16_t value);
        void setTournamentPreCapIncrement(uint16_t value);
        void setTournamentAdditionalTimeAfterCap(uint16_t value);
        void setTournamentPostCapIncrement(uint16_t value);
        void switchLedIndicator(boolean mode);
        void switchSoundIndicator(boolean mode);
        void switchMoveCounter(boolean mode);

        void timeUpdate(uint32_t time); //UPDATE CLOCK TIME (SUBTRACT TIME FROM CURRENT PLAYER)
        void playerMove(int8_t player);   //SET CURRENT MOVE TO OPPONENT (ADD OVERDUE TIME FROM INCREMENT)

        void loadPreset(byte preset);   //LOAD CHESS CLOCK PRESET FROM MEMORY

        uint32_t getLeftPlayerTime();
        uint32_t getRightPlayerTime();
        uint16_t getLeftPlayerMoves();
        uint16_t getRightPlayerMoves();
        int8_t getCurrentPlayer();
        byte getGameResult();
        byte getClockMode();
        uint16_t getIncrementValue();
        uint16_t getDelayValue();
        uint16_t getBronsteinValue();
        uint16_t getTournamentMoveCap();          
        uint16_t getTournamentPreCapIncrement();       
        uint16_t getTournamentAdditionalTimeAfterCap(); 
        uint16_t getTournamentPostCapIncrement();
        boolean getLedIndicatorEnabled();
        boolean getSoundIndicatorEnabled();
        boolean getMoveCounterEnabled();

};

#endif